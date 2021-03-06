#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "colors.h"
#include "entities.h"
#include "text.h"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    srand(time(0));

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *font = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "failed to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "failed to initialize SDL TTF: %s", TTF_GetError());
        return 1;
    }

    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, &window,
                &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "failed to create window and renderer: %s", SDL_GetError());
        return 1;
    }

    font = TTF_OpenFont("./assets/Inconsolata-Bold.ttf", FONT_SIZE);
    if (!font) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "failed to open font: %s", TTF_GetError());
        return 1;
    }

    Ball ball = {
        .dx = BALL_DX,
        .dy = BALL_DY,
        .rect = {
            .x = WINDOW_WIDTH / 2 - BALL_RADIUS,
            .y = WINDOW_HEIGHT / 2 - BALL_RADIUS,
            .w = BALL_DIAMETER,
            .h = BALL_DIAMETER,
        },
        .texture = load_ball(renderer, "./assets/ball.bmp"),
    };
    if (!ball.texture) return 1;

    // Initialize paddle for player.
    Paddle paddle = {
        .dx = 0,
        .rect = {
            .x = WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2,
            .y = WINDOW_HEIGHT - PADDLE_MARGIN,
            .w = PADDLE_WIDTH,
            .h = PADDLE_HEIGHT,
        },
    };

    // Initialize targets or bricks to hit.
    Target targets[TARGET_TOTAL];
    int targets_active = TARGET_TOTAL;
    for (int row = 0; row < TARGET_ROWS; ++row) {
        SDL_Color *color = &COLORS[row % COLOR_TOTAL];
        for (int column = 0; column < TARGET_COLUMNS; ++column) {
            Target *target = &targets[row * TARGET_COLUMNS + column];
            target->color = color;

            SDL_Rect *rect = &target->rect;
            rect->x = column * (TARGET_WIDTH + TARGET_MARGIN_X) + TARGET_OFFSET_X;
            rect->y = row * (TARGET_HEIGHT + TARGET_MARGIN_Y) + TARGET_OFFSET_Y;
            rect->w = TARGET_WIDTH;
            rect->h = TARGET_HEIGHT;
        }
    }

    bool victory = false;
    while (!victory) {
        unsigned int start = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                goto exit;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_d:
                case SDLK_RIGHT:
                    if (paddle.rect.x + PADDLE_WIDTH < WINDOW_WIDTH) paddle.dx = PADDLE_DX;
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                    if (paddle.rect.x > 0) paddle.dx = -PADDLE_DX;
                    break;
                }
            }
        }

        // End the game when ball falls below paddle. Note (0, 0) coordinate
        // sits at top left.
        if (ball.rect.y + BALL_DIAMETER >= WINDOW_HEIGHT)
            break;

        // Check for collision between ball and bounds.
        if (ball.rect.x <= 0 || ball.rect.x + BALL_DIAMETER >= WINDOW_WIDTH)
            ball.dx = -ball.dx;
        if (ball.rect.y <= 0 || ball.rect.y + BALL_DIAMETER >= WINDOW_HEIGHT)
            ball.dy = -ball.dy;

        // Check for collision between ball and paddle or collision between
        // ball and any target.
        SDL_Rect intersection = {0};
        if (SDL_IntersectRect(&ball.rect, &paddle.rect, &intersection)) {
            if (ball.rect.y <= paddle.rect.y)
                ball.dy = -ball.dy;

            // Set velocity of the ball in the x-direction upon collision as a
            // function of the distance between the collision point and the
            // center of the paddle.
            double d = (ball.rect.x + BALL_RADIUS) - (paddle.rect.x + paddle.rect.w / 2);
            ball.dx = (d / paddle.rect.w) * 10;
        } else {
            for (int i = 0; i < targets_active; ++i) {
                SDL_Rect *target = &targets[i].rect;
                if (SDL_IntersectRect(&ball.rect, target, &intersection)) {
                    // Respond to collision.
                    if (ball.rect.y + BALL_RADIUS <= target->y
                            || ball.rect.y + BALL_RADIUS >= target->y + target->h)
                        ball.dy = -ball.dy;
                    if (ball.rect.x + BALL_RADIUS <= target->x
                            || ball.rect.x + BALL_RADIUS >= target->x + target->w)
                        ball.dx = -ball.dx;

                    // Disable target.
                    Target temporary_target = targets[--targets_active];
                    targets[targets_active] = targets[i];
                    targets[i] = temporary_target;
                }
            }
        }
        if (!targets_active) victory = true;

        // Update position of ball.
        ball.rect.x += ball.dx;
        ball.rect.y += ball.dy;

        // Update position of paddle.
        paddle.rect.x += paddle.dx;
        paddle.dx = 0;

        // Clear screen to draw next frame.
        SDL_RenderClear(renderer);

        // Refresh paddle.
        SDL_SetRenderDrawColor(renderer, 58, 139, 232, 255);
        SDL_RenderFillRect(renderer, &paddle.rect);

        // Refresh ball.
        SDL_RenderCopy(renderer, ball.texture, NULL, &ball.rect);

        // Refresh targets.
        for (int i = 0; i < targets_active; ++i) {
            SDL_Color *color = targets[i].color;
            SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
            SDL_RenderFillRect(renderer, &targets[i].rect);
        }

        // Draw black background.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

        // Push the new frame.
        SDL_RenderPresent(renderer);

        // Stall to limit FPS and thus decrease speed of ball.
        unsigned int elapsed_time = SDL_GetTicks() - start;
        if (elapsed_time < TICKS_PER_FRAME)
            SDL_Delay(TICKS_PER_FRAME - elapsed_time);
    }

    Text end_text = {
        .font = font,
        .content = victory ? "VICTORY." : "GAME OVER.",
    };
    if (init_text(renderer, &end_text) == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "failed to create text for end of game");
        return 1;
    }
    center_text(&end_text, WINDOW_WIDTH, WINDOW_HEIGHT);
    write_text(renderer, &end_text);

    // Any event leads to exit. All roads lead to Rome.
    SDL_Event event;
    SDL_WaitEvent(&event);

exit:
    TTF_CloseFont(font);

    SDL_DestroyTexture(ball.texture);
    SDL_DestroyTexture(end_text.texture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}
