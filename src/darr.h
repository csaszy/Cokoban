#ifndef DARR_H
#define DARR_H

#define DARR_INITIAL_CAPACITY 8

typedef struct Darr{
  unsigned int member_size, arr_size, capacity;
  void *data;
} Darr;


Darr *darr_create(int member_size);
void darr_free(Darr *arr);
void *darr_get(Darr *arr, int index);
int darr_size(Darr *arr);
Darr *darr_push(Darr *arr, void *value);
void darr_remove(Darr *arr, unsigned int index);

#endif