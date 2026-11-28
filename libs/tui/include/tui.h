#ifndef TUI_H
#define TUI_H

/**
 * @defgroup tui_module tui könyvtár 
 * @brief UI objektumok, azok renderelése, és eventek.
 * @{
 */

#include <econio/econio.h>
#include <vec2/vec2.h>
#include "darr.h"
#include "events.h"


#define DEFAULT_CELL (Cell) {       \
  .background_color = COL_BLACK,    \
  .foreground_color = COL_WHITE,    \
  .c = ' '                          \
}                      

typedef enum {
  ALIGN_START,
  ALIGN_CENTER,
  ALIGN_END
} TuiAlignment;

typedef struct{
  char c;
  int foreground_color;
  int background_color;
} Cell;

typedef struct{
  i32Vec2 size;
  Cell *prev_content;
  Cell *curr_content;
} Screen;

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
  i32Vec2 _global_offset;
  void (*render_action)(struct TuiObject*, Screen *); //self pointer 
  void (*on_event)(struct TuiObject*, TUI_Event);
  EventSystem *p_event_system;
  int state_size;
  void *state;
  void (*state_free_action)(void *);
} TuiObject;

void tui_SET_ALL(TuiObject *obj, Screen *s, Cell c);

// --------- Generic ui tree stuff --------- 
/**
 * @brief 
 * Létrehoz egy TuiObject objektumot, és csatolja a megadott @p parent
 * objektumhoz ha az nem NULL.
 * 
 * @param parent pointer a szülő objektumra 
 * @param dimensions pointer a dimenziókat meghatározó objektumra 
 * @return TuiObject* 
 */
TuiObject *tui_create_tuiobj(TuiObject *parent, const TuiObjectDimensions *dimensions);
/**
 * @brief 
 * Rekurzívan felszabadítja a megadott @p tuiobj -et, annak gyermekeit, és állapotát.
 * 
 * @param tuiobj a felszabadítani kívánt UI elem
 */
void tui_free_tuiobj(TuiObject *tuiobj);
/**
 * @brief 
 * Lefoglal @p state_size bájtot, és beállítja @p obj.state pointert.
 * 
 * @param obj az objektum aminek szeretnénk állapotot adni
 * @param state_size az állapotot tároló adattípus mérete bájtban
 * @param state_free_action function ami felszabadítja azokat az esetlegesen foglalt memória
 * területeket melyekre a pointer az állapoton belül van tárolva 
 * @return void* pointer a foglalt memóriaterületre
 */
void *tui_tuiobj_add_state(TuiObject *obj, int state_size, void (*state_free_action)(void *));
/**
 * @brief  
 * Hozzácsatolja @p obj -et a @p parent -hez.
 * 
 * @param parent amihez csatolunk
 * @param obj amit csatolunk
 */
void tui_attach_tuiobj(TuiObject *parent, TuiObject *obj);
/**
 * @brief 
 * Rekurzívan felszabadítja a megadott @p obj összes gyermekét és azok 
 * függelékeit (gyermekek, állapot).
 * 
 * @param obj aminek a gyermekeit szeretnénk felszabadítani
 */
void tui_remove_and_free_all_child(TuiObject *obj);

// --------- Rendering -------- 
/**
 * @brief 
 * Létrehoz egy Screen objektumot.
 * 
 * @param size a Screen objektum mérete (magasség/szélesség)
 * @return Screen* pointer a létrejött objektumra
 */
Screen *tui_create_screen(i32Vec2 size);
/**
 * @brief 
 * Felszabadítja a megadott @p screen -t
 * 
 * @param screen felszabadítani kívánt objektum
 */
void tui_free_screen(Screen *screen);
/**
 * @brief 
 * Egyetlen karakter kiírása/rajzolása a hivatkozott @p p_screen -re
 * 
 * @param obj az a UI elem ami rajzolni szeretne
 * @param cell megjeleníteni kívánt cella
 * @param pos ahol szereténk, hogy megjelenjen a megadott cella, relatíve az 
 * @p obj bal felső sarkától  
 * @param p_screen pointer a Screen ojektumra amire rajzolni szeretnénk 
 */
