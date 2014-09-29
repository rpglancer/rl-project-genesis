#ifndef C_MAP
#define C_MAP
//#include "../include/bsp.h"
#include "../include/map.h"
#include "../include/util.h"
#include <unistd.h>

int bY, bX, bH, bW;

void drawMap(int y, int x){
	int xx, yy;
	xx = yy = 0;
	struct TILE BUFFER[(MAPTAIL->tY + 1) * (MAPTAIL->tX + 1)];
	MAPCURRENT = MAPROOT;
	for(yy = 0; yy <= MAPTAIL->tY; yy++){
		for(xx = 0; xx <= MAPTAIL->tX; xx++){
			BUFFER[yy * MAPTAIL->tX + xx] = *MAPCURRENT;
			MAPCURRENT = MAPCURRENT->next;
		}
	}
	for(yy = 0; yy < MAXHEIGHT; yy++){
		for(xx = 0; xx < MAXWIDTH; xx++){
			mvaddch(yy, xx, TILE_INDEX[BUFFER[(yy + VIEW.y) * MAPTAIL->tX + (xx + VIEW.x)].tT].tChar);
		}
	}
}

struct TILE TEST[100 * 100];

/*
bool mapCallback(BSP *node, void *data){
//	if(node->level == 3){
	if(bspIsLeaf(node)){
		int y, x;
		for(y = node->y + 1; y < node->y + node->h-1; y++){
			for(x = node->x + 1; x < node->x + node->w; x++){
				TEST[y * 100 + x].tT = TILE_ROCKFLOOR;
			}
		}
	}
	return true;
}
*/
/*
int initMap(int sy, int sx){
	seedRand();
	BSP *test = bspNewSized(0,0,sy,sx);
	bspSplitRecursive(test,5,5,5);
	struct TILE BUFFER[sy * sx];
	int y, x;
	for(y = 0; y < sy; y++){
		for(x = 0; x < sx; x++){
			TEST[y * sx + x].tY = y;
			TEST[y * sx + x].tX = x;
			TEST[y * sx + x].tT = TILE_WALL;
		}
	}
	bspTraverseIn(test, mapCallback, (void *)5);
	MAPROOT = (struct TILE*)malloc(sizeof(struct TILE));
	if(MAPROOT == NULL) return ERR_MALLOC;
	MAPCURRENT = MAPROOT;
	for(y = 0; y < sy; y++){
		for(x = 0; x < sx; x++){
			MAPCURRENT->tX = TEST[y * sx + x].tX;
			MAPCURRENT->tY = TEST[y * sx + x].tY;
			MAPCURRENT->tT = TEST[y * sx + x].tT;
			if(y == sy - 1 && x == sx - 1){
				MAPTAIL = MAPCURRENT;
				return ERR_NONE;
			}
			else{
				MAPCURRENT->next = (struct TILE *)malloc(sizeof(struct TILE));
				if(MAPCURRENT->next == NULL) return ERR_MALLOC;
				MAPCURRENT = MAPCURRENT->next;
			}
		}
	}
	return ERR_NONE;
}
*/

int initMap(int sx, int sy){
	seedRand();
	BSP *test = bspNewSized(0, 0, sy, sx);
	bspSplitRecursive(test, 5, 5, 5);
	MAPROOT = (struct TILE *)malloc(sizeof(struct TILE));
	if(MAPROOT == NULL) return ERR_MALLOC;
	MAPCURRENT = MAPROOT;
	int x, y;
	for(y = 0; y < sy; y++){
		for(x = 0; x < sx; x++){
			MAPCURRENT->tX = x;
			MAPCURRENT->tY = y;
			if(MAPCURRENT->tX == 0 || MAPCURRENT->tX == sx - 1 ||
				MAPCURRENT->tY == 0 || MAPCURRENT->tY == sy - 1){
				MAPCURRENT->tT = TILE_WALL;
			}
			else{
				MAPCURRENT->tT = TILE_ROCKFLOOR;
			}
			if(y == sy - 1 && x == sx - 1){
				MAPTAIL = MAPCURRENT;
				return ERR_NONE;
			}
			else{
				MAPCURRENT->next = (struct TILE *)malloc(sizeof(struct TILE));
				if(MAPCURRENT->next == NULL) return ERR_MALLOC;
				MAPCURRENT = MAPCURRENT->next;
			}
		}
	}
	return ERR_NONE;
}

/*
*	Any time this is updated, make sure to update the enum
*	located in include/const.h
*/
struct TILE_TYPE TILE_INDEX[] = {
	{ '.', 7, true},		// (0) TILE_ROCKFLOOR
	{ '#', 7, true},		// (1) TILE_WALL
	{ '+', 6, false},		// (2) TILE_DOORCLOSED
	{ '/', 8, true},		// (3) TILE_DOOROPEN
};

/*
*	Coordinates of the active view port.
*/
struct RECT VIEW = {0,0};

#endif
