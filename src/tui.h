#include <assert.h>

#include <vec2/vec2.h>
#include <string.h>
#include "darr.h"
#include "debugmalloc.h"

typedef enum {
  ALIGN_START,
  ALIGN_CENTER,
  ALIGN_END
} TuiAlignment;
typedef struct TuiObjectDimensions{
  TuiAlignment horizontal_alignment;
  TuiAlignment vertical_alignment;
  i32Vec2 offset;
  i32Vec2 size;
} TuiObjectDimensions;

typedef struct TuiObject{
  struct TuiObject *parent;
  Darr *children;
  TuiAlignment horizontal_alignment;
  TuiAlignment vertical_alignment;
  i32Vec2 offset;
  i32Vec2 size;
  char *content; 
} TuiObject;

TuiObject *tui_create_tuiobj(TuiObject *parent, const TuiObjectDimensions *dimensions){
  TuiObjectDimensions defaults = {
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, 0),
    .size = i32vec2_create(0, 0)
  };
  if(dimensions == NULL) dimensions = &defaults;

  TuiObject *tuiobj = (TuiObject*) malloc(sizeof(TuiObject));
  if(tuiobj == NULL) return NULL;

  tuiobj->parent = parent;
  tuiobj->children = darr_create(sizeof(TuiObject*));
  tuiobj->horizontal_alignment = dimensions->horizontal_alignment;
  tuiobj->vertical_alignment = dimensions->vertical_alignment;
  tuiobj->offset = dimensions->offset;
  tuiobj->size = dimensions->size;
  tuiobj->content = NULL;

  if(parent != NULL){
    darr_push(parent->children, &tuiobj);
  }

  return tuiobj;
}

void tui_set_content(TuiObject *obj, char *str){
  if(obj->content != NULL) free(obj->content);

  int len = strlen(str) + 1;
  obj->content = malloc(len);
  if(obj->content == NULL) assert(0 && "malloc failed");

  memcpy(obj->content, str, len);
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
  printf("%*s", level*indentation, ""); 
  printf("[%p]\n", root);

  printf("%*s|", (level+1)*indentation, ""); printf("parent:\t\t%p\n",root->parent);
  printf("%*s|", (level+1)*indentation, ""); printf("h_alignment:\t\t%d\n",root->horizontal_alignment);
  printf("%*s|", (level+1)*indentation, ""); printf("v_alignment:\t\t%d\n",root->vertical_alignment);
  printf("%*s|", (level+1)*indentation, ""); printf("offset:\t\tx:%d, y:%d\n",root->offset.x, root->offset.y);
  printf("%*s|", (level+1)*indentation, ""); printf("size:\t\tx:%d, y:%d\n",root->size.x, root->size.y);
  printf("%*s|", (level+1)*indentation, ""); printf("content:\t\t%s\n",root->content); 
  printf("%*s|", (level+1)*indentation, ""); printf("child_arr:\t\t%p\n",root->children);

  for(int i = 0; i < darr_size(root->children); ++i){
    TuiObject **target = (TuiObject **) darr_get(root->children, i);
    tui_debug_tree(*target, level+1);
  }
}

void tui_draw_content(TuiObject *obj, i32Vec2 pos){
  econio_rawmode();
  econio_gotoxy(pos.x, pos.y);
  if(obj->content == NULL) return;

  for(int i = 0; obj->content[i] != '\0'; ++i){
    printf("%c", obj->content[i]);
  }
  econio_normalmode();
}

void tui_render_accumulate(TuiObject *obj, i32Vec2 global_offset){
  tui_draw_content(obj, global_offset);

  for(int i = 0; i < darr_size(obj->children); ++i){
    TuiObject **target = (TuiObject **) darr_get(obj->children, i);

    //global_offset X
    switch((*target)->horizontal_alignment){
      case(ALIGN_START):
        global_offset.x = global_offset.x + (*target)->offset.x;
        break;
      case(ALIGN_END):
        global_offset.x = global_offset.x + obj->size.x - (*target)->size.x + (*target)->offset.x;
        break;
      case(ALIGN_CENTER):
        global_offset.x = global_offset.x + (int)(obj->size.x / 2) + 1 - (int)((*target)->size.x / 2); 
        break;
    }

    //global_offset Y
    switch((*target)->vertical_alignment){
      case(ALIGN_START):
        global_offset.y = global_offset.y + (*target)->offset.y;
        break;
      case(ALIGN_END):
        global_offset.y = global_offset.y + obj->size.y - (*target)->size.y + (*target)->offset.y;
        break;
      case(ALIGN_CENTER):
        global_offset.y = global_offset.y + (int)(obj->size.y / 2) + 1 - (int)((*target)->size.y / 2); 
        break;
    }

    tui_render_accumulate(*target, global_offset);
  }
}

void tui_render(TuiObject *root){
  econio_clrscr(); 
  for(int i = 0; i < root->size.y; ++i){
    for(int j = 0; j < root->size.x; ++j){
      econio_gotoxy(j,i);
      printf(" ");
    }
  }
  econio_clrscr(); 
  tui_render_accumulate(root, (i32Vec2){.x = 0, .y = 0});
  econio_gotoxy(root->size.x,root->size.y);
}