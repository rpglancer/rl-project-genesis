#ifndef H_NEWBSP
#define H_NEWBSP
#include "const.h"
#include "entity.h"
#include "map.h"
#include "util.h"

/*		BSP structure		*/
typedef struct BSP{
	int y;				// Starting Y coordinate
	int x;				// Starting X coordinate
	int h;				// Ending Y coordinate
	int w;				// Ending X coordinate
	unsigned int level;		// The level on which this node resides
	struct BSP *father;		// Parent node
	struct BSP *left;		// Left branch
	struct BSP *right;		// Right branch
}BSP;

/*		Global BSP pointers	*/
BSP *BSPROOT;				// Beginning of the BSP tree

bool bspAddDoors(BSP *bsp, struct TILE *map, int level, int sx, int ft, int wt);
bool bspDoResize(BSP *bsp, int level);
bool bspDrawRooms(BSP *bsp, struct TILE *map, int level, int sy, int sx, int ft, int wt);
bool bspIsLeaf(BSP *bsp);
bool bspIsRoot(BSP *bsp);
bool bspLinkRooms(BSP *bsp, struct TILE *map, int level,  int sy, int sx, int ft);
bool bspLoad(BSP *bsp, int level);
bool bspPrint(BSP *bsp, int level);

BSP *bspDel(BSP *del);
BSP *bspNew(BSP *bsp, int height, int width, int depth);
BSP *bspResize(BSP *bsp, float hPct, float wPct);

void bspRecursive(BSP *bsp, int depth);
void bspSplit(BSP *bsp, int direction, int position);


#endif
