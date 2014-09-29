#ifndef H_ITEM
#define H_ITEM
#include "const.h"

typedef struct _ITEM{
	char itemName[20];
	unsigned int itemSize;
	unsigned int itemType;
	unsigned int itemWearFlags;
	unsigned int itemUseFlags;
	unsigned int itemWeight;
	int itemAC;				// Armor Armor Class
	unsigned int itemDmgType;		// Weapon damage type
	unsigned int itemDmgRoll;		// Weapon damage roll [num dice]
	unsigned int itemDmgSize;		// Weapon damage size [size dice]
	unsigned int itemDmgMod;		// Weapon damage mod [+x]
}_ITEM;

typedef struct _ITEMSTATS{
	char itemName[20];
	char ch;
	unsigned int itemSize;
	unsigned int itemType;
	unsigned int itemWearF;
	unsigned int itemUseF;
	unsigned int itemWeight;
	unsigned int itemDmgType;
	unsigned int itemDmgRoll;
	unsigned int itemDmgSize;
	unsigned int itemDmgMod;
	int itemAC;
	struct _ITEMSTATS *next;
}_ITEMSTATS;

typedef struct INVENTORY{
	_ITEM *item;
	struct INVENTORY *next;
}INVENTORY;

typedef struct EQUIPMENT{
	unsigned int slot;
	_ITEM *item;
	struct EQUIPMENT *next;
}EQUIPMENT;

_ITEMSTATS *ITEMLIST;

EQUIPMENT *addSlot(EQUIPMENT *list, int slotType);

#endif
