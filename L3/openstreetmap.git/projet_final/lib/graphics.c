#include "utils.h"
#include "graphics.h"

// Largeur de la fenêtre
int SCREEN_WIDTH;

// Hauteur de la fenêtre
int SCREEN_HEIGHT;

float zoomFactor=1.0;
int xViewPos=0;
int yViewPos=0;

SDL_Event event;
SDL_Rect rect;


void draw_name(SDL_Renderer *mapRenderer,char *name,int Widthfont,int x, int y,int w,int h,uint8_t  r,uint8_t g,uint8_t  b, uint8_t a, double angle)
{
	TTF_Init();
	TTF_Font *police = NULL;
	police = TTF_OpenFont("arial_black.ttf",Widthfont);	
	SDL_Color color = {r,g,b,a};
	SDL_Surface* textSurf = TTF_RenderText_Blended(police,name,color);

	SDL_Surface *roto_name = rotozoomSurface(textSurf,angle,1.0,1);

	SDL_Texture* texture_name = SDL_CreateTextureFromSurface(mapRenderer,roto_name);

	SDL_Rect rect; //create a rec
	rect.x = x;  //controls the rect's x coordinate 
	rect.y = y; // controls the rect's y coordinte
	rect.w = w; // controls the width of the rect
	rect.h = h; // controls the height of the rect

	SDL_RenderCopyEx(mapRenderer,texture_name, NULL,  &rect, 0, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(mapRenderer);

	TTF_CloseFont(police);
	TTF_Quit();

}

//Function to make it easier to setup rectangles
void setupRect(SDL_Rect *rect,int x,int y,int w,int h)
{
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}
void zoomIn()
{
    float newW,newH;

    //increase zoomfactor
    zoomFactor+=0.25;

    //stop zooming at the defined max zoom in value
    if(zoomFactor>=MAX_ZOOM_IN)
        zoomFactor=MAX_ZOOM_IN;

    //calculate the new width and height
    newW = SCREEN_WIDTH/zoomFactor;
    newH = SCREEN_HEIGHT/zoomFactor;

    //calculate the new x & y position
    xViewPos = (SCREEN_WIDTH-newW)/2;
    yViewPos = (SCREEN_HEIGHT-newH)/2;

    //setup the new rectangle
    setupRect(&rect,xViewPos,yViewPos,(int)newW,(int)newH);
}

// This function zooms out the game view
void zoomOut()
{
    float newW,newH;

    //decrease zoomfactor
    zoomFactor-=0.25;

    //stop zooming at the defined max zoom out value
    if(zoomFactor<=MAX_ZOOM_OUT)
        zoomFactor=MAX_ZOOM_OUT;

    //calculate new width and height
    newW = SCREEN_WIDTH/zoomFactor;
    newH = SCREEN_HEIGHT/zoomFactor;

    //calculate the new x & y position
    xViewPos = (SCREEN_WIDTH-newW)/2;
    yViewPos = (SCREEN_HEIGHT-newH)/2;

    //setup the new rectangle
    setupRect(&rect,xViewPos,yViewPos,(int)newW,(int)newH);
}


// This function handles all the input
void handleInput(SDL_Renderer *mapRenderer, SDL_Texture *texture)
{
	SDL_Event event;
	int terminer = 0;
	SDL_Rect rect;
	float newW,newH;

	//increase zoomfactor
    zoomFactor+=0.25;

    //stop zooming at the defined max zoom in value
    if(zoomFactor>=MAX_ZOOM_IN)
        zoomFactor=MAX_ZOOM_IN;

    //calculate the new width and height
    newW = SCREEN_WIDTH/zoomFactor;
    newH = SCREEN_HEIGHT/zoomFactor;

    //calculate the new x & y position
    xViewPos = (SCREEN_WIDTH-newW)/2;
    yViewPos = (SCREEN_HEIGHT-newH)/2;





			//SDL_RendererFlip flipType = SDL_FLIP_NONE;

				//set zoom rectangle to 1:1
  		setupRect(&rect,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
      SDL_SetRenderTarget(mapRenderer, NULL);
      SDL_RenderCopyEx(mapRenderer, texture, &rect, &rect, 0, NULL, SDL_FLIP_NONE);
      SDL_RenderPresent(mapRenderer);

	while(!terminer)
    {
		SDL_WaitEvent(&event);
		
		if(event.window.event == SDL_WINDOWEVENT_CLOSE)
			terminer = 1;
		switch(event.type)
		{
			case SDL_QUIT:
				terminer = 1;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						terminer = 1;
						break;
					case SDLK_UP: // Flèche haut
						printf("Flèche haut");	
						setupRect(&rect,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);			
						zoomIn();
						setupRect(&rect,xViewPos,yViewPos,(int)newW,(int)newH);
						

						break;
					case SDLK_DOWN: // Flèche bas
						printf("Flèche bas");
						setupRect(&rect,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);						
						zoomOut();
						setupRect(&rect,xViewPos,yViewPos,SCREEN_WIDTH,SCREEN_HEIGHT);		
						
					
						break;
					case SDLK_RIGHT: // Flèche droite
						printf("Flèche droite");
						setupRect(&rect,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
						zoomIn();
						setupRect(&rect,xViewPos,yViewPos,(int)newW,(int)newH);
						
						break;
					case SDLK_LEFT: // Flèche gauche
						printf("Flèche gauche");
						setupRect(&rect,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
						zoomOut();
						setupRect(&rect,xViewPos,yViewPos,SCREEN_WIDTH,SCREEN_HEIGHT);

						break;
					case SDLK_PLUS: // On zoom
						printf("in zoom");
						break;
					case SDLK_MINUS:
						printf("out dezoom");
						break;



				}

				break;
		}
		
		
		//setupRect(&rect,xViewPos,yViewPos,(int)newW,(int)newH);
		SDL_SetRenderDrawColor( mapRenderer, 181, 208,208, 255 );
		SDL_RenderClear(mapRenderer );
		//Switch rendering to the window by passing NULL
    SDL_SetRenderTarget(mapRenderer, NULL);		
		SDL_RenderCopyEx(mapRenderer,texture, &rect, NULL, 0, NULL, SDL_FLIP_NONE);
		SDL_RenderPresent(mapRenderer);


	

    }
}


