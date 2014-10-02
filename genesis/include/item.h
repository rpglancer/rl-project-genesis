#ifndef H_ITEM
#define H_ITEM
#include "const.h"
#include "util.h"

typedef struct _ITEM{
	char ch;
	uchar color;
	char itemName[20];
	char name[20];
	int flags;
	uint itemSize;
	uint itemType;
	uint itemWearFlags;
	uint itemUseFlags;
	uint itemWeight;
	int itemAC;			// Armor Armor Class
	uint itemDmgType;		// Weapon damage type
	uint itemDmgRoll;		// Weapon damage roll [num dice]
	uint itemDmgSize;		// Weapon damage size [size dice]
	uint itemDmgMod;		// Weapon damage mod [+x]
}_ITEM;

typedef struct _ITEMSTATS{
	char ch;
	uchar color;
	char itemName[20];
	uint itemSize;
	uint itemType;
	uint itemWearF;
	uint itemUseF;
	uint itemWeight;
	uint itemDmgType;
	uint itemDmgRoll;
	uint itemDmgSize;
	uint itemDmgMod;
	int itemAC;
	struct _ITEMSTATS *next;
}_ITEMSTATS;

/*	This is obsolete shit, do not use it	*/
typedef struct INVENTORY{
	_ITEM *item;
	struct INVENTORY *next;
}INVENTORY;


typedef struct EQUIPMENT{
	bool isAvailable;
	uint slot;
	ITEMP item;
}EQUIPMENT;

_ITEMSTATS *ITEMLIST;

ITEMP newItem();
_ITEMSTATS *addItem(_ITEMSTATS *list);
_ITEMSTATS *delItemList(_ITEMSTATS *list);
_ITEMSTATS *loadItem(LL *list);
_ITEMSTATS *seekItem(_ITEM *name);

EQUIPMENT *addSlot(EQUIPMENT *list, int slotType);

void delEquipment(EQUIPMENT *eq);
void delItem(ITEMP item);

#endif
