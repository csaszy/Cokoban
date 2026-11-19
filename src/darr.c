#include "darr.h"

#include <string.h>
#include <assert.h>
#include "debugmalloc.h"

#include "tui.h"

int size_of_type(DarrType type){
  switch(type){
    case(DARRT_INT): return sizeof(int); break;
    case(DARRT_TUIOBJECT): return sizeof(TuiObject); break;
    default: assert(0 && "Forgot to do this lol"); 
  }
}

Darr *darr_create(DarrType type){
  Darr *arr = (Darr*) malloc(sizeof(Darr)); 
  if(arr == NULL) return NULL;

  arr->type = type;
  arr->size = 0;
  arr->capacity = DARR_INITIAL_CAPACITY;
  arr->data = malloc(size_of_type(type) * DARR_INITIAL_CAPACITY);

  return arr; 
}

void darr_free(Darr *arr){
  free(arr->data);
  free(arr);
}

Darr *darr_push(Darr *arr, void *value){
  if(arr->size == arr->capacity){
    arr->capacity *= 2;
    arr->data = realloc(arr->data, size_of_type(arr->type) * arr->capacity);
    if(arr->data == NULL) assert(0 && "realloc failed somehow");
  }

  switch(arr->type){
    case(DARRT_INT): ((int*)arr->data)[arr->size] = *((int*) value); break;
    case(DARRT_TUIOBJECT): ((TuiObject*)arr->data)[arr->size] = *((TuiObject*) value); break;
    default: assert(0 && "Forgot to do this lol");
  }

  arr->size++;
  return arr;
}

void *darr_remove(Darr *arr, int index){
  if(arr == NULL) assert(0 && "darr_remove: arr nem lehet null");
  if(index >= arr->size || index < 0) assert(0 && "darr_remove: index out of range"); 

  switch(arr->type){
    case(DARRT_INT):{ 
      int *value = ((int*)arr->data) + index;
      if(index == arr->size-1){
        arr->size--;
        return value;
      }
      memmove(
        ((int*)arr->data) + index, 
        ((int*)arr->data) + index + 1, 
        (arr->size - (index + 1)) * sizeof(int)
      );
      arr->size--;
      return value;
    }
    case(DARRT_TUIOBJECT):{ 
      TuiObject *value = ((TuiObject *)arr->data) + index;
      if(index == arr->size-1){
        arr->size--;
        return value;
      }
      memcpy(
        ((TuiObject *)arr->data) + index, 
        ((TuiObject *)arr->data) + index + 1, 
        (arr->size - (index + 1)) * sizeof(int)
      );
      arr->size--;
      return value;
    }
    default: assert(0 && "Forgot to do this lol");
  }
}