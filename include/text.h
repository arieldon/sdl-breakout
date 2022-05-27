#ifndef TEXT_H
#define TEXT_H

enum {
    FONT_SIZE = 64,
};

typedef struct {
    char *content;
    TTF_Font *font;
    SDL_Texture *texture;
    int width;
    int height;
    SDL_Rect rect;
} Text;

int init_text(SDL_Renderer *, Text *);
void center_text(Text *, int window_width, int window_height);
void write_text(SDL_Renderer *, Text *);

#endif
