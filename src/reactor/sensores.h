#ifndef SENSORES_H
#define SENSORES_H

#include "main.h"
#include "reactor.h"
#include "observer.h"

struct sensores_eh_t {
  EventHandler eh;
  Observable observable;
};
typedef struct sensores_eh_t SensoresEH;

typedef void (*sensores_eh_func_t) (SensoresEH*);


EventHandler* sensores_eh_new (const char* dev, int prio);
void sensores_eh_add_observer (SensoresEH* this, Observer* o);
void run_sensores(struct event_handler_t*, Train_env* train_env);

#endif
