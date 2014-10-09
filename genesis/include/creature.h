#ifndef H_CREATURE
#define H_CREATURE
#include "const.h"
#include "item.h"
#include "util.h"
#include "winmgr.h"

typedef struct CLASSSTATS{
	char name[15];
	int maxSTR, maxCON;
	int maxDEX, maxINT;
	int maxWIS, maxCHA;
	int bonusFOR, bonusREF;
	int bonusWIL, bonusINI;
	int bonusMLE, bonusRNG;
	int baseAC, baseAB;
	struct CLASSSTATS *next;
}CLASSSTATS;

typedef struct CREATURE{
	char ch;
	char name[20];
	char shortDesc[25];
	char longDesc[50];
	int flags;
	uchar color;
        UINT class, level;              	// Class and level
        int hp, hpMax;                          // Current HP / Maximum HP
        int mp, mpMax;                          // Current MP / Maximum MP
        int ap, apMax;                          // Current AP / Maximum AP
        int BAC, BAB;                           // Armor Class, AttackBonus
        int STR, CON, DEX, INT, WIS, CHA;	// Primary stats
        int FOR, REF, WIL, INI, MLE, RNG;	// Secondary stats
        int atk, def;                           // Attack, Defense [obsolete]
        int invCount, slotCount;                // Inventory Count, Equipment Slots
	_ITEM inventory[10];
	EQUIPMENT equipment[7];
}CREATURE;

/*      CREATURESTATS structure         */
typedef struct CREATURESTATS{
        char name[20], shortDesc[25], longDesc[50], ch;         // Name, description and ASCII representation
        UINT class, color, flags;                       	// Class, color and flags
        UINT dLevelMin, dLevelMax;                     	 	// Minimum and maximum dungeon level
        int hpMin, hpMax;                                       // Minimum and maximum HP
        int mpMin, mpMax;                                       // Minimum and maximum MP
        int apMin, apMax;                                       // Minimum and maximum AP
        int STR, CON, DEX, INT, WIS, CHA;                       // Strength, Constitution, Dexterity, Intelligence$
        int FOR, REF, WIL, INI, MLE, RNG;                       // Fortitude, Reflex, Will, Initiative, Melee, Ran$
        int BAC, BAB;                                           // Base Armor Class, Base Attack Bonus
        bool eqSlots[7];                                        // Equipment Slots
        struct CREATURESTATS *next;                             // Next in list
}CREATURESTATS;


CLASSSTATS *CLASSLIST;
CLASSSTATS *PLAYERCLASS;
CREATURESTATS *CREATURELIST;

CLASSSTATS *addClass(CLASSSTATS *list);
CLASSSTATS *delClassList(CLASSSTATS *list);
CLASSSTATS *getClass(CLASSSTATS *list, CREATURE *creature);
CLASSSTATS *loadClass(LL *list);

CREATURE *newCreature();

CREATURESTATS *addCreature(CREATURESTATS *list);	// Allocate memory for a new creature file
CREATURESTATS *delCreatureList(CREATURESTATS *list);	// Delete all available creatures
CREATURESTATS *loadCreature(LL *list);			// Load creature file into memory

bool isCarried(CREP creature, ITEMP item);
bool isEquipped(CREP creature, ITEMP item);
bool inventoryFull(CREP creature);

size_t getClassCount(CLASSSTATS *list);
size_t selectItem(CREP creature);

void delCreature(CREP creature);
void equipItem(CREP creature, ITEMP item);
void manageEq(CREP creature, UINT slot);
void removeItem(CREP creature, ITEMP item);
void setCreatureStats(CREP creature, CREATURESTATS *stats, int level);

#endif
