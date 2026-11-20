#include "tui.h"

void tui_renderaction_DEFAULT_SETTINGS(){
  econio_textbackground(COL_BLACK);
  econio_textcolor(COL_WHITE);
}

void tui_renderaction_setbgtowhite(){
  econio_textbackground(COL_WHITE);
  econio_textcolor(COL_BLACK);
}

TuiObject *tui_create_tuiobj(TuiObject *parent, const TuiObjectDimensions *dimensions, void (*render)(void)){
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
  tuiobj->render = render;
  tuiobj->on_key_down = NULL;
  tuiobj->has_changed = true;

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

void tui_draw_content(TuiObject *obj, i32Vec2 pos){
  econio_rawmode();
  econio_gotoxy(pos.x, pos.y);

  int char_index = 0;
  for(int i = pos.y; i < pos.y + obj->size.y; ++i){
    for(int j = pos.x; j < pos.x + obj->size.x; ++j){
      econio_gotoxy(j,i);
      if(obj->content == NULL) 
        printf(" ");
      else
        printf("%c", obj->content[char_index] == '\0' ? ' ' : obj->content[char_index++]);
    }
  }

  econio_normalmode();
}

void tui_render_accumulate(TuiObject *parent, i32Vec2 global_offset){
  tui_renderaction_DEFAULT_SETTINGS();

  if(parent->has_changed){
    if(parent->render != NULL) parent->render();
    tui_draw_content(parent, global_offset);
    parent->has_changed = false;
  }

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
        updated_global_offset.x = global_offset.x + (int)(parent->size.x / 2) + 1 - (int)((*child)->size.x / 2) + (*child)->offset.x; 
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
        updated_global_offset.y = global_offset.y + (int)(parent->size.y / 2) + 1 - (int)((*child)->size.y / 2) + (*child)->offset.y; 
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
