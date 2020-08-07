/* Using SDL and standard IO */
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

/* screen dimension constants */
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

/* surface images */

char *BACKGROUND  = "tiles/background.png";
char *PLAYER1     = "tiles/player.png";
char *PLAYER2     = "tiles/player2.png";
char *TILE1       = "tiles/tile4.png";
char *TILE2	  = "tiles/tile5.png";
char *TILE3	  = "tiles/tile6.png";
char *TILE4       = "tiles/tile3.png";
char *TILE5       = "tiles/tile7.png";
char *TILE6       = "tiles/tile11.png";
char *ENEMY       = "tiles/enemy.png";

const int TOTAL_TILES = 7;

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,	// 0
	KEY_PRESS_SURFACE_UP,		// 1
	KEY_PRESS_SURFACE_DOWN,		// 2
	KEY_PRESS_SURFACE_LEFT,		// 3
	KEY_PRESS_SURFACE_RIGHT,	// 4
        KEY_PRESS_SURFACE_SHIFT,        // 5
        KEY_PRESS_SURFACE_JUMP,         // 6
	KEY_PRESS_TEXTURE_TOTAL		// 7
};

enum State
{
	STATE_MENU,
	STATE_PLAYER_MOVEMENT
};

enum Direction
{
        LEFT,
        UP,
        DOWN,
        RIGHT,
        LEFT_UP,
        LEFT_DOWN,
        RIGHT_UP,
        RIGHT_DOWN
};

struct MapSize
{
	int x;
	int y;
};
typedef struct MapSize MapSize;

SDL_Window *window;

SDL_Window *init(SDL_Renderer **gRenderer);
_Bool loadMedia(SDL_Texture **gKeyPressTextures, SDL_Renderer **gRenderer,  SDL_Texture **mapTiles);
SDL_Texture *loadTexture(char *somePath, SDL_Renderer **gRenderer);
void updatePlayer(SDL_Window **window, SDL_Surface *screenSurface, SDL_Texture *gCurrentTexture, SDL_Rect playerRect);
_Bool mapDraw(SDL_Renderer *gRenderer, SDL_Rect *tileRect, SDL_Texture **mapTiles, MapSize dimensions, int map[][dimensions.x], int x, int y);
void placeObject(SDL_Renderer *gRenderer, SDL_Texture *objectTexture, SDL_Rect *objectRect);
SDL_Rect *toIsometric(SDL_Rect *cartRect);
MapSize mapDimensions(FILE *p);
_Bool mapReader(FILE *p, MapSize dimensions, int map[][dimensions.x]);
void cleanBuffer();
_Bool boundaryCheck(SDL_Rect someObject, MapSize dimensions, int map[][dimensions.x]);
void drawScreen(SDL_Renderer *gRenderer, SDL_Rect *backRect, SDL_Rect *tileRect, SDL_Rect *playerRect, SDL_Rect *enemyRect, SDL_Texture *gCurrentTexture, 
           SDL_Texture *mapTiles[TOTAL_TILES], SDL_Texture *mapBackground, MapSize dimensions, int map[][dimensions.x], int *x, int *y);
void enemyMovement(SDL_Rect *someEnemy);
int playerMovement(SDL_Rect *playerPositionTracker, SDL_Rect *enemyPositionTracker, SDL_Rect *enemyRect, int direction[4], int *x, int *y, MapSize dimensions, int map[][dimensions.x], SDL_Texture **gCurrentTexture, SDL_Texture **gKeyPressTextures[KEY_PRESS_TEXTURE_TOTAL]);
void end(SDL_Window *window, SDL_Texture **gKeyPressTextures, SDL_Texture **mapTiles);


SDL_Window*
init(SDL_Renderer **gRenderer)
{
        /* Initialize SDL */
        if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
                printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
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
                        } else {
                                SDL_SetRenderDrawColor(*gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	                        int imgFlags = IMG_INIT_PNG;
	                        if (!(IMG_Init(imgFlags) & imgFlags)) {
		                        printf("SDL_image could not initilize! SDL_image error: %s\n", IMG_GetError());
	    	                        return 0;
	                        }
                        }
	    	return window;
	    
                }
        }
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

