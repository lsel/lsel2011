#include "list.h"
#include <string.h>
#include <stdlib.h>

void list_init (List* l, compare_func_t compare)
{
  l->nelems = 0;
  l->compare = compare;
}

void
list_append_sorted (List* l, void* data)
{
  l->data[l->nelems++] = data;
  qsort (l->data, l->nelems, sizeof (void*), l->compare);
}

void
list_prepend_sorted (List* l, void* data)
{
  memcpy (l->data + 1, l->data, sizeof(void*) * l->nelems);
  l->data[0] = data;
  ++ l->nelems;
  qsort (l->data, l->nelems, sizeof (void*), l->compare);
}

int
list_empty (List* l)
{
  return l->nelems == 0;
}

void
list_remove (List* l, void* data)
{
  int i;
  for (i = 0; i < l->nelems; ++i) {
    if (l->data[i] == data)
      break;
  }
  -- l->nelems;
  if (i >= l->nelems)
    return;
  memcpy (l->data + i, l->data + i + 1, sizeof(void*) * (l->nelems - i));
}

void*
list_pop_front (List* l)
{
  void * data;
  if (! l->nelems)
    return NULL;
  data = l->data[0];
  -- l->nelems;
  memcpy (l->data, l->data + 1, sizeof(void*) * l->nelems);
  return data;
}


void**
list_begin (List* l)
{
  return l->data;
}

void**
list_end (List* l)
{
  return l->data + l->nelems;
}

void**
list_next (List* l, void** it)
{
  return it + 1;
}
