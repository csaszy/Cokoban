#include <econio/econio.h>
#include <vec2/vec2.h>
#include <stdbool.h>
#include <locale.h>

#include "debugmalloc.h"

#include "game.h"
#include "tui.h"
#include "darr.h"

#define DIR_COUNT 4
typedef struct MenuItem{
    i32Vec2 pos;
    char *text;
    struct MenuItem *next;
    struct MenuItem *nav_arr[DIR_COUNT];
    void (*onclick) (struct MenuItem *);
} MenuItem;
typedef struct Menu{
    MenuItem *head_menuitem;
    MenuItem *selected;
} Menu;

Menu *create_menu(){
    Menu *mp = (Menu*) malloc(sizeof(Menu)); 
    mp->head_menuitem = NULL;
    mp->selected = NULL;
    return mp;
}

void free_menu(Menu *menu){
    MenuItem *it = menu->head_menuitem, *next;
    while(it != NULL){
        next = it->next;
        free(it);
        it = next;
    }
    free(menu);
}

MenuItem *create_menu_item(i32Vec2 position, char *text){
    MenuItem* mi = (MenuItem*) malloc(sizeof(MenuItem));
    if(mi == NULL) return NULL;
    mi->pos = position; 
    mi->text = text;
    mi->next = NULL;
    mi->onclick = NULL;
    for(int i = 0; i < DIR_COUNT; ++i){
        mi->nav_arr[i] = NULL;
    }
    return mi;
}

MenuItem *add_menu_item(Menu *menu, MenuItem *menu_item){
    if(menu_item == NULL) return NULL;
    if(menu->head_menuitem == NULL) {
        menu->head_menuitem = menu_item;
        menu->selected = menu_item;
        return menu_item;
    }

    MenuItem *it = menu->head_menuitem;

    for(; it->next != NULL; it = it->next){};
    it->next = menu_item;

    return menu_item;
}

void render_menu(Menu *menu){
    econio_clrscr();
    econio_rawmode();
    for(MenuItem *it = menu->head_menuitem; it != NULL; it = it->next){
        econio_gotoxy(it->pos.x, it->pos.y);
        if(menu->selected == it){
            econio_textbackground(COL_WHITE);
            econio_textcolor(COL_BLACK);
        }else{
            econio_textbackground(COL_BLACK);
            econio_textcolor(COL_WHITE);
        }
        printf("%s", it->text);
        econio_textbackground(COL_BLACK);
        econio_textcolor(COL_WHITE);
    }
    econio_normalmode();
}

void connect_menu_items(MenuItem *from, MenuItem *to, Direction dir, bool both){
    from->nav_arr[dir & 3] = to;                    //example DIR_DOWN = 0b11, ~DIR_DOWN = 0b11..100
    if(both) to->nav_arr[~dir & 3] = from;          // "& 3" -> 0b00...011 bitmaszk => ~DIR_DOWN = DIR_UP 
}

int menu_move_selection(Menu *menu, Direction dir){
    if(menu->selected->nav_arr[dir] == NULL) return 0;
    menu->selected = menu->selected->nav_arr[dir];
    return 1;
}

void menu_item_set_onclick(MenuItem *menuItem, void (*onclick)(MenuItem*)){
    menuItem->onclick = onclick;
}

void menu_dispatch_onclick_events(Menu *menu){
    MenuItem *menu_item = menu->head_menuitem;
    while(menu_item != NULL){
        if(menu_item->onclick != NULL)
            menu_item->onclick(menu_item);
        menu_item = menu_item->next;
    }
}

void print_stuff(MenuItem *src){
    char result[100] = "";
    printf("lolsw");
    sprintf(&result[0], "Pressing: %s", src->text);
}

Menu *setup(){
    Menu *menu = create_menu(); 

    MenuItem *m1 = create_menu_item((i32Vec2){0,0}, "Menu item 1");
    MenuItem *m2 = create_menu_item((i32Vec2){15,0}, "Menu item 2");
    MenuItem *m3 = create_menu_item((i32Vec2){0,3}, "Menu item 3");
    MenuItem *m4 = create_menu_item((i32Vec2){15,3}, "Menu item 4");
    MenuItem *log = create_menu_item((i32Vec2){0,5}, "");

    add_menu_item(menu, m1);
    add_menu_item(menu, m2);
    add_menu_item(menu, m3);
    add_menu_item(menu, m4);
    add_menu_item(menu, log);

    connect_menu_items(m1, m3, DIR_DOWN, true);
    connect_menu_items(m2, m4, DIR_DOWN, true);
    connect_menu_items(m1, m2, DIR_RIGHT, true);
    connect_menu_items(m3, m4, DIR_RIGHT, true);

    menu_item_set_onclick(m1, &print_stuff);
    menu_item_set_onclick(m2, &print_stuff);
    menu_item_set_onclick(m3, &print_stuff);
    menu_item_set_onclick(m4, &print_stuff);

    render_menu(menu);

    return menu;
}

void start_menu(void){
    Menu *m = setup();
    bool exit = false;

    while (!exit)
    {
        econio_rawmode();
        switch(econio_getch()){
            case KEY_UP:
                menu_move_selection(m, DIR_UP);
                break;
            case KEY_DOWN:
                menu_move_selection(m, DIR_DOWN);
                break;
            case KEY_LEFT:
                menu_move_selection(m, DIR_LEFT);
                break;
            case KEY_RIGHT:
                menu_move_selection(m, DIR_RIGHT);
                break;
            case KEY_ENTER:
                menu_dispatch_onclick_events(m);
                break;
            case KEY_ESCAPE:
                exit = true;
            break;
        }
        render_menu(m);
    }
    free_menu(m);
}

void start_tui(){
    TuiObject *root = tui_create_tuiobj(NULL);

    TuiObject *a1 = tui_create_tuiobj(root);
    TuiObject *a2 = tui_create_tuiobj(root);
    TuiObject *a3 = tui_create_tuiobj(root);

    tui_debug_tree(root, 0);

    tui_free_tuiobj(root);
}


int main(void) {
    setlocale(LC_ALL, ".UTF8");

    start_tui();
    return 0;
    //start_menu();
    
    State game_state;
    game_init(&game_state);
    game_start(&game_state);
    game_end(&game_state);
    return 0;
}