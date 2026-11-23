#include "tui.h"

void tui_SET_ALL(TuiObject *obj, Screen *s, Cell c){
  for(int x = 0; x < obj->size.x; x++){
    for(int y = 0; y < obj->size.y; y++){
      tui_putc(obj, c, i32vec2_create(x,y), s);
    }
  }
}

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

  tui_attach_tuiobj(parent, tuiobj);
  tuiobj->children = darr_create(sizeof(TuiObject*));
  tuiobj->horizontal_alignment = dimensions->horizontal_alignment;
  tuiobj->vertical_alignment = dimensions->vertical_alignment;
  tuiobj->offset = dimensions->offset;
  tuiobj->size = dimensions->size;
  tuiobj->_global_offset = i32vec2_create(0, 0);
  tuiobj->render_action = NULL;
  tuiobj->on_event = NULL;
  tuiobj->p_event_system = NULL;
  tuiobj->state_size = 0;
  tuiobj->state = NULL;
  tuiobj->state_free_action = NULL;

  return tuiobj;
}

void tui_tuiobj_set_render_action(TuiObject *obj, void (*render_action)(TuiObject *, Screen *)){
  obj->render_action = render_action;
}

void *tui_tuiobj_add_state(TuiObject *obj, int state_size, void (*state_free_action)(void *)){
  if(obj->state == NULL) free(obj->state);

  void *p_state = calloc(1,state_size);
  if(p_state == NULL) assert(0 && "malloc failed");

  obj->state = p_state;
  obj->state_size = state_size;
  obj->state_free_action = state_free_action; 
  return p_state;
}

void tui_attach_tuiobj(TuiObject *parent, TuiObject *obj){
  obj->parent = parent;
  if(parent != NULL){
    darr_push(parent->children, &obj);
  }
}

void tui_remove_and_free_all_child(TuiObject *obj){
  for(int i = 0; i < darr_size(obj->children); ++i){
    TuiObject **child = (TuiObject **)darr_get(obj->children, i);
    tui_free_tuiobj(*child);
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

  if(tuiobj->state != NULL){
    if(tuiobj->state_free_action != NULL) tuiobj->state_free_action(tuiobj->state);
    free(tuiobj->state);
  } 

  free(tuiobj); 
}


void tui_putc(TuiObject *obj, Cell cell, i32Vec2 pos, Screen *p_screen){
  if(obj == NULL) return;

  if(
    pos.x < 0 || pos.x >= obj->size.x || 
    pos.y < 0 || pos.y >= obj->size.y
  ) return; 

  i32Vec2 global_position = {
    .x = obj->_global_offset.x + pos.x,
    .y = obj->_global_offset.y + pos.y
  };

  if(obj->parent != NULL){
    TuiObject parent = *(obj->parent);
    if(
      global_position.x < parent._global_offset.x || global_position.x >= parent._global_offset.x + parent.size.x ||
      global_position.y < parent._global_offset.y || global_position.y >= parent._global_offset.y + parent.size.y
    ) return;
  }

  if(
    global_position.x < 0 || global_position.x >= p_screen->size.x ||
    global_position.y < 0 || global_position.y >= p_screen->size.y
  ) return;

  p_screen->curr_content[global_position.y * p_screen->size.x + global_position.x] = cell;
}

bool eq_cell(Cell *c1, Cell *c2){
  return (
    c1->c == c2->c &&
    c1->background_color == c2->background_color &&
    c1->foreground_color == c2->foreground_color
  ); 
}

void tui_draw_diff(Screen *p_screen){
  int width = p_screen->size.x;
  int height = p_screen->size.y;
  
  for(int y = 0; y < height; ++y){
    for(int x = 0; x < width; ++x){
      int offset = y * width + x;
      if(!eq_cell(p_screen->curr_content + offset, p_screen->prev_content + offset)){
        Cell cell_in_current = p_screen->curr_content[offset];

        econio_gotoxy(x, y);
        econio_textbackground(cell_in_current.background_color);
        econio_textcolor(cell_in_current.foreground_color);

        putchar(cell_in_current.c); 
      }

      p_screen->prev_content[offset] = p_screen->curr_content[offset];
    }
  }
}

void tui_render_accumulate(TuiObject *parent, i32Vec2 global_offset, Screen *p_screen){
  parent->_global_offset = global_offset;
  if(parent->render_action != NULL)
    parent->render_action(parent, p_screen);

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

    tui_render_accumulate(*child, updated_global_offset, p_screen);
  }
}

