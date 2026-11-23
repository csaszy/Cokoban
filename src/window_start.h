#include <vec2/vec2.h>
#include <string.h>
#include "tui.h"
#include "events.h"
#include "constants.h"
#include "window_constants.h"

static const char *TITLE[] = {
" _____ _____ _   _____________  ___   _   _ ",
"/  __ \\  _  | | / /  _  | ___ \\/ _ \\ | \\ | |",
"| /  \\/ | | | |/ /| | | | |_/ / /_\\ \\|  \\| |",
"| |   | | | |    \\| | | | ___ \\  _  || . ` |",
"| \\__/\\ \\_/ / |\\  \\ \\_/ / |_/ / | | || |\\  |",
" \\____/\\___/\\_| \\_/\\___/\\____/\\_| |_/\\_| \\_/",
};

void title_render(TuiObject *self, Screen *s){
  int width = strlen(TITLE[0]);
  int height = 6;
  for(int x = 0; x < width; x++){
    for(int y = 0; y < height; y++){
      Cell c = (Cell){ .c = TITLE[y][x], .background_color = COL_WHITE, .foreground_color= COL_BLACK};
      tui_putc(self, c, i32vec2_create(x,y), s);
    }
  }
}

typedef struct {
  char str[MAX_PLAYER_NAME_LENGTH+1];
} NamefieldData;

void name_field_render(TuiObject *self, Screen *s){
  tui_SET_ALL(self, s, DEFAULT_CELL);
  NamefieldData *state = (NamefieldData *)self->state;
  int len = strlen(state->str);
  for(int i = 0; i < len; ++i){
    tui_putc(self, (Cell){
      .c = state->str[i],
      .background_color = COL_WHITE,
      .foreground_color = COL_BLACK
    }, i32vec2_create(i, 0), s);
  }
}

void name_field_on_event(TuiObject *self, TUI_Event event){
  if(event.type != EVENT_KEYPRESS) return;

  NamefieldData *state = (NamefieldData *)self->state;
  int len = strlen(state->str); 

  switch (event.keypress.key) {
    case KEY_ENTER:
      if(len == 0) break;
      tui_events_push((TUI_Event){
        .level_select.type = EVENT_GOTOLEVELSELECET,
        .level_select.data = state->str
      }, self->p_event_system);
      break;
    case KEY_BACKSPACE:
      if(len != 0){
        state->str[len-1] = '\0';
      }
      break;
    default:
      if(len < MAX_PLAYER_NAME_LENGTH){
        state->str[len] = event.keypress.key;
      }
      break;
  }
}

void hint_render(TuiObject *self, Screen *s){
  const char *str = "(Nyomj [Enter]-t a kezdeshez!)";
  int c = 0;
  for(int i = 0; i < self->size.x; ++i){
    tui_putc(self, (Cell) {
      .c = str[c] != '\0' ? str[c] : ' ',
      .background_color = COL_DARKGRAY,
      .foreground_color = COL_BLACK
    }, i32vec2_create(i, 0), s);
    if(str[c] != '\0') c++;
  }
}

void w_start_init(TuiObject *root, EventSystem *event_system){
  TuiObject *window_root = tui_create_tuiobj(root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, 3),
    .size = root->size 
  });
  tui_tuiobj_set_render_action(root, wc_root_render_action);

  TuiObject *title = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, 3),
    .size = i32vec2_create(46, 6)
  });
  tui_tuiobj_set_render_action(title, title_render);

  TuiObject *name_field = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_CENTER,
    .offset = i32vec2_create(0, 0),
    .size = i32vec2_create(MAX_PLAYER_NAME_LENGTH, 1)
  });
  tui_tuiobj_set_render_action(name_field, name_field_render);
  tui_tuiobj_add_state(name_field, sizeof(NamefieldData), NULL);
  tui_events_subscribe(name_field, name_field_on_event, event_system);

  TuiObject *hint = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_CENTER,
    .offset = i32vec2_create(0, 2),
    .size = i32vec2_create(30, 1)
  });
  tui_tuiobj_set_render_action(hint, hint_render);
}