#ifndef LIST_H
#define LIST_H

#define MAXELEMS 16

typedef int (*compare_func_t) (const void* data1, const void* data2);

struct list_t {
  void* data[MAXELEMS];
  int nelems;
  compare_func_t compare;
};
typedef struct list_t List;

void list_init (List* l, compare_func_t compare);
void list_append_sorted (List* l, void* data);
void list_prepend_sorted (List* l, void* data);
void list_remove (List* l, void* data);
int list_empty (List* l);
void* list_pop_front (List* l);

void** list_begin (List* l);
void** list_end (List* l);
void** list_next (List* l, void** it);

void list_remove_at (List* l, void** it);


#endif
