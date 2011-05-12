#include "reactor.h"
#include "list.h"
#include <sys/select.h>
#include <stdlib.h>

#define MAX_WAITING 16

struct reactor_t {
  List waiting;
  List ready;
  EventHandler* running;
  
  fd_set rdset;
  fd_set wrset;
  fd_set exset;
  int max_fd;
};
typedef struct reactor_t Reactor;

static Reactor r;

int
compare_prio (const void* a, const void* b)
{
  EventHandler* eh1 = *(EventHandler**) a;
  EventHandler* eh2 = *(EventHandler**) b;
  if (eh1->prio > eh2->prio)
    return -1;
  if (eh1->prio < eh2->prio)
    return 1;
  return 0;
}

int
timeval_compare (struct timeval* t1, struct timeval* t2)
{
  t1->tv_sec += t1->tv_usec / 1000000;
  t1->tv_usec %= 1000000;
  t2->tv_sec += t2->tv_usec / 1000000;
  t2->tv_usec %= 1000000;
  if (t1->tv_sec < t2->tv_sec)
    return -1;
  if (t1->tv_sec > t2->tv_sec)
    return 1;
  if (t1->tv_usec < t2->tv_usec)
    return -1;
  if (t1->tv_usec > t2->tv_usec)
    return 1;
  return 0;
}

void
event_handler_init (EventHandler* eh, int prio, eh_func_t run)
{
  eh->prio = prio;
  gettimeofday (&eh->next_activation, NULL);
  eh->run = run;
  eh->fd = -1;
  eh->events = 0;
}

void
event_handler_run (EventHandler* eh, Train_env* train_env)
{
  eh->run (eh, train_env);
}

void
reactor_init (void)
{
  list_init (&r.waiting, (compare_func_t) compare_prio);
  list_init (&r.ready, (compare_func_t) compare_prio);
  r.running = NULL;

  FD_ZERO (&r.rdset);
  FD_ZERO (&r.wrset);
  FD_ZERO (&r.exset);
  r.max_fd = -1;
}

void
reactor_add_handler (EventHandler* eh)
{
  list_append_sorted (&r.ready, eh);
}

void
reactor_yield (void)
{
  list_prepend_sorted (&r.ready, r.running);
  r.running = NULL;
}

void
reactor_activate (EventHandler* eh)
{
  list_remove (&r.waiting, eh);
  eh->events = 0;
  list_append_sorted (&r.ready, eh);
}

void
reactor_wait_event (int fd, int events)
{
  EventHandler* eh = r.running;
  eh->fd = fd;
  eh->events = events;
  list_append_sorted (&r.waiting, eh);
  if (events & EVENTS_READ)
    FD_SET (fd, &r.rdset);
  if (events & EVENTS_WRITE)
    FD_SET (fd, &r.wrset);
  if (events & EVENTS_EXCEPTION)
    FD_SET (fd, &r.exset);
  if (r.max_fd < fd)
    r.max_fd = fd;
  r.running = NULL;
}

void
reactor_delay (int ms)
{
  struct timeval tv;
  gettimeofday (&tv, NULL);
  tv.tv_sec += ms/1000;
  tv.tv_usec += (ms%1000)*1000;
  reactor_delay_until (&tv);
}

void
reactor_delay_until (struct timeval* tv)
{
  EventHandler* eh = r.running;
  eh->next_activation = *tv;
  reactor_wait_event (-1, EVENTS_TIMEOUT);
}

static struct timeval*
reactor_next_timeout (void)
{
  static struct timeval tv;
  struct timeval now;
  struct timeval next_activation;

  gettimeofday (&now, NULL);
  next_activation = now;
  next_activation.tv_sec += 60*60*24;

  if (list_empty (&r.ready) && list_empty (&r.waiting)) {
    return NULL; /* wait for ever */
  }

  tv.tv_usec = 0;
  tv.tv_sec = 0;
  if (!list_empty (&r.ready))
    return &tv;

  void** it;
  for (it = list_begin (&r.waiting);
       it != list_end (&r.waiting);
       it = list_next (&r.waiting, it)) {
    EventHandler* eh = (EventHandler*) *it;
    if (timeval_compare (&eh->next_activation, &next_activation) < 0)
      next_activation = eh->next_activation;
  }
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  if (timeval_compare (&next_activation, &now) > 0) {
    while (next_activation.tv_usec < now.tv_usec) {
      next_activation.tv_usec += 1000000;
      next_activation.tv_sec --;
    }
    tv.tv_sec = next_activation.tv_sec - now.tv_sec;
    tv.tv_usec = next_activation.tv_usec - now.tv_usec;
  }
  return &tv;
}

void
reactor_handle_events (Train_env* train_env)
{
  int n;
  struct timeval now;
  fd_set rdset = r.rdset;
  fd_set wrset = r.wrset;
  fd_set exset = r.exset;
  struct timeval* timeout = NULL;

  if (r.running)
    reactor_yield ();

  timeout = reactor_next_timeout ();
  n = select (r.max_fd + 1, &rdset, &wrset, &exset, timeout);
  gettimeofday (&now, NULL);
  if (n >= 0) { /* I/O event */
    void** it = list_begin (&r.waiting);
    while (it != list_end (&r.waiting)) {
      EventHandler* eh = (EventHandler*) *it;
      if ((eh->events & EVENTS_READ) && FD_ISSET(eh->fd, &rdset)) {
	FD_CLR(eh->fd, &rdset);
	reactor_activate (eh);
	it = list_begin (&r.waiting);
	continue;
      }
      if ((eh->events & EVENTS_WRITE) && FD_ISSET(eh->fd, &wrset)) {
	FD_CLR(eh->fd, &wrset);
	reactor_activate (eh);
	it = list_begin (&r.waiting);
	continue;
      }
      if ((eh->events & EVENTS_EXCEPTION) && FD_ISSET(eh->fd, &exset)) {
	FD_CLR(eh->fd, &exset);
	reactor_activate (eh);
	it = list_begin (&r.waiting);
	continue;
      }
      if ((eh->events & EVENTS_TIMEOUT) && 
	  (timeval_compare (&eh->next_activation, &now) <= 0)) {
	reactor_activate (eh);
	it = list_begin (&r.waiting);
	continue;
      }
      it = list_next (&r.waiting, it);
    }
  }
  r.running = (EventHandler*) list_pop_front (&r.ready);
  if (r.running)
    event_handler_run (r.running, train_env);
}

EventHandler*
reactor_get_running (void)
{
  return r.running;
}

