#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "../../game/game.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *screen = NULL;

typedef u16 rgb555_t;
typedef struct {
    rgb555_t pixels[YETI_VIEWPORT_HEIGHT][YETI_VIEWPORT_WIDTH];
} framebuffer_t;

framebuffer_t framebuffer;
yeti_t yeti;
int oldTick = 0;
int active = 0;

void render_frame()
{
    int currentTick;
    int waitTicks;
    currentTick = SDL_GetTicks();
    waitTicks = (oldTick + (YETI_VIEWPORT_INTERVAL)) - currentTick;
    oldTick = currentTick;

    if (waitTicks > 0) {
        SDL_Delay(waitTicks);
    }

    game_loop(&yeti);
    SDL_UpdateTexture(screen, NULL, yeti.viewport.back, YETI_VIEWPORT_WIDTH * sizeof(u16));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, screen, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    Uint8 *keys;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("Unable to init SDL: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
                 "Yeti",
                 SDL_WINDOWPOS_UNDEFINED,
                 SDL_WINDOWPOS_UNDEFINED,
                 YETI_VIEWPORT_WIDTH,
                 YETI_VIEWPORT_HEIGHT,
                 SDL_WINDOW_OPENGL
             );

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return 1;
    }

    screen = SDL_CreateTexture(
                 renderer,
                 SDL_PIXELFORMAT_BGR555,
                 SDL_TEXTUREACCESS_STREAMING,
                 YETI_VIEWPORT_WIDTH,
                 YETI_VIEWPORT_HEIGHT
             );

    if (screen == NULL) {
        printf("Could not create screen: %s\n", SDL_GetError());
        return 1;
    }

    // SDL initialization
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    // Yeti initialization
    yeti_init(&yeti, &framebuffer, &framebuffer, textures, palette);
    yeti_init_lua(&yeti, YETI_GAMMA);
    game_init(&yeti);

    while (active == 0) {
        SDL_Event event;
        render_frame();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                active = 1;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    active = 1;
                }
            }
        }

        keys = SDL_GetKeyboardState(NULL);
        yeti.keyboard.state.up = keys[SDL_SCANCODE_UP];
        yeti.keyboard.state.down = keys[SDL_SCANCODE_DOWN];
        yeti.keyboard.state.left = keys[SDL_SCANCODE_LEFT];
        yeti.keyboard.state.right = keys[SDL_SCANCODE_RIGHT];
        yeti.keyboard.state.a = keys[SDL_SCANCODE_RCTRL];
        yeti.keyboard.state.b = keys[SDL_SCANCODE_SPACE];
        yeti.keyboard.state.l = keys[SDL_SCANCODE_A];
        yeti.keyboard.state.r = keys[SDL_SCANCODE_Z];
        yeti.keyboard.state.select = keys[SDL_SCANCODE_ESCAPE];
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