MapSize 
mapDimensions(FILE *p)
{
	MapSize dimension;

	fscanf(p, "%d %d", &dimension.y, &dimension.x);
	printf("MapSize: %d x %d\n", dimension.x, dimension.y);

	return dimension;
}

_Bool
mapReader(FILE *p, MapSize dimensions, int map[][dimensions.x])
{
	for (int i = 0; i < dimensions.y; ++i) {
		for (int k = 0; k < dimensions.x; ++k) {
			fscanf(p, "%d", &map[i][k]);
			printf("%d ", map[i][k]);
		}
		puts(" ");
	}
	return 1;
}

void
cleanBuffer()
{
	char ch;
	while (ch != '\0' && ch != EOF)
		ch = getchar();
}

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
enemyMovement(SDL_Rect *someEnemy)
{
//        someEnemy->x += 2;
//        someEnemy->y += 1;
}

int
playerMovement(SDL_Rect *playerPositionTracker, SDL_Rect *enemyPositionTracker, SDL_Rect *enemyRect, int direction[4], int *x, int *y, MapSize dimensions, int map[][dimensions.x], SDL_Texture **gCurrentTexture, SDL_Texture **gKeyPressTextures[KEY_PRESS_TEXTURE_TOTAL])
{
        printf("  %d\n%d   %d\n  %d\n\n", direction[1], direction[0], direction[3], direction[2]);               
	       
        /* check direction values and then check boundaries of that move */
        SDL_Rect tempPlayer;
        tempPlayer.x = SCREEN_WIDTH/2;
        tempPlayer.y = 256;        
        tempPlayer.h = 64;
        tempPlayer.w = 128;
        
        SDL_Rect tempEnemy;
        tempEnemy.x = SCREEN_WIDTH/2;
        tempEnemy.y = 256;
        tempEnemy.h = 64;
        tempEnemy.w = 128;
        if (direction[LEFT] == 1 && direction[UP] == 1) {
                tempPlayer.x = playerPositionTracker->x-32;
                tempPlayer.y = playerPositionTracker->y-16;
                tempEnemy.x = enemyPositionTracker->x+32;
                tempEnemy.y = enemyPositionTracker->y+16;
        } else if (direction[LEFT] == 1 && direction[DOWN] == 1) {
                tempPlayer.x = playerPositionTracker->x-32;
                tempPlayer.y = playerPositionTracker->y+16;
                tempEnemy.x = enemyPositionTracker->x+32;
                tempEnemy.y = enemyPositionTracker->y-16;
        } else if (direction[RIGHT] == 1 && direction[UP] == 1) {
                tempPlayer.x = playerPositionTracker->x+32;
                tempPlayer.y = playerPositionTracker->y-16;
                tempEnemy.x = enemyPositionTracker->x-32;
                tempEnemy.y = enemyPositionTracker->y+16;
        } else if (direction[RIGHT] == 1 && direction[DOWN] == 1) {
                tempPlayer.x = playerPositionTracker->x+32;
                tempPlayer.y = playerPositionTracker->y+16;
                tempEnemy.x = enemyPositionTracker->x-32;
                tempEnemy.y = enemyPositionTracker->y-16;
        } else {
                if (direction[LEFT] == 1) {
                    tempPlayer.x = playerPositionTracker->x-32;
                    tempPlayer.y = playerPositionTracker->y;
                    tempEnemy.x = enemyPositionTracker->x+32;
                    tempEnemy.y = enemyPositionTracker->y;
                }
                if (direction[UP] == 1) {
                    tempPlayer.x = playerPositionTracker->x;
                    tempPlayer.y = playerPositionTracker->y-16;
                    tempEnemy.x = enemyPositionTracker->x;
                    tempEnemy.y = enemyPositionTracker->y+16;
                }
                if (direction[DOWN] == 1) {
                    tempPlayer.x = playerPositionTracker->x;
                    tempPlayer.y = playerPositionTracker->y+16;
                    tempEnemy.x = enemyPositionTracker->x;
                    tempEnemy.y = enemyPositionTracker->y-16;
                }
                if (direction[RIGHT] == 1) {
                    tempPlayer.x = playerPositionTracker->x+32;
                    tempPlayer.y = playerPositionTracker->y;
                    tempEnemy.x = enemyPositionTracker->x-32;
                    tempEnemy.y = enemyPositionTracker->y;
                }
        }

        /* move map and adjust player image according to direction */

        if (boundaryCheck(tempPlayer, dimensions, map)) {
                if (direction[LEFT] == 1) {
                        *x += 16; 
                        playerPositionTracker->x -= 32;
                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_LEFT];
                        enemyRect->x += 32;
                        if (boundaryCheck(tempEnemy, dimensions, map)) {
                                enemyPositionTracker->x -= 16; 
                                enemyRect->x += 32;
                        }

                }
                if (direction[UP] == 1) {
                        *y += 16;
                        playerPositionTracker->y -= 16;
                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_UP];
                        enemyRect->y += 16;
                        if (boundaryCheck(tempEnemy, dimensions, map)) {
                                enemyPositionTracker->y += 16;
                                enemyRect->y += 16;
                        }

                } 
                if (direction[DOWN] == 1) {
                        *y -= 16;
                        playerPositionTracker->y += 16;
                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_DOWN];
                        enemyRect->y -= 16;
                        if (boundaryCheck(tempEnemy, dimensions, map)) {
                                enemyPositionTracker->y -= 16;
                                enemyRect->y -= 16;
                        }
                } 
                if (direction[RIGHT] == 1) {
                        *x -= 16;
                        playerPositionTracker->x += 32;
                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_RIGHT];
                        enemyRect->x -= 32;
                        if (boundaryCheck(tempEnemy, dimensions, map)) {
                                enemyPositionTracker->x -= 16;
                                enemyRect->x -= 32;
                        }

                }

                return 1; 

        /* this section of the statement is to determine if the player is able to go one direction
         * if two keys are pressed. That is, if the player is pressing up-left, check both up and left
         * independently in case onie direction viable. Not pretty/elegant, probably needs refactoring.
         */

        } else if (!boundaryCheck(tempPlayer, dimensions, map)) {
                if (direction[UP] == 1 && direction[LEFT] == 1) {
                        /* check up */
                        tempPlayer.x = playerPositionTracker->x;
                        tempPlayer.y = playerPositionTracker->y-16;
                        if (boundaryCheck(tempPlayer, dimensions, map)) {
                                *y += 16;
                                enemyPositionTracker->y += 16;
                                playerPositionTracker->y -= 16;
                                gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_UP];
                                return 1;
                        } else {
                                /* check left */
                                tempPlayer.x = playerPositionTracker->x-32;
                                tempPlayer.y = playerPositionTracker->y;
                                if (boundaryCheck(tempPlayer, dimensions, map)) {
                                        *x += 16;
                                        enemyPositionTracker->x += 16;
                                        playerPositionTracker->x -= 32;
                                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_LEFT];
                                        return 1;
                                }
                        }
                }
                if (direction[UP] == 1 && direction[RIGHT] == 1) {
                        /* check up */
                        tempPlayer.x = playerPositionTracker->x;
                        tempPlayer.y = playerPositionTracker->y-16;
                        if (boundaryCheck(tempPlayer, dimensions, map)) {
                                *y += 16;
                                enemyPositionTracker->y += 16;
                                playerPositionTracker->y -= 16;
                                gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_UP];
                                return 1;
                        } else {
                                /* check right */
                                tempPlayer.x = playerPositionTracker->x+32;
                                tempPlayer.y = playerPositionTracker->y;
                                if (boundaryCheck(tempPlayer, dimensions, map)) {
                                        *x -= 16;
                                        enemyPositionTracker->x -= 16;
                                        playerPositionTracker->x += 32;
                                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_RIGHT];
                                        return 1;
                                }
                        }
                }
                if (direction[DOWN] == 1 && direction[LEFT] == 1) {
                        /* check down */
                        tempPlayer.x = playerPositionTracker->x;
                        tempPlayer.y = playerPositionTracker->y+16;
                        if(boundaryCheck(tempPlayer, dimensions, map)) {
                                *y -= 16;
                                enemyPositionTracker->y -= 16;
                                playerPositionTracker->y += 16;
                                gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_DOWN];
                                return 1;
                        } else {
                                /* check left */
                                tempPlayer.x = playerPositionTracker->x-32;
                                tempPlayer.y = playerPositionTracker->y;
                                if (boundaryCheck(tempPlayer, dimensions, map)) {
                                        *x += 16;
                                        enemyPositionTracker->x += 16;
                                        playerPositionTracker->x -= 32;
                                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_LEFT];
                                        return 1;
                                }
                        }
                }
                if (direction[DOWN] == 1 && direction[RIGHT] == 1) {
                        /* check down */
                        tempPlayer.x = playerPositionTracker->x;
                        tempPlayer.y = playerPositionTracker->y+16;
                        if(boundaryCheck(tempPlayer, dimensions, map)) {
                                *y -= 16;
                                enemyPositionTracker->y -= 16;
                                playerPositionTracker->y += 16;
                                gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_DOWN];
                                return 1;
                        } else {
                                /* check right */
                                tempPlayer.x = playerPositionTracker->x+32;
                                tempPlayer.y = playerPositionTracker->y;
                                if (boundaryCheck(tempPlayer, dimensions, map)) {
                                        *x -= 16;
                                        enemyPositionTracker->x -= 16;
                                        playerPositionTracker->x += 32;
                                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_RIGHT];
                                        return 1;
                                }
                        }
                }

        } else {
                return 0;
        }
}

