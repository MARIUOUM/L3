#define MAX_ZOOM_OUT    1
#define MAX_ZOOM_IN     4

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_rotozoom.h>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

void handleInput(SDL_Renderer *mapRenderer, SDL_Texture *texture);
void draw_name(SDL_Renderer *mapRenderer,char *text,int fontWidth,int x, int y,int w,int h,uint8_t  r,uint8_t g,uint8_t  b,uint8_t  a, double angle);
void setupRect(SDL_Rect *rect,int x,int y,int w,int h);
