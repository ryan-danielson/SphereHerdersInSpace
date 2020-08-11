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
