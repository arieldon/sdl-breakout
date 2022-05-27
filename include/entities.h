#ifndef ENTITIES_H
#define ENTITIES_H

#include <SDL2/SDL.h>

enum {
    WINDOW_WIDTH    = 600,
    WINDOW_HEIGHT   = 800,

    BALL_DIAMETER   = 16,
    BALL_RADIUS     = BALL_DIAMETER / 2,

    PADDLE_WIDTH    = 75,
    PADDLE_HEIGHT   = 25,
    PADDLE_VELOCITY = 10,

    TARGET_WIDTH    = PADDLE_WIDTH,
    TARGET_HEIGHT   = 15,
    TARGET_ROWS     = 5,
    TARGET_COLUMNS  = 5,
    TARGET_TOTAL    = TARGET_ROWS * TARGET_COLUMNS,
    TARGET_MARGIN_X = 40,
    TARGET_MARGIN_Y = 20,
    TARGETS_WIDTH   = TARGET_COLUMNS * TARGET_WIDTH
        + (TARGET_COLUMNS - 1) * TARGET_MARGIN_X,
    TARGET_OFFSET_X = WINDOW_WIDTH / 2 - TARGETS_WIDTH / 2,
    TARGET_OFFSET_Y = 50,

    FRAMES_PER_SEC  = 60,
    TICKS_PER_FRAME = 1000 / FRAMES_PER_SEC,
};

typedef struct {
    int x_velocity;
    int y_velocity;
    SDL_Rect image;
    SDL_Texture *texture;
} Ball;

SDL_Texture *load_ball(SDL_Renderer *, const char *);
int choose_dx(void);

#endif
