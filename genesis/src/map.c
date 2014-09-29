#ifndef C_MAP
#define C_MAP
#include "../include/map.h"
#include "../include/newbsp.h"

void drawMap(){
	int y, x;
	for(y = WINSTARTY; y < WINENDX; y++){
		for(x = WINSTARTX; x < MAXWIDTH; x++){
			mvaddch(y, x, TILE_INDEX[MAP[CM( (y+VIEW->y - WINSTARTY), (MAPTAIL->tX+1), (x+VIEW->x) )].tT].tChar);
		}
	}
}

void fillMap(struct TILE *map, int sy, int sx, int tt){
	int y,x;
	for(y = 0; y < sy; y++){
		for(x = 0; x < sx; x++){
			map[CM(y,sx,x)].tX = x;
			map[CM(y,sx,x)].tY = y;
			map[CM(y,sx,x)].tT = tt;
		}
	}
}

void fillStairs(struct TILE *map, int sy, int sx, int ft){
	int y,x;
	if(genesis->floor == 1){
		do{
			y = getRand_i(0, sy);
			x = getRand_i(0, sx);
		}while(map[C(y, sx, x)].tT != ft);
		map[C(y,sx,x)].tT = TILE_STAIRSDOWN;
		return;
	}
	else if(genesis->floor == 99){
		do{
			y = getRand_i(0, sy);
			x = getRand_i(0, sx);
		}while(map[C(y,sx,x)].tT != ft);
		map[C(y,sx,x)].tT == TILE_STAIRSUP;
		return;
	}
	else{
		do{
			y = getRand_i(0, sy);
			x = getRand_i(0, sx);
		}while(map[C(y,sx,x)].tT != ft);
		map[C(y,sx,x)].tT = TILE_STAIRSDOWN;
		do{
			y = getRand_i(0,sy);
			x = getRand_i(0,sx);
		}while(map[C(y,sx,x)].tT != ft);
		map[C(y,sx,x)].tT = TILE_STAIRSUP;
		return;
	}
}

void fillWall(struct TILE *map, int sy, int sx, int ft, int wt){
	int y, x;
	for(y = 1; y < sy - 1; y++){
		for(x = 1; x < sx - 1; x++){
			if(map[CM(y,sx,x)].tT == TILE_BLANK){
				if(map[TL(y,sx,x)].tT == ft ||
				map[TM(y,sx,x)].tT == ft ||
				map[TR(y,sx,x)].tT == ft ||
				map[CL(y,sx,x)].tT == ft ||
				map[CR(y,sx,x)].tT == ft ||
				map[BL(y,sx,x)].tT == ft ||
				map[BM(y,sx,x)].tT == ft ||
				map[BR(y,sx,x)].tT == ft){
					map[CM(y,sx,x)].tT = wt;
				}
			}
		}
	}
}

int initMap(int sy, int sx, int nr, int ft, int wt){
	if(MAP != NULL){
		MAP = mapFree(MAP);
	}
	MAP = mapAllocate(MAP, sy, sx);
	if(MAP == NULL) return ERR_MALLOC;
	int y, x;
	BSPROOT = bspNew(BSPROOT, sy, sx, 4);
	if(BSPROOT == NULL) return ERR_MALLOC;
	bspRecursive(BSPROOT, nr);
	bspDoResize(BSPROOT, nr);
	fillMap(MAP, sy, sx, TILE_BLANK);
	MAPTAIL = &MAP[(sy * sx) - 1];
	bspDrawRooms(BSPROOT, MAP, nr, sy, sx, ft, wt);
	int r = nr - 1;
	for(r; r >= 0; r--){
		bspLinkRooms(BSPROOT, MAP, r, sy, sx, ft);
	}
	int d;
	bspAddDoors(BSPROOT, MAP, nr, sx, ft, wt);
	fillWall(MAP, sy, sx, ft, wt);
	fillStairs(MAP, sy, sx, ft);
	do{
		y = getRand_i(0, sy);
		x = getRand_i(0, sx);
	}while(MAP[CM(y,sx,x)].tT != ft);
	player->locY = y;
	player->locX = x;
	genesis->maxY = sy;
	genesis->maxX = sx;
	bspDel(BSPROOT);
	int i;
	for(i = 0; i < 25; i++){
		seedCreature(genesis->floor, ft);	// Keep this out until seedCreature is revised to use spawnCreatur()
	}
	return ERR_NONE;
}

struct TILE *mapAllocate(struct TILE *map, int sy, int sx){
	if(map != NULL){
		free(map);
		map = NULL;
	}
	map = (struct TILE *)malloc(sizeof(struct TILE) * (sy * sx));
	if(map == NULL) return NULL;
	return map;
}

struct TILE *mapFree(struct TILE *map){
	free(map);
	map = NULL;
//	map = NULL;
//	free(map);
	return map;
}

/*
*	Any time this is updated, make sure to update the enum
*	located in include/const.h
*/
struct TILE_TYPE TILE_INDEX[] = {
	{ '.', 1, true},		// (0) TILE_ROCKFLOOR
	{ '#', 1, false},		// (1) TILE_WALL
	{ '+', 1, false},		// (2) TILE_DOORCLOSED
	{ '/', 1, true},		// (3) TILE_DOOROPEN
	{ ' ', 1, false},		// (4) TILE_BLANK
	{ '>', 1, true},		// (5) TILE_STAIRSDOWN
	{ '<', 1, true},		// (6) TILE_STAIRSUP
};

#endif
