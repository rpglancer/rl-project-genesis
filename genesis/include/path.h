#ifndef H_PATH
#define H_PATH
#include "engine.h"

/*		LIST structure		*/
typedef struct LIST{
	struct TILE *start;	// Starting TILE
	struct TILE *dest;	// Destination TILE
	unsigned int F;		// Final score
	unsigned int G;		// Given path score
	unsigned int H;		// Heuristic estimate score
	struct LIST *parent;	// Parent LIST
	struct LIST *next;	// Next LIST
}LIST;

/*		Global LIST pointers	*/
LIST *ADJROOT;			// Adjacent TILE LIST
LIST *CLOSEDROOT;		// Closed TILE LIST
LIST *CURRENT;			// Currently selected LIST
LIST *OPENROOT;			// Open TILE LIST
LIST *LASTADDED;		// Last added LIST

LIST *addStart(struct TILE *start, struct TILE *dest);
LIST *addToClosed(LIST *toAdd);
LIST *addToOpen(LIST *toAdd);
LIST *freeList(LIST *toFree);
LIST *getAdjacent(LIST *ofTile, struct TILE *map);
LIST *getBestOpen();
LIST *remFromOpen(LIST *toRemove);

bool isEmpty(LIST *list);
bool listContains(LIST *toCheck, struct TILE *coords);

unsigned int getHeur(struct TILE *begin, struct TILE *end);

#endif
