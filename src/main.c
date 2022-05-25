#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "ball.h"

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   1200

#define FRAMES_PER_SEC  60
#define TICKS_PER_FRAME (1000 / FRAMES_PER_SEC)

#define PADDLE_WIDTH    75
#define PADDLE_HEIGHT   25

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    srand(time(0));

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, &window,
                &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "failed to create window and renderer: %s", SDL_GetError());
        return 1;
    }

    Ball ball = {
        // Send ball in random direction.
        .x_velocity = choose_dx(),
        .y_velocity = choose_dx(),

        // Center ball.
        .image = {
            .x = WINDOW_WIDTH / 2,
            .y = WINDOW_HEIGHT / 2,
            .w = BALL_DIAMETER,
            .h = BALL_DIAMETER,
        },

        // Load ball.
        .texture = load_ball(renderer, "./assets/ball.bmp"),
    };
    if (!ball.texture) {
        return 1;
    }

    SDL_Rect paddle = {
        .x = WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2,
        .y = WINDOW_HEIGHT - 100,
        .w = PADDLE_WIDTH,
        .h = PADDLE_HEIGHT,
    };

    for (;;) {
        unsigned int start = SDL_GetTicks();

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

        // Update position and velocity of ball.
        ball.image.x += ball.x_velocity;
        ball.image.y += ball.y_velocity;
        if (ball.image.x < 0 || ball.image.x + BALL_DIAMETER > WINDOW_WIDTH)
            ball.x_velocity = -ball.x_velocity;
        if (ball.image.y < 0 || ball.image.y + BALL_DIAMETER > WINDOW_HEIGHT)
            ball.y_velocity = -ball.y_velocity;

        // Clear the screen to draw the next frame.
        SDL_RenderClear(renderer);

        // Refresh paddle.
        SDL_SetRenderDrawColor(renderer, 58, 139, 232, 255);
        SDL_RenderFillRect(renderer, &paddle);

        // Refresh ball.
        SDL_RenderCopy(renderer, ball.texture, NULL, &ball.image);

        // Draw a black background.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

        // Push the new frame.
        SDL_RenderPresent(renderer);

        // Stall to limit FPS and thus decrease speed of ball.
        unsigned int elapsed_time = SDL_GetTicks() - start;
        if (elapsed_time < TICKS_PER_FRAME)
            SDL_Delay(TICKS_PER_FRAME - elapsed_time);
    }

exit:
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
