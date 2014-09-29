#ifndef C_PATH
#define C_PATH
#include "../include/path.h"

LIST *addStart(struct TILE *start, struct TILE *dest){
	OPENROOT = (LIST *)calloc(1, sizeof(LIST));
	if(OPENROOT == NULL) return NULL;
	OPENROOT->parent = NULL;
	OPENROOT->next = NULL;
	OPENROOT->start = start;
	OPENROOT->dest = dest;
	OPENROOT->G = 0;
	OPENROOT->H = getHeur(OPENROOT->start, OPENROOT->dest);
	OPENROOT->F = OPENROOT->G + OPENROOT->H;
	return OPENROOT;
}

LIST *addToClosed(LIST *toAdd){
	if(CLOSEDROOT == NULL){
		CLOSEDROOT = toAdd;
		if(OPENROOT == toAdd && OPENROOT->next != NULL) OPENROOT = OPENROOT->next;
		CLOSEDROOT->next = NULL;
		return CLOSEDROOT;
	}
	else{
		LIST *CLOSEDCUR;
		for(CLOSEDCUR = CLOSEDROOT; CLOSEDCUR->next != NULL; CLOSEDCUR = CLOSEDCUR->next);
		CLOSEDCUR->next = toAdd;
		if(OPENROOT == toAdd && OPENROOT->next != NULL) OPENROOT = OPENROOT->next;
		CLOSEDCUR = CLOSEDCUR->next;
		CLOSEDCUR->next = NULL;
		if(CLOSEDROOT->next == NULL) CLOSEDROOT->next = CLOSEDCUR;
		return CLOSEDROOT;
	}
}

LIST *addToOpen(LIST *toAdd){
	OPENROOT = OPENROOT;
	if(OPENROOT == NULL){
		OPENROOT = toAdd;
		OPENROOT->next = NULL;
		return OPENROOT;
	}
	else{
		LIST *OPENCUR;
		for(OPENCUR = OPENROOT; OPENCUR->next != NULL; OPENCUR = OPENCUR->next);
		OPENCUR->next = toAdd;
		OPENCUR = OPENCUR->next;
		OPENCUR->next = NULL;
		if(OPENROOT->next == NULL) OPENROOT->next = OPENCUR;
		return OPENROOT;
	}
}

LIST *freeList(LIST *toFree){
	if(toFree == NULL) return NULL;
	while(toFree != NULL){
		LIST *temp = toFree->next;
		free(toFree);
		toFree = NULL;
		toFree = temp;
	}
/*
	if(toFree == NULL) return NULL;
	else{
		while(toFree != NULL){
			if(toFree->next == NULL){
				free(toFree);
				toFree = NULL;
			}
			else{
				LIST *temp = toFree->next;
				free(toFree);
				toFree = temp;
			}
		}
	}
*/
	return toFree;
}

