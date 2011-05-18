#ifndef VIAS_H
#define VIAS_H

#include "observer.h"
#define DELAY2S 2000

Observer* vias_observer_new ();
void vias_notify(Observer* o, Train_env* train_env);

#endif
