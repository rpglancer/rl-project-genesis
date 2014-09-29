#ifndef H_MAP
#define H_MAP
#include "const.h"
#include "engine.h"
#include "entity.h"

/*		Tile Type structure		*/
struct TILE_TYPE{
	char tChar;		// The character representing the tile
	short tColor;		// The color of the character
	bool tPass;		// Is the tile passable?
};

/*		Totally not a rect structure	*/
struct RECT{
	int x;
	int y;
};

struct RECT *VIEW;		// Used for offsetting the view of the map.

/*		Index of TILE TYPEs		*/
extern struct TILE_TYPE TILE_INDEX[];

/*		Map TILE structure		*/
struct TILE{
	int tX;			// X Coordinate
	int tY;			// Y Coordinate
	int tT;			// Type indicator
};

/*		Global TILE structures		*/
struct TILE *MAP;		// Contains the entirety of the current map
struct TILE *MAPTAIL;		// Contains the information about the last map tile. [obsolete, still used]


/*		Global Map Function Prototypes		*/

int initMap(int, int, int, int, int);				// Initialize map

struct TILE *mapAllocate(struct TILE *map, int sy, int sx);	// Allocate space for map
struct TILE *mapFree(struct TILE *map);				// Free space from map

void drawMap();							// Draw the map
void fillMap(struct TILE *map, int sy, int sx, int tt);		// Fill map with specified tile type
void fillStairs(struct TILE *map, int sy, int sx, int ft);	// Add staircases to map
void fillWall(struct TILE *map,int sy,int sx,int ft,int wt);	// Surround open floor tiles with walls

#endif
