#include "darr.h"

#include <string.h>
#include <assert.h>
#include "debugmalloc.h"

Darr *darr_create(int member_size){
  Darr *arr = (Darr*) malloc(sizeof(Darr)); 
  assert(arr != NULL && "malloc failed\n");

  arr->member_size = member_size;
  arr->arr_size = 0;
  arr->capacity = DARR_INITIAL_CAPACITY;
  arr->data = malloc(member_size * DARR_INITIAL_CAPACITY);

  return arr; 
}

void darr_free(Darr *arr){
  free(arr->data);
  free(arr);
}

void *darr_get(Darr *arr, int index){
  return (char*) arr->data + index * arr->member_size;
}

int darr_size(Darr *arr){
  return arr->arr_size;
}

Darr *darr_push(Darr *arr, void *value){
  if(arr->arr_size == arr->capacity){
    arr->capacity *= 2;
    arr->data = realloc(arr->data, arr->member_size * arr->capacity);
    assert(arr->member_size && "realloc failed somehow");
  }

  memcpy((char*)arr->data + arr->arr_size * arr->member_size, value, arr->member_size);

  arr->arr_size++;
  return arr;
}

void darr_remove(Darr *arr, unsigned int index){
  if(arr == NULL) assert(0 && "darr_remove: arr nem lehet null\n");
  assert(index < arr->arr_size && "darr_remove: index out of range\n"); 

  if(index == arr->arr_size-1){
    arr->arr_size--;
    return;
  }

  memmove(
    (char*)arr->data + index * arr->member_size, 
    (char*)arr->data + (index + 1) * arr->member_size, 
    (arr->arr_size - (index + 1)) * arr->member_size
  );

  arr->arr_size--;
}