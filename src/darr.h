#ifndef DARR_H
#define DARR_H

#define DARR_INITIAL_CAPACITY 8

typedef enum{
  DARRT_INT,
  DARRT_TUIOBJECT
} DarrType;

typedef struct Darr{
  DarrType type;
  int size, capacity;
  void *data;
} Darr;


int size_of_type(DarrType type);
Darr *darr_create(DarrType type);
void darr_free(Darr *arr);
Darr *darr_push(Darr *arr, void *value);
void *darr_remove(Darr *arr, int index);

#endif