void tui_putc(TuiObject *obj, Cell cell, i32Vec2 pos, Screen *p_screen);
/**
 * @brief 
 * String kiírása a hivatkozott @p p_screen -re
 * 
 * @param obj az a UI elem ami rajzolni szeretne
 * @param str nullterminált stirng amit ki szeretnénk írni 
 * @param cell a megjelenített cellák stílusa (.c field ignorált)
 * @param pos ahol szereténk, hogy kezdődjön a string kiírása
 * @param p_screen pointer a Screen ojektumra amire rajzolni szeretnénk 
 * @return int feldolgozott karakterek száma 
 */
int tui_puts(TuiObject *obj, char *str, Cell cell, i32Vec2 pos, Screen *p_screen);
/**
 * @brief 
 * A megadott @p p_screen curr_content és prev_content eltéréseinek megjelenítése a terminálon
 * 
 * @param p_screen pointer a Screen ojektumra amire rajzolni szeretnénk 
 */
void tui_draw_diff(Screen *p_screen);
/**
 * @brief 
 * Rekurzívan végigmegy a UI fán, meghívja az összes @p TuiObject.render_action callbacket,
 * és akkumulálja a globális pozíciót. Terminálra nem rajzol.
 * 
 * @param parent a renderelni kívánt fa gyökere
 * @param global_offset kezdeti abszolút pozíció (origó: terminál bal felső sarka)
 * @param p_screen pointer a Screen ojektumra amire rajzolni szeretnénk 
 */
void tui_render_accumulate(TuiObject *parent, i32Vec2 global_offset, Screen *p_screen);
/**
 * @brief 
 * Rekurzívan kirendereli a UI fát és az eredményt megjeníti a terminálon. 
 * 
 * @param root a renderelni kívánt fa gyökere
 * @param p_screen pointer a Screen ojektumra amire rajzolni szeretnénk 
 */
void tui_render(TuiObject *root, Screen *p_screen);
/**
 * @brief 
 * Beállítja a megadott @p obj -nek a render_action callback függvényét
 * 
 * @param obj aminek szeretnénk beállítani a render_action fieldjét
 * @param render_action subroutine ami renderelés alatt lesz invokálva 
 */
void tui_tuiobj_set_render_action(TuiObject *obj, void (*render_action)(TuiObject *, Screen *));

// --------- Eventek -------- 
/**
 * @brief 
 * Beállítja az adott @p obj -nek az eventek fogadására alkalmas függvényét, ezzel 
 * feliratkoztatva azt események fogadására.
 * 
 * @param obj amit szeretnénk feliratkoztatni
 * @param callback függvény pointer ami a beérkezett eventet dolgozza fel
 * @param event_system pointer a globális EventSystem objektumra  
 */
void tui_events_subscribe(TuiObject *obj, void (*callback)(TuiObject*, TUI_Event), EventSystem *event_system);
/**
 * @brief 
 * Hozzáad egy új eventet az event queuehoz.
 * 
 * @param event amit szeretnénk hozzáadni
 * @param event_system pointer a globális EventSystem objektumra  
 */
void tui_events_push(TUI_Event event, EventSystem *event_system);
/**
 * @brief 
 * Kiveszi a legrégebben belerakott objektumot az event queueból.
 * 
 * @param event_system pointer a globális EventSystem objektumra  
 */
void _tui_events_pop(EventSystem *event_system);
/**
 * @brief 
 * Rekurzívan végigsétál a UI fán, és meghívja minden eseményekre feliratkozott elem 
 * on_event metódusát, a megadott @p event parméterrel.
 * 
 * @param root gyökere a UI fának
 * @param event küldeni kívánt event
 */
void _tui_dispatch_event(TuiObject *root, TUI_Event event);
/**
 * @brief 
 * Minden eventet sorban kiküld az összes UI elemnek. 
 * 
 * @param root a UI fa gyökere 
 * @param event_system pointer a globális EventSystem objektumra  
 */
void tui_dispatch_events_in_queue(TuiObject *root, EventSystem *event_system);

// -------- Degub ------

void tui_debug_tree_print_sep(int level);
/**
 * @brief 
 * Kiírja a teljes UI fát a konzolra, debuggoláshoz használt.
 * 
 * @param root a UI fa gyökere 
 * @param level kezdeti indentációs szint, javasolt: 0 
 */
void tui_debug_tree(TuiObject *root, int level);

/** @} */
#endif