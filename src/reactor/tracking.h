#ifndef TRACKING_H
#define TRACKING_H

#include "memory.h"
#include "reactor.h"
#include "observer.h"
//Periodo de tracking
#define PERIODO 0
#define UPERIODO 300000

struct tracking_eh_t {
  EventHandler eh;
  Observable observable;
};
typedef struct tracking_eh_t TrackingEH;

typedef void (*tracking_eh_func_t) (TrackingEH*);


EventHandler* tracking_eh_new (const char* dev, int prio);
void tracking_eh_add_observer (TrackingEH* this, Observer* o);
void run_tracking(struct event_handler_t*, Train_env* train_env);

#endif
