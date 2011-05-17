#ifndef ESTIMACION_H
#define ESTIMACION_H

#include "observer.h"

#define DISTS0 385
#define DISTS1 235
#define DISTS2 145
#define DIST01 150
#define DIST12 90
#define DIST23 145
#define DIST30 90

struct control_tiempo {
  unsigned long t0;
  unsigned long t1;
  unsigned long t2;
  unsigned long t3;
  unsigned long estimacionT0;
  unsigned long estimacionT1;
  unsigned long estimacionT2;
};

Observer* estimacion_observer_new ();
void estimacion_notify(Observer* o, Train_env* train_env);

#endif
