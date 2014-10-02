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
	uchar category;
	uchar locY;
	uchar locX;
	void *ent;
	struct ENTITY *next;
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


//void addEntity(ENTP list, UINT category);
void addEnt(ENTP list, uint category, uint y, uint x);
void addFlag(ENTP e, int flag);
void delEnt(ENTP entity);
void delFlag(ENTP e, int flag);
void doClose(ENTP entity);
void doOpen(ENTP entity);
void drawEnt();
void dropItem(ENTP entity, ITEMP item);
void getItem(ENTP entity, ENTP item);
void setClass(ENTP entity);
void setName(ENTP entity);
void setRace(ENTP *entity);
void spawnCreature(ENTP list, CREATURESTATS *creature, UINT level, UINT y, UINT x);
void spawnItem(ENTP list, _ITEMSTATS *item, UINT level, UINT y, UINT x);
void spawnObject(ENTP list, OBJECTSTATS *object, UINT level, UINT y, UINT x);
void TEST_seedItem();

#endif
