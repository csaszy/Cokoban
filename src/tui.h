#include "darr.h"
#include <vec2/vec2.h>
#include "debugmalloc.h"

typedef struct TuiObject{
  Darr *children;
  struct TuiObject *parent;
  void (*on_render)(struct TuiObject*);
  void (*on_keydown)(struct TuiObject*, int);
} TuiObject;

TuiObject *tui_create_tuiobj(TuiObject *parent){
  TuiObject *tuiobj = (TuiObject*) malloc(sizeof(TuiObject));
  if(tuiobj == NULL) return NULL;

  tuiobj->parent = parent;
  tuiobj->children = darr_create(DARRT_TUIOBJECT);
  tuiobj->on_render = NULL;
  tuiobj->on_keydown = NULL;

  if(parent != NULL){
    darr_push(parent->children, tuiobj);
  }

  return tuiobj;
}

void tui_free_tuiobj(TuiObject *tuiobj){
  if(tuiobj->children->size > 0) {
    darr_free(tuiobj->children);
    free(tuiobj); 
  }
  for(int i = 0; i < tuiobj->children->size; ++i){
    TuiObject *removed_chiled = (TuiObject*) darr_remove(tuiobj->children, i);
    tui_free_tuiobj(removed_chiled);
  }
}