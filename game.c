/* Using SDL and standard IO */

#include <SDL.h>
#include <SDL_image.h>
#include "functions.h"
#include <stdio.h>

const int SCREEN_WIDTH  = 1024;
const int SCREEN_HEIGHT = 768;
const int TOTAL_TILES = 7;

char *BACKGROUND        = "tiles/background.png";
char *PLAYER1           = "tiles/player.png";
char *PLAYER2           = "tiles/player2.png";
char *TILE1             = "tiles/tile4.png";
char *TILE2             = "tiles/tile5.png";
char *TILE3             = "tiles/tile6.png";
char *TILE4             = "tiles/tile8.png";
char *TILE5             = "tiles/tile7.png";
char *TILE6             = "tiles/tile11.png";
char *ENEMY             = "tiles/enemy.png";
char *BLANK             = "tiles/blank.png";
SDL_Window *window;



void
cleanBuffer()
{
	char ch;
	while (ch != '\0' && ch != EOF)
		ch = getchar();
}


void
end(SDL_Window *window, SDL_Texture **gKeyPressTextures, SDL_Texture **mapTiles)
{
        puts("Entered cleanup");
        for (int i = 0; i < KEY_PRESS_TEXTURE_TOTAL; ++i) {
                SDL_DestroyTexture(gKeyPressTextures[i]);
                printf("Free KEY_PRESS_SURFACE: %d\n", i);
        }

        for (int i = 0; i < TOTAL_TILES; ++i) {
                SDL_DestroyTexture(mapTiles[i]);
                printf("Free MAP_TILE: %d\n", i);
        }
	gKeyPressTextures = NULL;
        mapTiles = NULL;

	/* destroy window */
	SDL_DestroyWindow(window);

	/* quite SDL subsystem */
	SDL_Quit();
}

