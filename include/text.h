#ifndef TEXT_H
#define TEXT_H

#define FONT_SIZE   64

typedef struct {
    char *content;
    TTF_Font *font;
    SDL_Texture *texture;
    int width;
    int height;
    SDL_Rect rect;
} Text;

int create_text(SDL_Renderer *renderer, Text *text);
void center_text(Text *text, int window_width, int window_height);

#endif
