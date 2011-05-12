#ifndef VIAS_H
#define VIAS_H

#include "observer.h"

Observer* vias_observer_new ();
void vias_notify(Observer* o, Train_env* train_env);

#endif