int 
main(int argc, char *args[])
{ 
        SDL_Surface *screenSurface = NULL;
        SDL_Renderer *gRenderer = NULL; 
        SDL_Rect *playerRect = NULL, *tileRect = NULL, *backRect = NULL, *enemyRect = NULL;
        SDL_Event e;
        int x, y;



        /* new section with texture and render :( */

        SDL_Texture *gKeyPressTextures[KEY_PRESS_TEXTURE_TOTAL];
        SDL_Texture *mapTiles[TOTAL_TILES];
        SDL_Texture *gCurrentTexture;
        SDL_Texture *mapBackground;
        SDL_Texture *gEnemyTexture;

        playerRect = malloc(sizeof(SDL_Rect));
        playerRect->x = SCREEN_WIDTH/2;
        playerRect->y = 64;
        playerRect->h = 64;
        playerRect->w = 128;

        tileRect = malloc(sizeof(SDL_Rect));
        tileRect->x = 0;
        tileRect->y = 0;
        tileRect->h = 64;
        tileRect->w = 128;

        backRect = malloc(sizeof(SDL_Rect));
        backRect->x = 0;
        backRect->y = 0;
        backRect->h = SCREEN_HEIGHT;
        backRect->w = SCREEN_WIDTH;

        enemyRect = malloc(sizeof(SDL_Rect));
        enemyRect->x = SCREEN_WIDTH/2;
        enemyRect->y = 512;
        enemyRect->h = 64;
        enemyRect->w = 128;


        x = 386;
        y = 0;
        _Bool quit = 0;	

        /* create window */
        window = init(&gRenderer);
        screenSurface = SDL_GetWindowSurface(window);
        
        puts("Debug 1");

        puts("Debug 2");
        if (window == NULL) {
	        puts("Window could not be initialized");
	        return 1;
        }

         /* load media to window */
        puts("loading media");
        if (!loadMedia(&gKeyPressTextures[0], &gRenderer, &mapTiles[0])) {
	        puts("Failed to load media.");
	        return 1;
        }

        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_DEFAULT];
        mapBackground = loadTexture(BACKGROUND, &gRenderer);


        /* open and close tile map */

        FILE *p = fopen("maps/tilemap.txt", "rw+");
        if (p == NULL)
	        puts("Error opening tilemap");
    
        MapSize dimensions = mapDimensions(p);
        if (&dimensions.x == NULL || &dimensions.y == NULL) {
	        puts("Failed to load map dimensions");
	        return 1;
        }

        MapTile map[dimensions.y][dimensions.x];

        if (!mapReader(p, dimensions, map, gRenderer, x, y)) {
	        puts("error reading map");
	        return 1;
        }

        fclose(p);
        puts("Media loaded");
        cleanBuffer();
    
        /* print map and player starting position */
        SDL_RenderCopy(gRenderer, mapBackground, NULL, backRect); 
        puts("debug 2.5"); 
        if (!mapDraw(gRenderer, dimensions, map, x, y)) {
	        puts("Tile are empty in mapMaker()");
	        return 1;
        }	
        puts("debug 3");
        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_DEFAULT];
        gEnemyTexture   = mapTiles[6];
        placeObject(gRenderer, gCurrentTexture, toIsometric(playerRect));
  

        /*
         * use for tracking player position in the map as player doesn't actually move
         * purely representative of actual movement for sake of bounds checking and 
         * moving the map around the player
        */ 
        SDL_Rect playerPositionTracker;
        playerPositionTracker.w = 128;
        playerPositionTracker.h = 64;
        playerPositionTracker.x = playerRect->x-128;
        playerPositionTracker.y = playerRect->y-192;
       
        SDL_Rect enemyPositionTracker;
        enemyPositionTracker.w = enemyRect->w;
        enemyPositionTracker.h = enemyRect->h;
        enemyPositionTracker.x = enemyRect->x+256;
        enemyPositionTracker.y = enemyRect->y-256; 

        /* 
         * the temp rect is for sending in values into boundaryCheck without
         * modifying playerPosition until after the blocked/not-blocked results
         * come back
         */

        SDL_Rect temp;  
        temp.y = playerRect->y;
        temp.x = playerRect->x; 

        boundaryCheck(enemyPositionTracker, dimensions, map);
        
        while (!quit) {        
	        int direction[4] = { 0, 0, 0, 0 };
                int *xPtr = &x;
                int *yPtr = &y;
                SDL_Texture **currentTexturePtr = &gCurrentTexture;
                SDL_Texture **keyPressTexturePtr = &gKeyPressTextures[0];
                SDL_Rect *playerPosTrackPtr = &playerPositionTracker;
                SDL_Rect *enemyPosTrackPtr  = &enemyPositionTracker;
                while (SDL_PollEvent(&e) != 0) {
		        if (e.type == SDL_QUIT) 
			        quit = 1;
	        }
                
                SDL_RenderClear(gRenderer);

	        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
                
                puts(" ");
               
                /* assign direction */

                if (currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_UP]) {
                        direction[UP] = 1;
                }
                if (currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_DOWN]) {
                        direction[DOWN] = 1;
                }
                if (currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT]) {
                        direction[LEFT] = 1;
                }
                if (currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT]) {
                        direction[RIGHT] = 1;
                }
                
                playerMovement(playerPosTrackPtr, enemyPosTrackPtr, enemyRect, direction, xPtr, yPtr, dimensions, map, currentTexturePtr, &keyPressTexturePtr);
        
                if (currentKeyStates[SDL_SCANCODE_SPACE]) {
                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_JUMP];
                        int jumpStart = playerPositionTracker.y / playerPositionTracker.x;
                        int jumpCeiling = jumpStart + 31;
                        for (int i = 0; jumpStart < jumpCeiling; jumpStart++) {
                                i += 4; 
                                x += 64-i;
                                y += 64-i;
                              //  enemyRect->x += 64-i;
                                enemyRect->y += 64-i;
                                               
                                drawScreen(gRenderer, backRect, playerRect, enemyRect, gCurrentTexture, mapTiles, mapBackground, dimensions, map, xPtr, yPtr);
                        }
                } else if (currentKeyStates[SDL_SCANCODE_LSHIFT]) {
                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_SHIFT];
                } else if (currentKeyStates[SDL_SCANCODE_RSHIFT]) {
       
                } else {
                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_DEFAULT];
	        }	        
                
		/* clear screen to redraw map */   
                drawScreen(gRenderer, backRect, playerRect, enemyRect,  gCurrentTexture, mapTiles, mapBackground, dimensions, map, xPtr, yPtr);
                
                
        }                

            
        /* cleanup and exit */
        SDL_DestroyTexture(gCurrentTexture);
        free(tileRect);
        free(playerRect);
        free(backRect);
        free(enemyRect);
        end(window, &gKeyPressTextures[0], &mapTiles[0]);
        SDL_FreeSurface(screenSurface);
        screenSurface = NULL;
        puts("screenSurface free");
        return 0;
}
