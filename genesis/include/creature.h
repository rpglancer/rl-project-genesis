#ifndef H_CREATURE
#define H_CREATURE
#include "const.h"
#include "item.h"
#include "util.h"

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
        unsigned int class, level;              // Should probably have entity keep track of level
        int hp, hpMax;                          // Current HP / Maximum HP
        int mp, mpMax;                          // Current MP / Maximum MP
        int ap, apMax;                          // Current AP / Maximum AP
        int BAC, BAB;                           // Armor Class, AttackBonus
        int STR, CON, DEX, INT, WIS, CHA;
        int FOR, REF, WIL, INI, MLE, RNG;
        int atk, def;                           // Attack, Defense [obsolete]
        int invCount, slotCount;                // Inventory Count, Equipment Slots
        INVENTORY *inventory;                   // Inventory
        EQUIPMENT *equipment;                   // Equipment
}CREATURE;

/*      CREATURESTATS structure         */
typedef struct CREATURESTATS{
        char name[15], shortDesc[25], longDesc[50], ch;         // Name, description and ASCII representation
        unsigned int class, color, flags;                       // Class, color and flags
        unsigned int dLevelMin, dLevelMax;                      // Minimum and maximum dungeon level
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

size_t getClassCount(CLASSSTATS *list);

void delCreature(CREATURE *creature);
void manageEq(CREATURE *creature, unsigned int slot);
void setCreatureStats(CREATURE *creature, CREATURESTATS *stats, int level);

#endif
