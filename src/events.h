#include "darr.h";
#include "tui.h";

typedef struct {
  Darr tui_obj;//list of int: {index of child, index of child,...}

} EventSubRecord;

typedef struct {
  Darr *table;
} EventSubTable;

void dispatch_render(TuiObject *head){
  if(head->children != NULL){}
};

void event_init(){
  //create event-subscriber table
}