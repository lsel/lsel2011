#ifndef ESTIMACION_H
#define ESTIMACION_H

#include "observer.h"

Observer* estimacion_observer_new ();
void estimacion_notify(Observer* o, Train_env* train_env);

#endif
