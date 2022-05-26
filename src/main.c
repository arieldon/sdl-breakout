#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "ball.h"

#define WINDOW_WIDTH    600
#define WINDOW_HEIGHT   800

#define FRAMES_PER_SEC  60
#define TICKS_PER_FRAME (1000 / FRAMES_PER_SEC)

#define PADDLE_WIDTH    75
#define PADDLE_HEIGHT   25

#define TARGET_WIDTH    PADDLE_WIDTH
#define TARGET_HEIGHT   15
#define TARGET_ROWS     5
#define TARGET_COLUMNS  5
#define TARGET_TOTAL    (TARGET_ROWS * TARGET_COLUMNS)
#define TARGET_MARGIN_X 40
#define TARGET_MARGIN_Y 20
#define TARGETS_WIDTH   (TARGET_COLUMNS * TARGET_WIDTH + \
        (TARGET_COLUMNS - 1) * TARGET_MARGIN_X)
#define TARGET_OFFSET_X (WINDOW_WIDTH / 2 - TARGETS_WIDTH / 2)
#define TARGET_OFFSET_Y 50

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
        .x_velocity = choose_dx() * 2,
        .y_velocity = choose_dx() * 2,

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

    SDL_Rect targets[TARGET_TOTAL];
    for (int row = 0; row < TARGET_ROWS; ++row) {
        for (int column = 0; column < TARGET_COLUMNS; ++column) {
            SDL_Rect *target = &targets[row * TARGET_COLUMNS + column];
            target->x = column * (TARGET_WIDTH + TARGET_MARGIN_X) + TARGET_OFFSET_X;
            target->y = row * (TARGET_HEIGHT + TARGET_MARGIN_Y) + TARGET_OFFSET_Y;
            target->w = TARGET_WIDTH;
            target->h = TARGET_HEIGHT;
        }
    }

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

        // End the game when ball falls below paddle. Note (0, 0) coordinate
        // sits at top left.
        if (ball.image.y + BALL_DIAMETER >= WINDOW_HEIGHT) {
            puts("Game over.");
            break;
        }

        // Check for collision between ball and bounds.
        if (ball.image.x <= 0 || ball.image.x + BALL_DIAMETER >= WINDOW_WIDTH)
            ball.x_velocity = -ball.x_velocity;
        if (ball.image.y <= 0 || ball.image.y + BALL_DIAMETER >= WINDOW_HEIGHT)
            ball.y_velocity = -ball.y_velocity;

        SDL_Rect intersection = {0};
        if (SDL_IntersectRect(&ball.image, &paddle, &intersection)) {
            if (ball.x_velocity > 0 && intersection.x - paddle.x <= BALL_RADIUS)
                ball.x_velocity = -ball.x_velocity;
            else if (ball.x_velocity < 0
                    && paddle.x + paddle.w - intersection.x <= BALL_RADIUS)
                ball.x_velocity = -ball.x_velocity;
            if (intersection.y == paddle.y)
                ball.y_velocity = -ball.y_velocity;
        }

        // Update position of ball.
        ball.image.x += ball.x_velocity;
        ball.image.y += ball.y_velocity;

        // Clear the screen to draw the next frame.
        SDL_RenderClear(renderer);

        // Refresh paddle.
        SDL_SetRenderDrawColor(renderer, 58, 139, 232, 255);
        SDL_RenderFillRect(renderer, &paddle);

        // Refresh ball.
        SDL_RenderCopy(renderer, ball.texture, NULL, &ball.image);

        // Refresh targets.
        SDL_SetRenderDrawColor(renderer, 226, 198, 86, 255);
        SDL_RenderFillRects(renderer, targets, TARGET_TOTAL);

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
