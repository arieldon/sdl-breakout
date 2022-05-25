#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#define WINDOW_WIDTH    600
#define WINDOW_HEIGHT   800

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "failed to initialize SDL: \
                %s", SDL_GetError());
        return 1;
    }

    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, &window,
                &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "failed to create window \
                and renderer: %s", SDL_GetError());
        return 1;
    }

    SDL_Event event = {0};
    for (;;) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
