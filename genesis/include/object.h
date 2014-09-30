#ifndef H_OBJECT
#define H_OBJECT
#include "const.h"
#include "item.h"
#include "util.h"

typedef struct OBJECT{
	unsigned int type;
	bool isLocked;
	bool isOpen;
	bool isPassable;
	unsigned int keyNum;
	INVENTORY *contents;
}OBJECT;

OBJECT *newObject();

typedef struct OBJECTSTATS{
	char name[15], shortDesc[25], longDesc[50];
	char chClosed, chOpen;
	unsigned int color, flags, type;
	unsigned int dLevelMin, dLevelMax;
	bool isLocked, isOpen, isPassable;
	int keyNum;
	struct OBJECTSTATS *next;
}OBJECTSTATS;

OBJECTSTATS *OBJECTLIST;

OBJECTSTATS *addObject(OBJECTSTATS *list);
OBJECTSTATS *delObjectList(OBJECTSTATS *list);
OBJECTSTATS *getObjects(OBJECTSTATS *list, unsigned int level, unsigned int type);
OBJECTSTATS *loadObject(LL *list);
OBJECTSTATS *seekObject(OBJECTSTATS *list, char *name);

void delObject(OBJECT *obj);
void setObjectStats(OBJECT *obj, OBJECTSTATS *stats);

#endif