_Bool
boundaryCheck(SDL_Rect someObject, MapSize dimensions, int map[][dimensions.x])
{
	printf("playerRect[%d][%d], map: %d\n", someObject.y/64, someObject.x/128, map[someObject.y/64][someObject.x/128]);

	if (someObject.x < 0 || someObject.x/128 >= dimensions.x || someObject.y/64 < 0 || someObject.y/64 >= dimensions.y || map[someObject.y/64][someObject.x/128] != 0)
		return 0;
	puts("No boundary");
	return 1;
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

        int map[dimensions.y][dimensions.x];

        if (!mapReader(p, dimensions, map)) {
	        puts("error reading map");
	        return 1;
        }

        fclose(p);
        puts("Media loaded");
        cleanBuffer();
    
        /* print map and player starting position */
        SDL_RenderCopy(gRenderer, mapBackground, NULL, backRect); 
    
        if (!mapDraw(gRenderer, tileRect, mapTiles, dimensions, map, x, y)) {
	        puts("Tile are empty in mapMaker()");
	        return 1;
        }	

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
        enemyPositionTracker.w = 128;
        enemyPositionTracker.h = 64;
        enemyPositionTracker.x = enemyRect->x-128;
        enemyPositionTracker.y = enemyRect->y-192; 

        /* 
         * the temp rect is for sending in values into boundaryCheck without
         * modifying playerPosition until after the blocked/not-blocked results
         * come back
         */

        SDL_Rect temp;  
        temp.y = playerRect->y;
        temp.x = playerRect->x; 

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
                /* [0] = left
                 * [1] = up
                 * [2] = down
                 * [3] = right
                 */

               
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
                                               
                                drawScreen(gRenderer, backRect, tileRect, playerRect, enemyRect, gCurrentTexture, mapTiles, mapBackground, dimensions, map, xPtr, yPtr);
                        }
                } else if (currentKeyStates[SDL_SCANCODE_LSHIFT]) {
                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_SHIFT];
                } else if (currentKeyStates[SDL_SCANCODE_RSHIFT]) {
       
                } else {
                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_DEFAULT];
	        }	        
                
		/* clear screen to redraw map */   
                drawScreen(gRenderer, backRect, tileRect, playerRect, enemyRect,  gCurrentTexture, mapTiles, mapBackground, dimensions, map, xPtr, yPtr);
                
                
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