void tui_render(TuiObject *root, Screen *p_screen){
  tui_render_accumulate(root, (i32Vec2){.x = 0, .y = 0}, p_screen);
  tui_draw_diff(p_screen);

  econio_gotoxy(p_screen->size.x, p_screen->size.y);
}

Screen *tui_create_screen(i32Vec2 size){
  int area = size.x * size.y;

  Cell defaultCell = DEFAULT_CELL;

  Screen *screen = malloc(sizeof(Screen));
  assert(screen != NULL && "malloc failed"); 

  screen->size = size;

  screen->curr_content = (Cell *) malloc(sizeof(Cell) * area);
  assert(screen->curr_content != NULL && "malloc failed"); 
  for(int offset = 0; offset < area; offset++){
    screen->curr_content[offset] = defaultCell;
  }

  screen->prev_content = (Cell *) malloc(sizeof(Cell) * area);
  assert(screen->prev_content != NULL && "malloc failed"); 
  for(int offset = 0; offset < area; offset++){
    screen->prev_content[offset] = defaultCell;
    screen->prev_content[offset].background_color--; //forcing initial render
  }

  return screen;
}

void tui_free_screen(Screen *screen){
  if(screen == NULL) return;
  free(screen->curr_content);
  free(screen->prev_content);
  free(screen);
}

//event stuff
void tui_events_subscribe(TuiObject *obj, void (*callback)(TuiObject*, TUI_Event), EventSystem *event_system){
  if(obj == NULL) return;
  if(obj->p_event_system == NULL) obj->p_event_system = event_system;
  obj->on_event = callback;
}

void tui_events_push(TUI_Event event, EventSystem *event_system){
  if(event_system == NULL) return;
  darr_push(event_system->event_queue, &event);
}

void _tui_events_pop(EventSystem *event_system){
  if(event_system == NULL) return;
  darr_remove(event_system->event_queue, 0);
}

void _tui_dispatch_event(TuiObject *root, TUI_Event event){
  //idk hasrautes szeruen elsoszor a gyerek nodeokon dispatchelunk aztan majd lesz vmi
  if(root == NULL) return;
  for(int i = 0; i < darr_size(root->children); i++){
    TuiObject **child = (TuiObject **) darr_get(root->children, i);
    _tui_dispatch_event(*child, event);
  }

  if(root->on_event != NULL){
    root->on_event(root, event);
  }
}

void tui_dispatch_events_in_queue(TuiObject *root, EventSystem *event_system){
  if(event_system == NULL) return;
  while (darr_size(event_system->event_queue) > 0) {
    TUI_Event *current_event = (TUI_Event*)darr_get(event_system->event_queue, darr_size(event_system->event_queue) - 1);
    _tui_dispatch_event(root, *current_event);
    _tui_events_pop(event_system);
  }
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
  tui_debug_tree_print_sep(level+1); printf("g_offset:\t\tx:%d, y:%d\n",root->_global_offset.x, root->_global_offset.y);
  tui_debug_tree_print_sep(level+1); printf("size:\t\tx:%d, y:%d\n",root->size.x, root->size.y);
  tui_debug_tree_print_sep(level+1); printf("child_arr:\t\t%p\n",root->children);

  for(int i = 0; i < darr_size(root->children); ++i){
    TuiObject **target = (TuiObject **) darr_get(root->children, i);
    tui_debug_tree(*target, level+1);
  }
}