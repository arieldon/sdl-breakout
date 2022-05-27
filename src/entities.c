#include <stdlib.h>

#include <SDL2/SDL.h>

#include "entities.h"

SDL_Texture *load_ball(SDL_Renderer *renderer, const char *path)
{
    SDL_Surface *surface = SDL_LoadBMP(path);
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "failed to load ball BMP from '%s' file: %s",
                path, SDL_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "failed to create SDL texture for ball from SDL surface: %s",
                SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(surface);

    return texture;
}

inline int choose_dx(void)
{
    return rand() % 2 ? 1 : -1;
}
