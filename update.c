#include <SDL.h>
#include <SDL_image.h>
#include "functions.h"

_Bool
mapDraw(SDL_Renderer *gRenderer, SDL_Rect *tileRect, SDL_Texture **mapTiles, MapSize dimensions, int map[][dimensions.x], int x, int y)
{
	if (&mapTiles[0] == NULL || &mapTiles[1] == NULL || &mapTiles[2] == NULL) {
		puts("mapTiles has no content in mapMaker");
		return 0;
	}
	
	for (int i = 0; i < dimensions.y; ++i) {
		for (int k = 0; k < dimensions.x; ++k) {
			tileRect->x = (k*64)+x;
			tileRect->y = (i*64)+y;
			
                        switch (map[i][k]) {
                        case 0:
                                SDL_RenderCopy(gRenderer, mapTiles[0], NULL, toIsometric(tileRect));
                                break;
                        case 1:
                                SDL_RenderCopy(gRenderer, mapTiles[1], NULL, toIsometric(tileRect));
                                break;
                        case 2:
                                SDL_RenderCopy(gRenderer, mapTiles[2], NULL, toIsometric(tileRect));
                                break;
                        case 3:
                                SDL_RenderCopy(gRenderer, mapTiles[3], NULL, toIsometric(tileRect));
                                break;
                        case 4:
                                SDL_RenderCopy(gRenderer, mapTiles[4], NULL, toIsometric(tileRect));
                                break;
                        case 5:
                                SDL_RenderCopy(gRenderer, mapTiles[5], NULL, toIsometric(tileRect));
                                break;
                        case 6:
                                SDL_RenderCopy(gRenderer, mapTiles[6], NULL, toIsometric(tileRect));
                                break;
                        default:
                                break;
                        } 
                }
	}
	tileRect->y = 0;

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
drawScreen(SDL_Renderer *gRenderer, SDL_Rect *backRect, SDL_Rect *tileRect, SDL_Rect *playerRect, SDL_Rect *enemyRect, SDL_Texture *gCurrentTexture, 
           SDL_Texture *mapTiles[TOTAL_TILES], SDL_Texture *mapBackground, MapSize dimensions, int map[][dimensions.x], int *x, int *y)
{
        SDL_RenderClear(gRenderer);
        SDL_RenderCopy(gRenderer, mapBackground, NULL, backRect); 
	mapDraw(gRenderer, tileRect, mapTiles, dimensions, map, *x, *y);
        enemyMovement(enemyRect);
        placeObject(gRenderer, mapTiles[6], enemyRect);
	placeObject(gRenderer, gCurrentTexture, playerRect);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(20);
}

