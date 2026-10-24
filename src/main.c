#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
 
int main(int argc, char *argv[]) {
    /* SDL inicializálása és ablak megnyitása */
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("SDL peldaprogram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 440, 360, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);
    
    /* az esemenyvezerelt hurok */
    bool quit = false;
    bool click = false;
    int elozox = 0;
    int elozoy = 0;
    while (!quit) {
        SDL_Event event;
        SDL_WaitEvent(&event);
 
        bool rajzoltam = false;
 
        switch (event.type) {
            /* eger kattintas */
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    click = true;
                    elozox = event.button.x;
                    elozoy = event.button.y;
                }
                else if (event.button.button == SDL_BUTTON_RIGHT) {
                    boxColor(renderer, 0, 0, 439, 359, 0x000000FF);
                    rajzoltam = true;
                }
                break;	
            /* egergomb elengedese */
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    click = false;
                }
                break;
            /* eger mozdulat */
            case SDL_MOUSEMOTION:
                if (click) {
                    aalineColor(renderer, elozox, elozoy,
                                event.motion.x, event.motion.y, 0xFFFFFFFF);
                    rajzoltam = true;
                }
                /* a kovetkezo mozdulat esemenyhez */
                elozox = event.motion.x;
                elozoy = event.motion.y;
                break;
            /* ablak bezarasa */
            case SDL_QUIT:
                quit = true;
                break;
        }
 
		boxColor(renderer, 10, 10, 30, 30, 0xFF0000FF);
 
        if (rajzoltam)
            SDL_RenderPresent(renderer);
    }
 
    SDL_Quit();
 
    return 0;
}