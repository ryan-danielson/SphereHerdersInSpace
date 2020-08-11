#include <SDL.h>
#include <SDL_image.h>
#include "functions.h"

SDL_Window*
init(SDL_Renderer **gRenderer)
{
        /* Initialize SDL */
        if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
                printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
                return 0;
        }  else {
        /*  Create window  */
                window = SDL_CreateWindow( "SDL Isometric", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
                if (window == NULL)  {
                        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
                        return 0;
	        } else {
                        *gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
                        if (gRenderer == NULL) {
                                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                                return 0;
                        } else {
                                SDL_SetRenderDrawColor(*gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	                        int imgFlags = IMG_INIT_PNG;
	                        if (!(IMG_Init(imgFlags) & imgFlags)) {
		                        printf("SDL_image could not initilize! SDL_image error: %s\n", IMG_GetError());
	                                return 0;
                                }
                        }
	    
                }
        }
        return window;
}

_Bool
loadMedia(SDL_Texture **gKeyPressTextures, SDL_Renderer **gRenderer, SDL_Texture **mapTiles)
{
	gKeyPressTextures[KEY_PRESS_SURFACE_DEFAULT]     = loadTexture(PLAYER1, gRenderer);
	gKeyPressTextures[KEY_PRESS_SURFACE_UP]          = loadTexture(PLAYER1, gRenderer);
	gKeyPressTextures[KEY_PRESS_SURFACE_DOWN]        = loadTexture(PLAYER1, gRenderer);
	gKeyPressTextures[KEY_PRESS_SURFACE_LEFT]        = loadTexture(PLAYER1, gRenderer);
	gKeyPressTextures[KEY_PRESS_SURFACE_RIGHT]       = loadTexture(PLAYER1, gRenderer);
        gKeyPressTextures[KEY_PRESS_SURFACE_SHIFT]       = loadTexture(PLAYER2, gRenderer);
        gKeyPressTextures[KEY_PRESS_SURFACE_JUMP]        = loadTexture(PLAYER1, gRenderer);
	mapTiles[0] = loadTexture(TILE1, gRenderer);
	mapTiles[1] = loadTexture(TILE2, gRenderer);
	mapTiles[2] = loadTexture(TILE3, gRenderer);
	mapTiles[3] = loadTexture(TILE4, gRenderer);
	mapTiles[4] = loadTexture(TILE5, gRenderer);
	mapTiles[5] = loadTexture(TILE6, gRenderer);
	mapTiles[6] = loadTexture(ENEMY, gRenderer);

	/* check for NULL key press surfaces */

	for (int i = 0; i < KEY_PRESS_TEXTURE_TOTAL; ++i) {
		if (&gKeyPressTextures[i] == NULL) {
			printf("Failed to load keypress surface %d\n", i);
			return 0;
		}
	}

	/* check for NULL tiles */
	for (int i = 0; i < TOTAL_TILES; ++i) {
		if (&mapTiles[i] == NULL) {
			printf("Failed to load TILE%d\n", i);
			return 0;
		}
	}
	
	return 1;
}

SDL_Texture* 
loadTexture(char* somePath, SDL_Renderer **gRenderer)
{
        SDL_Texture *newTexture = NULL;
        SDL_Surface *loadedSurface = IMG_Load(somePath);
	
	if (loadedSurface == NULL)
		printf("Unable to load surface: %s\n", somePath);
        newTexture = SDL_CreateTextureFromSurface(*gRenderer, loadedSurface);

        if (newTexture == NULL)
                printf("Unable to create texture! SDL Error: %s\n", SDL_GetError());

        /* free old surface */

        SDL_FreeSurface(loadedSurface);
        return newTexture;
}
