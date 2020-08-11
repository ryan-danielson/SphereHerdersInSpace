#include <SDL.h>
#include <SDL_image.h>
#include "functions.h"
#include <stdio.h>


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
        tempEnemy.x = enemyPositionTracker->x;
        tempEnemy.y = enemyPositionTracker->y;
        tempEnemy.h = enemyPositionTracker->h;
        tempEnemy.w = enemyPositionTracker->w;
        if (direction[LEFT] == 1 && direction[UP] == 1) {
                tempPlayer.x = playerPositionTracker->x-32;
                tempPlayer.y = playerPositionTracker->y-16;
          //      tempEnemy.x = enemyPositionTracker->x+32;
          //      tempEnemy.y = enemyPositionTracker->y+16;
        } else if (direction[LEFT] == 1 && direction[DOWN] == 1) {
                tempPlayer.x = playerPositionTracker->x-32;
                tempPlayer.y = playerPositionTracker->y+16;
                tempEnemy.x = enemyPositionTracker->x+32;
          //      tempEnemy.y = enemyPositionTracker->y-16;
        } else if (direction[RIGHT] == 1 && direction[UP] == 1) {
                tempPlayer.x = playerPositionTracker->x+32;
                tempPlayer.y = playerPositionTracker->y-16;
          //      tempEnemy.x = enemyPositionTracker->x-32;
          //      tempEnemy.y = enemyPositionTracker->y+16;
        } else if (direction[RIGHT] == 1 && direction[DOWN] == 1) {
                tempPlayer.x = playerPositionTracker->x+32;
                tempPlayer.y = playerPositionTracker->y+16;
        //        tempEnemy.x = enemyPositionTracker->x-32;
        //        tempEnemy.y = enemyPositionTracker->y-16;
        } else {
                if (direction[LEFT] == 1) {
                    tempPlayer.x = playerPositionTracker->x-32;
                    tempPlayer.y = playerPositionTracker->y;
          //          tempEnemy.x = enemyPositionTracker->x+32;
          //          tempEnemy.y = enemyPositionTracker->y;
                }
                if (direction[UP] == 1) {
                    tempPlayer.x = playerPositionTracker->x;
                    tempPlayer.y = playerPositionTracker->y-16;
          //          tempEnemy.x = enemyPositionTracker->x;
          //          tempEnemy.y = enemyPositionTracker->y+16;
                }
                if (direction[DOWN] == 1) {
                    tempPlayer.x = playerPositionTracker->x;
                    tempPlayer.y = playerPositionTracker->y+16;
          //          tempEnemy.x = enemyPositionTracker->x;
          //          tempEnemy.y = enemyPositionTracker->y-16;
                }
                if (direction[RIGHT] == 1) {
                    tempPlayer.x = playerPositionTracker->x+32;
                    tempPlayer.y = playerPositionTracker->y;
          //          tempEnemy.x = enemyPositionTracker->x-32;
          //          tempEnemy.y = enemyPositionTracker->y;
                }
        }

        /* move map and adjust player image according to direction */

        if (boundaryCheck(tempPlayer, dimensions, map)) {
                if (direction[LEFT] == 1) {
                        *x += 16; 
                        playerPositionTracker->x -= 32;
                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_LEFT];
                        enemyRect->x += 16;
                        enemyRect->y += 8;
                        if (boundaryCheck(tempEnemy, dimensions, map)) {
                                //enemyPositionTracker->x -= 16; 
                                //enemyRect->x += 32;
                        }

                }
                if (direction[UP] == 1) {
                        *y += 16;
                        playerPositionTracker->y -= 16;
                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_UP];
                        enemyRect->y += 16;
                        enemyRect->x -= 16;
                        if (boundaryCheck(tempEnemy, dimensions, map)) {
                        //        enemyPositionTracker->y += 16;
                        //        enemyRect->y += 16;
                        }

                } 
                if (direction[DOWN] == 1) {
                        *y -= 16;
                        playerPositionTracker->y += 16;
                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_DOWN];
                        enemyRect->y -= 16;
                        enemyRect->x += 16;
                        if (boundaryCheck(tempEnemy, dimensions, map)) {
                        //        enemyPositionTracker->y -= 16;
                        //        enemyRect->y -= 16;
                        }
                } 
                if (direction[RIGHT] == 1) {
                        *x -= 16;
                        playerPositionTracker->x += 32;
                        gCurrentTexture = gKeyPressTextures[KEY_PRESS_SURFACE_RIGHT];
                        enemyRect->y -= 8;
                        enemyRect->x -= 16;
                /*        if (boundaryCheck(tempEnemy, dimensions, map)) {
                                enemyPositionTracker->x -= 16;
                                enemyRect->x -= 32;
                        }*/

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

        }
        return 0;
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
