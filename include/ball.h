#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>

#define BALL_DIAMETER   16
#define BALL_RADIUS     (BALL_DIAMETER / 2)

typedef struct {
    int x_velocity;
    int y_velocity;
    SDL_Rect image;
    SDL_Texture *texture;
} Ball;

SDL_Texture *load_ball(SDL_Renderer *, const char *);
int choose_dx(void);

#endif
