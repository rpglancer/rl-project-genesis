#ifndef C_NEWBSP
#define C_NEWBSP
#include "../include/newbsp.h"

/*
*	Somewhere in this mess there needs to be added a check
*	for minimum room size. There is the possibility of a 0
*	room size which is causing hangs on map generation.
*/

BSP *bspNew(BSP *bsp, int height, int width, int depth){
	if(bsp == NULL){
		bsp = (BSP *)calloc(1, sizeof(BSP));
		if(bsp == NULL) return NULL;
	}
	bsp->level = 0;
	bsp->y = 0;
	bsp->x = 0;
	bsp->h = height;
	bsp->w = width;
	bsp->father = NULL;
	bsp->left = NULL;
	bsp->right = NULL;
	return bsp;
}

BSP *bspResize(BSP *bsp, float hPct, float wPct){
	if(bspIsLeaf(bsp)){
		int width = bsp->w - bsp->x;
		int height = bsp->h - bsp->y;
		width = width / wPct / 2;
		height = height / hPct / 2;
		if(width <= 0) width = bsp->w - bsp->x;
		if(height <=0) height = bsp->h - bsp->y;
		bsp->y = bsp->y + (rand() % height);
		bsp->h = bsp->h - (rand() % height);
		bsp->x = bsp->x + (rand() % width);
		bsp->w = bsp->w - (rand() % width);
	}
	return bsp;
}

void bspSplit(BSP *bsp, int direction, int position){
	bsp->left = (BSP *)calloc(1, sizeof(BSP));
	bsp->left->father = bsp;
	bsp->left->level = bsp->level + 1;
	bsp->right = (BSP *)calloc(1, sizeof(BSP));
	bsp->right->father = bsp;
	bsp->right->level = bsp->level + 1;
	if(direction > 1 && direction < 0){
		direction = rand() %2;
	}
	if(direction == 0){		// (0) Vertical Split
		bsp->left->x = bsp->x;
		bsp->left->w = position;
		bsp->left->y = bsp->y;
		bsp->left->h = bsp->h;
		bsp->right->x = position;
		bsp->right->w = bsp->w;
		bsp->right->y = bsp->y;
		bsp->right->h = bsp->h;
	}
	else if(direction == 1){	// (1) Horizontal Split
		bsp->left->x = bsp->x;
		bsp->left->w = bsp->w;
		bsp->left->y = bsp->y;
		bsp->left->h = position;
		bsp->right->x = bsp->x;
		bsp->right->w = bsp->w;
		bsp->right->y = position;
		bsp->right->h = bsp->h;
	}
}

void bspRecursive(BSP *bsp, int depth){
	int horiz, position;
	if(depth == 0)return;
	if(bsp->h - bsp->y > bsp->w - bsp->x){
		horiz = 1;
	}
	else if(bsp->w - bsp->x > bsp->h - bsp->y){
		horiz = 0;
	}
	else{
		horiz = rand() %2;
	}
/*	I'm pretty sure this is working, try not to fuck with it.	*/
	if(horiz == 0){
		position = bsp->w/2 + bsp->x/2;
		position = getRand_i( position *0.9, position *1.1);
	}
	else{
		position = bsp->h/2 + bsp->y/2;
		position = getRand_i( position *0.9, position *1.1);
	}
	bspSplit(bsp, horiz, position);
	bspRecursive(bsp->left, depth-1);
	bspRecursive(bsp->right, depth-1);
}

bool bspIsRoot(BSP *bsp){
	if(bsp->father == NULL)return true;
	return false;
}

bool bspIsLeaf(BSP *bsp){
	if(bsp->left == NULL && bsp->right == NULL)return true;
	return false;
}

bool bspDoResize(BSP *bsp, int level){
	if(bsp == NULL) return false;
	if(bsp->level == level){
		bsp = bspResize(bsp, 1.25, 1.25);
	}
	if(bsp->left != NULL && !bspDoResize(bsp->left, level)) return false;
	if(bsp->right != NULL && !bspDoResize(bsp->right, level)) return false;
	return true;
}
bool bspDrawRooms(BSP *bsp, struct TILE *map, int level, int sy, int sx, int ft, int wt){
	if(bsp == NULL || map == NULL) return false;
	if(bsp->level == level){
		int x, y;
		for(y = bsp->y; y < bsp->h; y++){
			for(x = bsp->x; x < bsp->w; x++){
				if(y == bsp->y || y == bsp->h - 1 || x == bsp->x || x == bsp->w - 1){
					map[CM(y,sx,x)].tT = wt;
				}
				else{
					map[CM(y,sx,x)].tT = ft;
				}
			}
		}
	}
	if(bsp->left != NULL && !bspDrawRooms(bsp->left, map, level, sy, sx, ft, wt)) return false;
	if(bsp->right != NULL && !bspDrawRooms(bsp->right, map, level, sy, sx, ft, wt)) return false;
	return true;
}

