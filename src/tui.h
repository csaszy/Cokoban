#include <assert.h>

#include <vec2/vec2.h>
#include "darr.h"
#include "debugmalloc.h"

typedef enum {
  ALIGN_START,
  ALIGN_CENTER,
  ALIGN_END
} TuiAlignment;

typedef struct TuiObject{
  struct TuiObject *parent;
  Darr *children;
  TuiAlignment alignment;
  i32Vec2 offset;
  i32Vec2 size;
  char *content; 
} TuiObject;

TuiObject *tui_create_tuiobj(TuiObject *parent){
  TuiObject *tuiobj = (TuiObject*) malloc(sizeof(TuiObject));
  if(tuiobj == NULL) return NULL;

  tuiobj->parent = parent;
  tuiobj->children = darr_create(sizeof(TuiObject*));
  tuiobj->alignment = ALIGN_START;
  tuiobj->offset = i32vec2_create(0, 0);
  tuiobj->size = i32vec2_create(0, 0);
  tuiobj->content = NULL;

  if(parent != NULL){
    darr_push(parent->children, &tuiobj);
  }

  return tuiobj;
}

void tui_free_tuiobj(TuiObject *tuiobj){
  assert(tuiobj->children != NULL && "tuiobj missing children Darr");

  while(darr_size(tuiobj->children) > 0){
    TuiObject **target = (TuiObject **) darr_get(tuiobj->children, 0);
    tui_free_tuiobj(*target);
  }

  TuiObject *parent = tuiobj->parent; 
  if(parent != NULL){
    int index_of_pointer = darr_index_of(parent->children, &tuiobj);
    assert(index_of_pointer >= 0 && "couldn't find pointer to this node in the parent's children array");
    darr_remove(parent->children, index_of_pointer);
  }

  darr_free(tuiobj->children);
  if(tuiobj->content != NULL) free(tuiobj->content);
  free(tuiobj); 

}

void tui_debug_tree(TuiObject *root, int level){
  int indentation = 4;
  printf("% *s", level*indentation, ""); 
  printf("[%p]\n", root);

  printf("% *s|", (level+1)*indentation, ""); printf("parent:\t\t%p\n",root->parent);
  printf("% *s|", (level+1)*indentation, ""); printf("alignment:\t\t%d\n",root->alignment);
  printf("% *s|", (level+1)*indentation, ""); printf("offset:\t\tx:%d, y:%d\n",root->offset.x, root->offset.y);
  printf("% *s|", (level+1)*indentation, ""); printf("size:\t\tx:%d, y:%d\n",root->size.x, root->size.y);
  printf("% *s|", (level+1)*indentation, ""); printf("content:\t\t%s\n",root->content); 
  printf("% *s|", (level+1)*indentation, ""); printf("child_arr:\t\t%p\n",root->children);

  for(int i = 0; i < darr_size(root->children); ++i){
    TuiObject **target = (TuiObject **) darr_get(root->children, i);
    tui_debug_tree(*target, level+1);
  }
}