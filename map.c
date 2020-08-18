#include <SDL.h>
#include <SDL_image.h>
#include "functions.h"

MapSize 
mapDimensions(FILE *p)
{
	MapSize dimension;

	fscanf(p, "%d %d", &dimension.y, &dimension.x);
	printf("MapSize: %d x %d\n", dimension.x, dimension.y);

	return dimension;
}

_Bool
mapReader(FILE *p, MapSize dimensions, MapTile map[][dimensions.x], SDL_Renderer *gRenderer, int x, int y)
{
        int temp = 0;
	for (int i = 0; i < dimensions.y; ++i) {
		for (int k = 0; k < dimensions.x; ++k) {
			fscanf(p, "%d", &temp);
			printf("%d ", temp);
                        map[i][k].rect.w = 128;
                        map[i][k].rect.h = 64;
                        map[i][k].rect.x = (k*64) + x;
                        map[i][k].rect.y = (i*64) + y;


                        switch (temp) {
                        case 0:
                                map[i][k].texture = loadTexture(TILE1, &gRenderer);
                                map[i][k].boundary = 0;
                                break;
                        case 1:
                                map[i][k].texture = loadTexture(TILE2, &gRenderer);
                                map[i][k].boundary = 1;
                                break;
                        case 2:
                                map[i][k].texture = loadTexture(TILE3, &gRenderer);
                                map[i][k].boundary = 1;
                                break;
                        case 3:
                                map[i][k].texture = loadTexture(TILE4, &gRenderer);
                                map[i][k].boundary = 1;
                                break;
                        case 4:
                                map[i][k].texture = loadTexture(TILE5, &gRenderer);
                                map[i][k].boundary = 1;
                                break;
                        case 5:  
                                map[i][k].texture = loadTexture(TILE6, &gRenderer);
                                map[i][k].boundary = 1;
                                break;
                        case 6:  
                                map[i][k].texture = loadTexture(ENEMY, &gRenderer);
                                map[i][k].boundary = 1;
                                break;
                        case 7:
                                map[i][k].texture = loadTexture(TILE4, &gRenderer);
                                map[i][k].boundary = 1;
                                break;
                        case 8:
                                map[i][k].texture = loadTexture(BLANK, &gRenderer);
                                map[i][k].boundary = 0;
                                break;
                        default: 
                                break;
                        }
                        
		}
		puts(" ");
	}
	return 1;
}
