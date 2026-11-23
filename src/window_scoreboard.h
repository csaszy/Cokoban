#include "tui.h"
#include <vec2/vec2.h>
#include <string.h>

#include "events.h"
#include "constants.h"
#include "window_constants.h"

#define MAX_SCORE_NUMBER_LENGTH 20
#define MAX_SCORE_LENGTH MAX_PLAYER_NAME_LENGTH + MAX_SCORE_NUMBER_LENGTH

typedef struct{
  char name[MAX_PLAYER_NAME_LENGTH];
  int number_of_moves;
} Score;
typedef struct 
{
  int scroll_offset;
  Darr *score_list;
} Scoreboard_RootState;
void scoreboard_root_event(TuiObject *self, TUI_Event event){
  if(event.type != EVENT_KEYPRESS) return; 

  switch (event.keypress.key) {
    case KEY_ESCAPE:
      tui_events_push((TUI_Event){
        .level_select.type = EVENT_GOTOLEVELSELECET
      }, self->p_event_system);
      break;
    default:
      break;
  }
}
void free_window_root_state(void *state){
  Scoreboard_RootState *window_root_state = (Scoreboard_RootState *)state;
  darr_free(window_root_state->score_list);
}

void load_scores(Darr *score_list, char *level_name){
  
  char path[MAX_LEVEL_NAME_LENGTH + 40];
  sprintf(path, ".\\assets\\scoreboard\\%s.txt", level_name);

  FILE *f = fopen(path, "r");
  if(f == NULL) return;

  char buff[MAX_SCORE_LENGTH];
  while (fgets(buff, MAX_SCORE_LENGTH, f) != NULL){
    buff[strcspn(buff, "\r\n")] = '\0';
    char *p_div = strchr(buff, ' ');
    *(p_div++) = '\0'; 
    Score score = (Score){
      .number_of_moves = atoi(buff),
    };
    strcpy(score.name, p_div);
    darr_push(score_list, &score);
  }

  fclose(f);
}

typedef struct {
  Scoreboard_RootState *window_state;
} Scoreboard_ScoreListState;

void scoreboard_score_list_render_acttion(TuiObject *self, Screen *s){
  Scoreboard_ScoreListState *self_state = (Scoreboard_ScoreListState *)self->state;
  Scoreboard_RootState *window_state = (Scoreboard_RootState *)self_state->window_state;

  for(int i = 0; i < darr_size(window_state->score_list); ++i){
    Score *curr_score = (Score *)darr_get(window_state->score_list, i);

    int offset = 0;
    //player_name
    offset = tui_puts(
      self, 
      curr_score->name, 
      (Cell){
        .foreground_color = COL_WHITE,
        .background_color = COL_BLACK
      },
      i32vec2_create(0, i),
      s
    );

    //": "
    offset += tui_puts(
      self, 
      ": ", 
      (Cell){
        .foreground_color = COL_WHITE,
        .background_color = COL_BLACK
      },
      i32vec2_create(offset, i),
      s
    );

    //move count 
    char number_str[MAX_SCORE_NUMBER_LENGTH + 1];
    itoa(curr_score->number_of_moves, number_str, 10);
    offset += tui_puts(
      self, 
      number_str, 
      (Cell){
        .foreground_color = COL_WHITE,
        .background_color = COL_BLACK
      },
      i32vec2_create(offset, i),
      s
    );

    //": "
    offset += tui_puts(
      self, 
      " lepes", 
      (Cell){
        .foreground_color = COL_WHITE,
        .background_color = COL_BLACK
      },
      i32vec2_create(offset, i),
      s
    );
  }
} 

void w_scoreboard_init(TuiObject *root, char *level_name, EventSystem *event_system){
  TuiObject *window_root = tui_create_tuiobj(root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, 0),
    .size = root->size 
  });
  tui_tuiobj_set_render_action(window_root, wc_root_render_action);
  Scoreboard_RootState *window_root_state = (Scoreboard_RootState *) tui_tuiobj_add_state(
    window_root, 
    sizeof(Scoreboard_RootState), 
    free_window_root_state
  );
  window_root_state->scroll_offset = 0;
  window_root_state->score_list = darr_create(sizeof(Score));
  load_scores(window_root_state->score_list, level_name);
  tui_events_subscribe(window_root, scoreboard_root_event, event_system);

  //dicsoseglista
  TuiObject *score_list = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, 3),
    .size = i32vec2_create(MAX_SCORE_LENGTH, window_root->size.y - 4) 
  });
  tui_tuiobj_set_render_action(score_list, scoreboard_score_list_render_acttion);
  Scoreboard_ScoreListState *sls = (Scoreboard_ScoreListState *)tui_tuiobj_add_state(
    score_list, 
    sizeof(Scoreboard_ScoreListState), 
    NULL
  );
  sls->window_state = window_root_state;

  //hints
  TuiObject *hint_title = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, HINT_PADDING_Y),
    .size = i32vec2_create(0, 0) 
  });
  wc_set_hint_text_helper(hint_title, "Dicsoseglista");
  tui_tuiobj_set_render_action(hint_title, wc_hint_render_action);

  TuiObject *hint_back = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_END,
    .offset = i32vec2_create(HINT_PADDING_X, -HINT_PADDING_Y),
    .size = i32vec2_create(0, 0) 
  });
  wc_set_hint_text_helper(hint_back, "[ESC] Vissaz");
  tui_tuiobj_set_render_action(hint_back, wc_hint_render_action);

  TuiObject *hint_quit = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_END,
    .vertical_alignment = ALIGN_END,
    .offset = i32vec2_create(-HINT_PADDING_X, -HINT_PADDING_Y),
    .size = i32vec2_create(0, 0) 
  });
  wc_set_hint_text_helper(hint_quit, "[Q] Kilepes");
  tui_tuiobj_set_render_action(hint_quit, wc_hint_render_action);

}