#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "text.h"

int create_text(SDL_Renderer *renderer, Text *text)
{
    SDL_Surface *surface = TTF_RenderText_Solid(text->font, text->content,
            (SDL_Color){ 255, 255, 255, 255 });
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "failed to render text surface: %s", TTF_GetError());
        return -1;
    }

    text->width = surface->w;
    text->height = surface->h;
    text->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!text->texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "failed to create text texture from text surface: %s",
                TTF_GetError());
        return -1;
    }
    SDL_FreeSurface(surface);

    return 0;
}

void center_text(Text *text, int window_width, int window_height)
{
    text->rect.x = window_width / 2 - text->width / 2;
    text->rect.y = window_height / 2 - text->height / 2;
    text->rect.w = text->width;
    text->rect.h = text->height;
}
