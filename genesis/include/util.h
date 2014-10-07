#ifndef H_UTIL
#define H_UTIL
#include "const.h"

/*	(L)oad (L)ist structure		*/
typedef struct LL{
	char fileToLoad[50];		// Increase if needed
	struct LL *next;
}LL;

LL *delLoadList(LL *list);
LL *loadList(unsigned int listType);

typedef struct ART{
	char name[25];
	int sizeY, sizeX;
	char *contents;
	char *color;
	struct ART *next;
}ART;

ART *ARTLIST;
ART *addArt(ART *list);
ART *delArtList(ART *list);
ART *loadArt(LL *list);
ART *seekArt(ART *list, char *name);

bool isNull(void *);
bool yesNo();

int getMinLevel(int level);
int getMaxLevel(int level);
int getRand_i(int min, int max);

size_t getSlot(uint wearFlag);

void logEntry(char *text);
void replaceAll(char *text, size_t count);
void replaceChar(char *text, size_t count, char toReplace, char withChar);

void seedRand();					// Seed random number generator
void testGetRand();					// Test RNG

#endif
