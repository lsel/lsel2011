#ifndef OBSERVER_H
#define OBSERVER_H

#include "list.h"
#include "main.h"

struct observable_t {
  List observers;
};
typedef struct observable_t Observable;

struct observer_t;
typedef void (*notify_func_t) (struct observer_t*, Train_env* train_env);

struct observer_t {
  notify_func_t notify;
};
typedef struct observer_t Observer;


void observable_init (Observable*);
void observable_notify_all (Observable*, Train_env* train_env);
void observable_add_observer (Observable*, Observer*);

void observer_init (Observer*, notify_func_t f);
void observer_notify (Observer* observer, Train_env* train_env);

#endif
