#include <stdio.h>
#include <string.h>
#include <vec2/vec2.h>

#include "window_constants.h"

#include "window_win.h"

#define WIN_TITLE_HEIGHT 6

static const char *WIN_TITLE[] = {
"   _____  _____ ",
"  / ____|/ ____|",
" | |  __| |  __ ",
" | | |_ | | |_ |",
" | |__| | |__| |",
"  \\_____|\\_____|"
};


void win_title_render(TuiObject *self, Screen *s){
  int width = strlen(WIN_TITLE[0]);
  int height = WIN_TITLE_HEIGHT;
  for(int x = 0; x < width; x++){
    for(int y = 0; y < height; y++){
      Cell c = (Cell){ .c = WIN_TITLE[y][x], .background_color = COL_WHITE, .foreground_color= COL_BLACK};
      tui_putc(self, c, i32vec2_create(x,y), s);
    }
  }
}

void on_event(TuiObject *self, TUI_Event event){
  if(event.type != EVENT_KEYPRESS) return;

  if(event.keypress.key == KEY_ENTER)
    tui_events_push((TUI_Event){ .level_select.type = EVENT_GOTOLEVELSELECET }, self->p_event_system);
}

void write_result_to_scoreboard(char *player_name, char *level_name, unsigned int move_count){
  char path[MAX_LEVEL_NAME_LENGTH + 40];
  sprintf(path, ".\\assets\\scoreboard\\%s.txt", level_name);

  FILE *f = fopen(path, "a");
  if(f == NULL) return;

  fprintf(f, "%d %s\n", move_count, player_name);

  fclose(f);
}

void w_win_init(TuiObject *root, EventSystem *event_system, char *player_name, char *level_name, unsigned int move_count){
  TuiObject *window_root = tui_create_tuiobj(root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, 3),
    .size = root->size 
  });
  tui_tuiobj_set_render_action(root, wc_root_render_action);
  tui_events_subscribe(window_root, on_event, event_system);

  TuiObject *title = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_CENTER,
    .offset = i32vec2_create(0, 0),
    .size = i32vec2_create(strlen(WIN_TITLE[0]), WIN_TITLE_HEIGHT)
  });
  tui_tuiobj_set_render_action(title, win_title_render);

  TuiObject *hint = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_CENTER,
    .offset = i32vec2_create(0, WIN_TITLE_HEIGHT / 2 + 2),
    .size = i32vec2_create(0, 0)
  });
  wc_set_hint_text_helper(hint, "[Enter] a folyatatashoz");
  tui_tuiobj_set_render_action(hint, wc_hint_render_action);

  write_result_to_scoreboard(player_name, level_name, move_count);
}