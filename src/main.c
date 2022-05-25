#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#define WINDOW_WIDTH    600
#define WINDOW_HEIGHT   800

#define PADDLE_WIDTH    75
#define PADDLE_HEIGHT   25

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

    SDL_Rect paddle = {
        .x = WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2,
        .y = WINDOW_HEIGHT - 100,
        .w = PADDLE_WIDTH,
        .h = PADDLE_HEIGHT,
    };

    for (;;) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                goto exit;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_d:
                case SDLK_RIGHT:
                    if (paddle.x + PADDLE_WIDTH < WINDOW_WIDTH) {
                        paddle.x += 10;
                    }
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                    if (paddle.x > 0) {
                        paddle.x -= 10;
                    }
                    break;
                }
            }
        }

        // Clear the screen to draw the next frame.
        SDL_RenderClear(renderer);

        // Refresh paddle.
        SDL_SetRenderDrawColor(renderer, 58, 139, 232, 255);
        SDL_RenderFillRect(renderer, &paddle);

        // Draw a black background.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

        // Push the new frame.
        SDL_RenderPresent(renderer);
    }

exit:
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
