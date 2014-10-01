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
	unsigned int level;		// The level of the trap
	unsigned int quality;		// The quality of the trap
	unsigned int traptype;		// The type of trap ie: spike, fire etc.
}TRAP;

/*		Entity structure		*/
typedef struct ENTITY{
	unsigned int category;		// The category of this entity
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
ENTITY *ENTCURRENT;
ENTITY *ENTROOT;
ENTITY *player;

ENTITY *delEntList(ENTITY *entity);
//ENTITY *spawnCreature(ENTITY *list, CREATURESTATS *creature, unsigned int level, unsigned int y, unsigned int x);
//ENTITY *spawnObject(ENTITY *list, OBJECTSTATS *object, unsigned int level, unsigned int y, unsigned int x);
ENTITY *seekEntity(ENTITY *entity);


bool canHear(ENTITY *src, ENTITY *tgt);
bool checkFlag(ENTITY *, int flag);
bool seedCreature(int level, unsigned int ft);
bool seedFurniture(int level);
bool seedTraps(int level);
bool seedTreasure(int level);

int initEnt();

unsigned int countEnt();

void addFlag(ENTITY *e, int flag);
void delEnt(ENTITY *entity);
void delFlag(ENTITY *e, int flag);
void doClose(ENTITY *entity);
void doOpen(ENTITY *entity);
void drawEnt();
void setClass(ENTITY *entity);
void setName(ENTITY *entity);
void setRace(ENTITY *entity);
void spawnCreature(ENTITY *list, CREATURESTATS *creature, unsigned int level, unsigned int y, unsigned int x);
void spawnItem(ENTITY *list, _ITEMSTATS *item, unsigned int level, unsigned int y, unsigned int x);
void spawnObject(ENTITY *list, OBJECTSTATS *object, unsigned int level, unsigned int y, unsigned int x);
void TEST_seedItem();

#endif