LIST *getAdjacent(LIST *ofTile, struct TILE *map){
	unsigned int i, m;
	bool validDirection[] = {false, false, false, false};
	if( ofTile->start->tY - 1 > 0) validDirection[0] = true;
	if( ofTile->start->tX + 1 < genesis->maxX) validDirection[1] = true;
	if( ofTile->start->tY + 1 < genesis->maxX) validDirection[2] = true;
	if( ofTile->start->tX - 1 > 0) validDirection[3] = true;
	for(i = 0; i < 4; i++){
		if(validDirection[i] == true){
			switch(i){
				case 0:
					m = TM(ofTile->start->tY, genesis->maxX, ofTile->start->tX);
					if(TILE_INDEX[map[TM(ofTile->start->tY, genesis->maxX, ofTile->start->tX)].tT].tPass == false) continue;
					if(listContains(CLOSEDROOT, &map[TM(ofTile->start->tY, genesis->maxX, ofTile->start->tX)])) continue;
					if(listContains(OPENROOT, &map[TM(ofTile->start->tY, genesis->maxX, ofTile->start->tX)])) continue;	// No need for duplicate opens, test
					break;
				case 1:
					m = CR(ofTile->start->tY, genesis->maxX, ofTile->start->tX);
					if(TILE_INDEX[map[CR(ofTile->start->tY, genesis->maxX, ofTile->start->tX)].tT].tPass == false) continue;
					if(listContains(CLOSEDROOT, &map[CR(ofTile->start->tY, genesis->maxX, ofTile->start->tX)])) continue;
					if(listContains(OPENROOT, &map[CR(ofTile->start->tY, genesis->maxX, ofTile->start->tX)])) continue;	// No need for duplicate opens, test
					break;
				case 2:
					m = BM(ofTile->start->tY, genesis->maxX, ofTile->start->tX);
					if(TILE_INDEX[map[BM(ofTile->start->tY, genesis->maxX, ofTile->start->tX)].tT].tPass == false) continue;
					if(listContains(CLOSEDROOT, &map[BM(ofTile->start->tY, genesis->maxX, ofTile->start->tX)])) continue;
					if(listContains(OPENROOT, &map[BM(ofTile->start->tY, genesis->maxX, ofTile->start->tX)])) continue;	// No need for duplicate opens, test
					break;
				case 3:
					m = CL(ofTile->start->tY, genesis->maxX, ofTile->start->tX);
					if(TILE_INDEX[map[CL(ofTile->start->tY, genesis->maxX, ofTile->start->tX)].tT].tPass == false) continue;
					if(listContains(CLOSEDROOT, &map[CL(ofTile->start->tY, genesis->maxX, ofTile->start->tX)])) continue;
					if(listContains(OPENROOT, &map[CL(ofTile->start->tY, genesis->maxX, ofTile->start->tX)])) continue;	// No need for duplicate opens, test
					break;
			}
			if(ADJROOT == NULL){
				ADJROOT = (LIST *)calloc(1, sizeof(LIST));
				if(ADJROOT == NULL) return NULL;
				ADJROOT->start = &map[m];
				ADJROOT->dest = ofTile->dest;
				ADJROOT->parent = ofTile;
				ADJROOT->G = ofTile->G + 1;
				ADJROOT->H = getHeur(ADJROOT->start, ADJROOT->dest);
				ADJROOT->F = ADJROOT->G + ADJROOT->H;
				ADJROOT->next = NULL;
			}
			else{
				LIST *ADJCUR;
				for(ADJCUR = ADJROOT; ADJCUR->next != NULL; ADJCUR = ADJCUR->next);
				ADJCUR->next = (LIST *)calloc(1, sizeof(LIST));
				if(ADJCUR->next == NULL) return NULL;
				ADJCUR = ADJCUR->next;
				ADJCUR->start = &map[m];
				ADJCUR->dest = ofTile->dest;
				ADJCUR->parent = ofTile;
				ADJCUR->G = ofTile->G + 1;
				ADJCUR->H = getHeur(ADJCUR->start, ADJCUR->dest);
				ADJCUR->F = ADJCUR->G + ADJCUR->H;
				ADJCUR->next = NULL;
				if(ADJROOT->next == NULL) ADJROOT->next = ADJCUR;
			}
		}
	}
	return ADJROOT;
}

LIST *getBestOpen(){
	if(OPENROOT == NULL) return NULL;
	LIST *best = NULL;
	for(CURRENT = OPENROOT; CURRENT != NULL; CURRENT = CURRENT->next){
		if(best == NULL) best = CURRENT;
		if(CURRENT->F < best->F) best = CURRENT;
		if(CURRENT->parent == LASTADDED && CURRENT->F <= best->F) best = CURRENT;
	}
	LASTADDED = best;
	return best;
}

