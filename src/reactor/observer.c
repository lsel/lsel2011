#include "observer.h"

int pointer_compare (const void* a, const void* b) 
{ 
  return (a < b)? -1 : (a > b)? 1 : 0; 
}

void observable_init (Observable* this)
{
  list_init (&this->observers, pointer_compare);
}

void observable_notify_all (Observable* this, Train_env* train_env)
{
  List* l = &this->observers;
  void** it;
  for (it = list_begin (l);
       it != list_end (l);
       it = list_next (l, it)) {
    Observer* obs = (Observer*) *it;
    observer_notify (obs, train_env);
  }  
}

void observable_add_observer (Observable* this, Observer* observer)
{
  list_append_sorted (&this->observers, observer);
}


void observer_init (Observer* this, notify_func_t f)
{
  this->notify = f;
}

void observer_notify (Observer* observer, Train_env* train_env)
{
  observer->notify(observer, train_env);
}
