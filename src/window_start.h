#include "tui.h"
#include <vec2/vec2.h>
#include <string.h>

#define WSTART_TITLE_OFFSET (i32Vec2){ .x = 0, .y = 3}
#define WSTART_TITLE_SIZE (i32Vec2){ .x = 40, .y = 10}

#define WSTART_NAMEFIELD_OFFSET (i32Vec2){ .x = 0, .y = 0}
#define W1_NAMEFIELD_SIZE (i32Vec2){ .x = 15, .y = 1}

#define W1_HINT_OFFSET (i32Vec2){ .x = 0, .y = 2}
#define W1_HINT_SIZE (i32Vec2){ .x = 15, .y = 1}

static const char *TITLE[] = {
" _____ _____ _   _____________  ___   _   _ ",
"/  __ \\  _  | | / /  _  | ___ \\/ _ \\ | \\ | |",
"| /  \\/ | | | |/ /| | | | |_/ / /_\\ \\|  \\| |",
"| |   | | | |    \\| | | | ___ \\  _  || . ` |",
"| \\__/\\ \\_/ / |\\  \\ \\_/ / |_/ / | | || |\\  |",
" \\____/\\___/\\_| \\_/\\___/\\____/\\_| |_/\\_| \\_/",
};

void root_render(TuiObject *self, Screen *s){
  for(int x = 0; x < self->size.x; x++){
    for(int y = 0; y < self->size.y; y++){
      tui_putc(self, (Cell) {
        .c = ' ',
        .background_color = COL_DARKGRAY,
        .foreground_color = COL_BLACK
      }, i32vec2_create(x, y), s);
    }
  }
}

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

void hint_render(TuiObject *self, Screen *s){
  const char *str = "Nyomj [Enter]-t a kezdeshez!";
  int c = 0;
  for(int i = 0; i < self->size.x; ++i){
    tui_putc(self, (Cell) {
      .c = str[c] != '\0' ? str[c] : ' ',
      .background_color = COL_WHITE,
      .foreground_color = COL_BLACK
    }, i32vec2_create(i, 0), s);
    if(str[c] != '\0') c++;
  }
}

void w_start_init(TuiObject *root){
  TuiObject *window_root = tui_create_tuiobj(root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_START,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, 3),
    .size = root->size 
  });
  tui_tuiobj_set_render(root, root_render);

  TuiObject *title = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_START,
    .offset = i32vec2_create(0, 3),
    .size = i32vec2_create(46, 6)
  });
  tui_tuiobj_set_render(title, title_render);

  TuiObject *name_field = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_CENTER,
    .offset = i32vec2_create(0, 0),
    .size = i32vec2_create(15, 1)
  });

  TuiObject *hint = tui_create_tuiobj(window_root, &(TuiObjectDimensions){
    .horizontal_alignment = ALIGN_CENTER,
    .vertical_alignment = ALIGN_CENTER,
    .offset = i32vec2_create(0, 2),
    .size = i32vec2_create(20, 1)
  });
  tui_tuiobj_set_render(hint, hint_render);
}