bool bspAddDoors(BSP *bsp, struct TILE *map, int level, int sx, int ft, int wt){
	OBJECTSTATS *toUse = NULL;
	toUse = getObjects(toUse, level, OBJ_DOOR);
	if(toUse == NULL) return false;
//	int count = sizeof(toUse) / sizeof(toUse[0]);
	size_t count = 0;
	OBJECTSTATS *c;
	for(c = toUse; c != NULL; c = c->next){
		count++;
	}
	if(bsp == NULL || map == NULL) return false;
	if(bsp->level == level){
		int y, x;
		for(y = bsp->y; y < bsp->h; y++){
			for(x = bsp->x; x < bsp->w; x++){
				if(map[CM(y,sx,x)].tT == ft && MAP[TM(y,sx,x)].tT == wt && MAP[BM(y,sx,x)].tT == wt ||
				map[CM(y,sx,x)].tT == ft && MAP[CL(y,sx,x)].tT == wt && MAP[CR(y,sx,x)].tT == wt){
					if(y == bsp->y || y == bsp->h - 1 || x == bsp->x || x == bsp->w - 1){
						int doorchance = getRand_i(1,10);
						if(doorchance <= 3){
							size_t random = getRand_i(0, count);
							for(c = toUse; random != 0; c = c->next, random--);
							ENTROOT = spawnObject(ENTROOT, c, level, y, x);
						}
					}
				}
			}
		}
	}
	for(toUse; toUse != NULL;){
		OBJECTSTATS *temp = toUse->next;
		free(toUse);
		toUse = NULL;
		toUse = temp;
	}
	if(bsp->left != NULL && !bspAddDoors(bsp->left, map, level, sx, ft, wt)) return false;
	if(bsp->right != NULL && !bspAddDoors(bsp->right, map, level, sx, ft, wt)) return false;
	return true;
}

void bspDigDown(struct TILE *map, int start, int end, int tx, int sx, int ft){
	int y;
	for(y = start; y <= end; y++){
		map[CM(y,sx,tx)].tT = TILE_ROCKFLOOR;
	}
}

void bspDigRight(struct TILE *map, int start, int end, int ty, int sx, int ft){
	int x;
	for(x = start; x <= end; x++){
		map[CM(ty,sx,x)].tT = TILE_ROCKFLOOR;
	}
}

void bspDigUp(struct TILE *map, int start, int end, int tx, int sx, int ft){
	int y;
	for(y = start; y >= end; y--){
		map[CM(y,sx,tx)].tT = TILE_ROCKFLOOR;
	}
}

void bspDigLeft(struct TILE *map, int start, int end, int ty, int sx, int ft){
	int x;
	for(x = start; x >= end; x--){
		map[CM(ty,sx,x)].tT = TILE_ROCKFLOOR;
	}
}
/*
*	This needs simplification.
*/
bool bspLinkRooms(BSP *bsp, struct TILE *map, int level, int sy, int sx, int ft){
	if(bsp == NULL || map == NULL) return false;
	if(bsp->level == level){
		int startY, endY, startX, endX, lengthY, lengthX, x, y;
		startY = endY = startX = endX = lengthY = lengthX = x = y = 0;
		do{
			startY = getRand_i( (bsp->left->y), (bsp->left->h) );
			endY = getRand_i( (bsp->right->y), (bsp->right->h) );
			startX = getRand_i( (bsp->left->x), (bsp->left->w) );
			endX = getRand_i( (bsp->right->x), (bsp->right->w) );
		}while (map[CM(startY,sx,startX)].tT != ft ||
			map[CM(endY,sx,endX)].tT != ft);
		lengthY = endY - startY;
		lengthX = endX - startX;
		if(lengthY < 0) lengthY = -lengthY;
		if(lengthX < 0) lengthX = -lengthX;
		// Dig horizontal
		if(bsp->right->x >= bsp->left->w){
			// Dig up
			if(startY > endY){
				bspDigRight(map, startX, startX + (lengthX / 2), startY, sx, ft);
				bspDigUp(map, startY - 1, endY, startX + (lengthX / 2), sx, ft);
				bspDigRight(map, startX + (lengthX / 2) + 1, endX, endY, sx, ft);
			}
			// Dig down
			else if(startY < endY){
				bspDigRight(map, startX, startX + (lengthX / 2), startY, sx, ft);
				bspDigDown(map, startY + 1, endY, startX + (lengthX / 2), sx, ft);
				bspDigRight(map, startX + (lengthX / 2) + 1, endX, endY, sx, ft);
			}
			// Dig right
			else{
				bspDigRight(map, startX, endX + 1, startY, sx, ft);
			}
		}
		// Dig vertical
		else{
			// Dig left
			if(startX > endX){
				bspDigDown(map, startY, startY + (lengthY / 2), startX, sx, ft);
				bspDigLeft(map, startX, startX - lengthX, startY + (lengthY / 2), sx, ft);
				bspDigDown(map, startY + (lengthY / 2) + 1, endY, startX - lengthX, sx, ft);
			}
			// Dig right
			else if(startX < endX){
				bspDigDown(map, startY, startY + (lengthY / 2), startX, sx, ft);
				bspDigRight(map, startX, startX + lengthX, startY + (lengthY / 2), sx, ft);
				bspDigDown(map, startY + (lengthY / 2) + 1, endY, startX + lengthX, sx, ft);
			}
			// Dig down
			else{
				bspDigDown(map, startY, endY + 1, startX, sx, ft);
			}
		}
	}
	if(bsp->left != NULL && !bspLinkRooms(bsp->left, map, level, sy, sx, ft)) return false;
	if(bsp->right != NULL && !bspLinkRooms(bsp->right, map, level, sy, sx, ft)) return false;
	return true;
}

BSP *bspDel(BSP *del){
	if(del == NULL)return;
	BSP *tl = del->left;
	BSP *tr = del->right;
	del = NULL;
	free(del);
	bspDel(tl);
	bspDel(tr);
	return del;
}

#endif
