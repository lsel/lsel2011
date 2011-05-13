#ifndef REACTOR_H
#define REACTOR_H

#include <sys/time.h>
#include "memory.h"

struct event_handler_t;
typedef void (*eh_func_t) (struct event_handler_t*, Train_env* train_env);

enum event_t {
  EVENTS_READ = 1,
  EVENTS_WRITE = 2,
  EVENTS_EXCEPTION = 4,
  EVENTS_TIMEOUT = 8,
  EVENTS_MUTEX = 16
};

struct event_handler_t {
  int prio;
  struct timeval next_activation;
  eh_func_t run;

  int fd;
  int events;
};
typedef struct event_handler_t EventHandler;

void event_handler_init (EventHandler* eh, int prio, eh_func_t run);
void event_handler_run (EventHandler* eh, Train_env* train_env);

void reactor_init (void);
void reactor_add_handler (EventHandler* eh);
void reactor_handle_events (Train_env* train_env);
EventHandler* reactor_get_running (void);
void reactor_yield (void);
void reactor_wait_event (int fd, int events);
void reactor_activate (EventHandler* eh);
void reactor_delay (int ms);
void reactor_delay_until (struct timeval* tv);

int compare_prio (const void* a, const void* b);

#endif
