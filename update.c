#include <SDL.h>
#include <SDL_image.h>
#include "functions.h"

_Bool
mapDraw(SDL_Renderer *gRenderer, MapSize dimensions, MapTile map[][dimensions.x], int x, int y)
{	
	for (int i = 0; i < dimensions.y; ++i) {
		for (int k = 0; k < dimensions.x; ++k) {
//        for (int i = dimensions.y; i > 0; --i) {
//                for (int k = dimensions.x; k > 0; --k) {
                            map[i][k].rect.x = (k*64)+x;
			map[i][k].rect.y = (i*64)+y;
                        printf("In mapDraw: [%d][%d]\n", i, k);                        
                        SDL_RenderCopy(gRenderer, map[i][k].texture, NULL, toIsometric(&map[i][k].rect)); 
                }
	}
        puts("exiting mapDraw()");
	return 1;
}

void
placeObject(SDL_Renderer *gRenderer, SDL_Texture *objectTexture, SDL_Rect *objectRect)
{
        SDL_RenderCopy(gRenderer, objectTexture, NULL, objectRect);
	SDL_UpdateWindowSurface(window);
}

SDL_Rect*
toIsometric(SDL_Rect* cartRect)
{	
        int isoX, isoY;
	isoX = cartRect->x - cartRect->y;
	isoY = (cartRect->x + cartRect->y) / 2;
	cartRect->x = isoX;
	cartRect->y = isoY;
	
	return cartRect;

}

void
drawScreen(SDL_Renderer *gRenderer, SDL_Rect *backRect, SDL_Rect *playerRect, SDL_Rect *enemyRect, SDL_Texture *gCurrentTexture, 
           SDL_Texture *mapTiles[TOTAL_TILES], SDL_Texture *mapBackground, MapSize dimensions, MapTile map[][dimensions.x], int *x, int *y)
{
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, mapBackground, NULL, backRect); 
	mapDraw(gRenderer, dimensions, map, *x, *y);
        enemyMovement(enemyRect);
        placeObject(gRenderer, mapTiles[6], enemyRect);
	placeObject(gRenderer, gCurrentTexture, playerRect);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(20);
}

