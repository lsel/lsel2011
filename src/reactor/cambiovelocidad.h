#ifndef CAMBIOVELOCIDAD_H
#define CAMBIOVELOCIDAD_H

#include "observer.h"

Observer* cambiovelocidad_observer_new ();
void cambiovelocidad_notify(Observer* o, Train_env* train_env);

#endif
