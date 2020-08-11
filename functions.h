#ifndef FUNCTIONS_H
#define FUNCTIONS_H

extern const int TOTAL_TILES;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern char *BACKGROUND;
extern char *PLAYER1;
extern char *PLAYER2;
extern char *TILE1;
extern char *TILE2;
extern char *TILE3;
extern char *TILE4;
extern char *TILE5;
extern char *TILE6;
extern char *ENEMY;
extern SDL_Window *window;

enum KeyPressSurfaces
{
        KEY_PRESS_SURFACE_DEFAULT,
        KEY_PRESS_SURFACE_UP,
        KEY_PRESS_SURFACE_DOWN,
        KEY_PRESS_SURFACE_LEFT,
        KEY_PRESS_SURFACE_RIGHT,
        KEY_PRESS_SURFACE_SHIFT,
        KEY_PRESS_SURFACE_JUMP,
        KEY_PRESS_TEXTURE_TOTAL
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

/* function prototypes */

SDL_Window *init(SDL_Renderer **gRenderer);
_Bool loadMedia(SDL_Texture **gKeyPressTextures, SDL_Renderer **gRenderer, SDL_Texture **mapTiles);
SDL_Texture *loadTexture(char *somePath, SDL_Renderer **gRenderer);
MapSize mapDimensions(FILE *p);
_Bool mapReader(FILE *p, MapSize dimensions, int map[][dimensions.x]);
void cleanBuffer();
_Bool mapDraw(SDL_Renderer *gRenderer, SDL_Rect *tileRect, SDL_Texture **mapTiles, MapSize dimensions, int map[][dimensions.x], int x, int y);
void placeObject(SDL_Renderer *gRenderer, SDL_Texture *objectTexture, SDL_Rect *objectRect);
SDL_Rect *toIsometric(SDL_Rect *cartRect);
void enemyMovement(SDL_Rect *someEnemy);
int playerMovement(SDL_Rect *playerPositionTracker, SDL_Rect *enemyPositionTracker, SDL_Rect *enemyRect, int direction[4], int *x, int *y, MapSize dimensions, int map[][dimensions.x], SDL_Texture **gCurrentTexture, SDL_Texture **gKeyPressTextures[KEY_PRESS_TEXTURE_TOTAL]);
_Bool boundaryCheck(SDL_Rect someObject, MapSize dimensions, int map[][dimensions.x]);
void drawScreen(SDL_Renderer *gRenderer, SDL_Rect *backRect, SDL_Rect *tileRect, SDL_Rect *playerRect, SDL_Rect *enemyRect, SDL_Texture *gCurrentTexture, SDL_Texture *mapTiles[7], SDL_Texture *mapBackground, MapSize dimensions, int map[][dimensions.x], int *x, int *y);
void end(SDL_Window *window, SDL_Texture **gKeyPressTextures, SDL_Texture **mapTiles);


#endif
