#ifndef H_ENTITY
#define H_ENTITY
#include "const.h"
#include "creature.h"
#include "dice.h"
#include "item.h"
#include "object.h"
#include "util.h"

/*		Trap structure			*/
typedef struct TRAP{
	UINT level;		// The level of the trap
	UINT quality;		// The quality of the trap
	UINT traptype;		// The type of trap ie: spike, fire etc.
}TRAP;

/*		Entity structure		*/
typedef struct ENTITY{
	UINT category;		// The category of this entity
	char name[15];			// Name of entity
	char shortDesc[25];		// Short description of entity
	char longDesc[50];		// Long description of entity
	char ch;			// The ASCII character represnenting the entity
	short color;			// The color pair of the entity
	void *ent;			// The contents specific to the entity category
	int locY;			// The Y coordinate of the entity [former uInt]
	int locX;			// The X coordinate of the entity [former uInt]
	int flags;			// Flags relevant to the entity
	struct ENTITY *next;		// The next entity in the list
}ENTITY;

/*		Global entity pointers		*/
ENTP ENTCURRENT;
ENTP ENTROOT;
ENTP player;

ENTP delEntList(ENTP entity);
ENTP seekEntity(ENTP entity);

bool canHear(ENTP src, ENTP tgt);
bool checkFlag(ENTP, int);
bool seedCreature(int level, UINT ft);
bool seedFurniture(int level);
bool seedTraps(int level);
bool seedTreasure(int level);

int initEnt();

unsigned int countEnt();


void addEntity(ENTP list, UINT category);
void addFlag(ENTP e, int flag);
void delEnt(ENTP entity);
void delFlag(ENTP e, int flag);
void doClose(ENTP entity);
void doOpen(ENTP entity);
void drawEnt();
void moveItem(ITEMP item);
void setClass(ENTP entity);
void setName(ENTP entity);
void setRace(ENTP *entity);
void spawnCreature(ENTP list, CREATURESTATS *creature, UINT level, UINT y, UINT x);
void spawnItem(ENTP list, _ITEMSTATS *item, UINT level, UINT y, UINT x);
void spawnObject(ENTP list, OBJECTSTATS *object, UINT level, UINT y, UINT x);
void TEST_seedItem();

#endif
