#include "tui.h"

void tui_renderaction_DEFAULT_SETTINGS(){
  econio_textbackground(COL_BLACK);
  econio_textcolor(COL_WHITE);
}

void tui_renderaction_setbgtowhite(){
  econio_textbackground(COL_WHITE);
  econio_textcolor(COL_BLACK);
}


TuiObject *tui_create_tuiobj(TuiObject *parent, const TuiObjectDimensions *dimensions, Screen *p_screen){
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
  tuiobj->state = NULL;
  tuiobj->state_size = 0;
  tuiobj->render = NULL;
  if(tuiobj->parent == NULL) tuiobj->p_screen = p_screen;

  tui_attach_tuiobj(parent, tuiobj);

  return tuiobj;
}



void tui_tuiobj_set_render(TuiObject *obj, void (*render)(TuiObject *, i32Vec2)){
  obj->render = render;
}

void *tui_tuiobj_add_state(TuiObject *obj, int state_size){
  void * p_state = malloc(state_size);
  if(p_state == NULL) assert(0 && "malloc failed");

  obj->state = p_state;
  obj->state_size = state_size;
}

void tui_set_screen_on_all_nodes(TuiObject *obj){
  assert(obj->p_screen != NULL && "parent TuiObject did not have a screen");
  obj->p_screen = obj->parent->p_screen;

  for(int i = 0; i < darr_size(obj->children); ++i){
    TuiObject **child = (TuiObject **) darr_get(obj->children, i);
    tui_set_screen_on_all_nodes(*child);
  }
}

void tui_attach_tuiobj(TuiObject *parent, TuiObject *obj){
  if(parent != NULL){
    darr_push(parent->children, &obj);

    tui_set_screen_on_all_children(obj);
  }
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
  if(tuiobj->state != NULL) free(tuiobj->state);

  free(tuiobj); 
}


tui_putc(TuiObject *obj, Cell cell){

}

void tui_render_accumulate(TuiObject *parent, i32Vec2 global_offset){
  parent->_global_offset = global_offset;
  parent->render(parent);

  for(int i = 0; i < darr_size(parent->children); ++i){
    TuiObject **child = (TuiObject **) darr_get(parent->children, i);
    i32Vec2 updated_global_offset = global_offset; 

    //global_offset X
    switch((*child)->horizontal_alignment){
      case(ALIGN_START):
        updated_global_offset.x = global_offset.x + (*child)->offset.x;
        break;
      case(ALIGN_END):
        updated_global_offset.x = global_offset.x + parent->size.x - (*child)->size.x + (*child)->offset.x;
        break;
      case(ALIGN_CENTER):
        updated_global_offset.x = global_offset.x + (int)(parent->size.x / 2) - (int)((*child)->size.x / 2) + (*child)->offset.x; 
        break;
    }

    //global_offset Y
    switch((*child)->vertical_alignment){
      case(ALIGN_START):
        updated_global_offset.y = global_offset.y + (*child)->offset.y;
        break;
      case(ALIGN_END):
        updated_global_offset.y = global_offset.y + parent->size.y - (*child)->size.y + (*child)->offset.y;
        break;
      case(ALIGN_CENTER):
        updated_global_offset.y = global_offset.y + (int)(parent->size.y / 2) - (int)((*child)->size.y / 2) + (*child)->offset.y; 
        break;
    }

    tui_render_accumulate(*child, updated_global_offset);
  }
}

void tui_render(TuiObject *root){
  econio_clrscr(); 
  tui_renderaction_DEFAULT_SETTINGS();
  tui_render_accumulate(root, (i32Vec2){.x = 0, .y = 0});
  econio_gotoxy(root->size.x,root->size.y);
}

//debug stuff
void tui_debug_tree_print_sep(int level){
  int indentation = 4;
  for(int i = 0; i < level; ++i){
    printf("|%*s", indentation, ""); 
  }
}
void tui_debug_tree(TuiObject *root, int level){
  tui_debug_tree_print_sep(level);
  printf("+[%p]\n", root);

  tui_debug_tree_print_sep(level+1); printf("parent:\t\t%p\n",root->parent);
  tui_debug_tree_print_sep(level+1); printf("h_alignment:\t\t%d\n",root->horizontal_alignment);
  tui_debug_tree_print_sep(level+1); printf("v_alignment:\t\t%d\n",root->vertical_alignment);
  tui_debug_tree_print_sep(level+1); printf("offset:\t\tx:%d, y:%d\n",root->offset.x, root->offset.y);
  tui_debug_tree_print_sep(level+1); printf("size:\t\tx:%d, y:%d\n",root->size.x, root->size.y);
  tui_debug_tree_print_sep(level+1); printf("content:\t\t%s\n",root->content); 
  tui_debug_tree_print_sep(level+1); printf("child_arr:\t\t%p\n",root->children);

  for(int i = 0; i < darr_size(root->children); ++i){
    TuiObject **target = (TuiObject **) darr_get(root->children, i);
    tui_debug_tree(*target, level+1);
  }
}