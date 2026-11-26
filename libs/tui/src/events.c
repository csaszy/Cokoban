
#include <darr.h>
#include "events.h"
#include "debugmalloc.h"

EventSystem *tui_create_event_system(){
    EventSystem *event_system = (EventSystem *) malloc(sizeof(EventSystem));
    if(event_system == NULL) return NULL;

    event_system->event_queue = darr_create(sizeof(TUI_Event));

    return event_system;
}

void tui_free_event_system(EventSystem *event_system){
    darr_free(event_system->event_queue); 
    free(event_system);
}