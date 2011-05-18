#ifndef VISUALIZACION_H
#define VISUALIZACION_H

#include "memory.h"
#include "reactor.h"
#include "observer.h"

struct visualizacion_eh_t {
  EventHandler eh;
};
typedef struct visualizacion_eh_t VisualizacionEH;

typedef void (*visualizacion_eh_func_t) (VisualizacionEH*);


EventHandler* visualizacion_eh_new (const char* dev, int prio);
void run_visual(struct event_handler_t*, Train_env* train_env);

#endif