LIST *remFromOpen(LIST *toRemove){
	if(OPENROOT == NULL){
		return NULL;
	}
	else if(OPENROOT == toRemove){
		if(OPENROOT->next == NULL){
			return NULL;
		}
		else{
			OPENROOT = OPENROOT->next;
		}
		return OPENROOT;
	}
	else{
		LIST *temp;
		for(temp = OPENROOT; temp->next != toRemove; temp = temp->next){
			if(temp->next == NULL){
				return OPENROOT;
			}
		}
		if(toRemove->next == NULL){
			temp->next = NULL;
		}
		else{
			temp->next = toRemove->next;
		}
	}
	return OPENROOT;
}

bool isEmpty(LIST *list){
	if(list == NULL) return true;
	return false;
}

bool listContains(LIST *toCheck, struct TILE *coords){
	if(toCheck == NULL) return false;
	LIST *temp;
	for(temp = toCheck; temp != NULL; temp = temp->next){
		if(temp->start == coords) return true;
	}
	return false;
}

unsigned int findPath(struct TILE *start, struct TILE *dest){
	bool pathFound = false;
	unsigned int direction = 0;
	addStart(start, dest);
	do{
		CURRENT = getBestOpen();
		CLOSEDROOT = addToClosed(CURRENT);
		OPENROOT = remFromOpen(CURRENT);
		if(listContains(CLOSEDROOT, CLOSEDROOT->dest)){
			pathFound = true;
			break;
		}
		ADJROOT = NULL;
		ADJROOT = freeList(ADJROOT);
		ADJROOT = getAdjacent(CURRENT, MAP);
		LIST *ADJI;
/*		I'm confident that this will cause leaks	*/
		for(ADJI = ADJROOT; ADJI != NULL; ADJI = ADJI->next){
			if(listContains(CLOSEDROOT, ADJI->start)){
				continue;
			}
//		If this isn't added to the OPENROOT, the memory
//		allocation has still already been done, and when
//		ADJROOT is NULLed on the next cycle the pointer
//		to it, while still allocated, will be lost from
//		active lists. Thus when OPENROOT and CLOSEDROOT
//		are freed at the end of the function said memory
//		in isolated blocks is never freed.
			if(!listContains(OPENROOT, ADJI->start)){
				LIST *temp = ADJI->next;
				OPENROOT = addToOpen(ADJI);
				ADJI->next = temp;
			}
			else{
				if((CURRENT->G + 1) + getHeur(ADJI->start, ADJI->dest) < ADJI->F ){
					ADJI->parent = CURRENT;
				}
			}
		}
	}while(!isEmpty(OPENROOT));
	if(pathFound == true){
		for(CURRENT = CLOSEDROOT; CURRENT->next != NULL; CURRENT = CURRENT->next);
		for(CURRENT; CURRENT->parent != NULL; CURRENT = CURRENT->parent){
			LIST *PARENT = CURRENT->parent;
			if(CURRENT->start->tY == PARENT->start->tY - 1 && CURRENT->start->tX == PARENT->start->tX){
				direction = DIR_NORTH;
			}
			if(CURRENT->start->tY == PARENT->start->tY && CURRENT->start->tX == PARENT->start->tX + 1){
				direction = DIR_EAST;
			}
			if(CURRENT->start->tY == PARENT->start->tY + 1 && CURRENT->start->tX == PARENT->start->tX){
				direction = DIR_SOUTH;
			}
			if(CURRENT->start->tY == PARENT->start->tY && CURRENT->start->tX == PARENT->start->tX - 1){
				direction = DIR_WEST;
			}
		}
	}
	else{
		direction = getRand_i(1,4);
	}
	CLOSEDROOT = freeList(CLOSEDROOT);
	OPENROOT = freeList(OPENROOT);
	return direction;
}

unsigned int getHeur(struct TILE *start, struct TILE *dest){
	int absY = start->tY - dest->tY;
	int absX = start->tX - dest->tX;
	if(absY < 0) absY = -absY;
	if(absX < 0) absX = -absX;
	return absY + absX;
}

#endif
