#ifndef H_CONST
#define H_CONST
#include <menu.h>
#include <ncurses.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// git test
/*
*	Definitions
*/
#define MAXHEIGHT	24
#define MAXWIDTH	80
#define MSGSTARTX	0
#define MSGSTARTY	0
#define MSGENDX		80
#define MSGENDY		3
#define	WINSTARTX	0
#define WINSTARTY	3
#define WINENDX		80
#define WINENDY		24
#define MAXMAPHEIGHT	255
#define MAXMAPWIDTH	255

/*	Cheap tile finding macros	*/
#define TL(y,sx,x) (y-1)*sx+(x-1)			// Top left tile
#define TM(y,sx,x) (y-1)*sx+x				// Top middle tile
#define TR(y,sx,x) (y-1)*sx+(x+1)			// Top right tile
#define CL(y,sx,x) y * sx + (x-1)			// Center left tile
#define CM(y,sx,x) y * sx + x				// Center middle tile
#define CR(y,sx,x) y * sx + (x+1)			// Center right tile
#define BL(y,sx,x) (y+1)*sx+(x-1)			// Bottom left tile
#define BM(y,sx,x) (y+1)*sx+x				// Bottom middle tile
#define BR(y,sx,x) (y+1)*sx+(x+1)			// Bottom right tile

/*	Less confusing tile finding macros	*/
#define NW(y, sx, x)	(y-1) * sx + (x-1)		// Tile to the northwest
#define N(y,sx,x)	(y-1) * sx + x			// Tile to the north
#define NE(y,sx,x)	(y-1) * sx + (x+1)		// Tile to the north east
#define E(y,sx,x)	y * sx + (x+1)			// Tile to the east
#define SE(y,sx,x)	(y+1) * sx + (x+1)		// Tile to the south east
#define S(y,sx,x)	(y+1) * sx + x			// Tile to the south
#define SW(y,sx,x)	(y+1) * sx + (x-1)		// Tile to the south west
#define W(y,sx,x)	y * sx + (x-1)			// Tile to the west
#define C(y,sx,x)	y * sx + x			// Tile currently on

#define OFFSETY(Y)	Y - VIEW->y + WINSTARTY
#define OFFSETX(X)	X - VIEW->x

#define ARRAYSIZE(a)	sizeof(a)/sizeof(a[0])
#define CREP CREATURE *
#define ENTP ENTITY *
#define ITEMP _ITEM *
#define OBJP OBJECT *

typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef unsigned long ULONG;
typedef char schar;
typedef UCHAR uchar;
typedef USHORT ushort;
typedef UINT uint;
typedef ULONG ulong;


/*	Enumerations			*/

enum CATEGORIES{
	C_CREATURE = 1,
	C_OBJECT = 2,
	C_ITEM = 3,
};

enum DMG_TYPES{
	DMG_MARTIAL = 0,
	DMG_BLUNT = 1,
	DMG_SLASH = 2,
	DMG_PIERCE = 3,
};

enum ITEM_TYPES{
	ITEM_NONE = 0,
	ITEM_CONSUMABLE = 1,
	ITEM_KEYITEM = 2,
	ITEM_WEAPON = 3,
	ITEM_ARMOR = 4,
	ITEM_ACC = 5,
};

enum OBJECT_TYPES{
	OBJ_DOOR = 0,
	OBJ_CHEST = 1,
};

enum RACES{
	RACE_HUMAN = 1,
	RACE_ELF = 2,
	RACE_DWARF = 3,
	RACE_HALFLING = 4,
};

enum CLASSES{
	CLASS_FIGHTER = 1,
	CLASS_WIZARD = 2,
	CLASS_ROGUE = 3,
};

enum DIRECTIONS{
	DIR_NORTH = 1,
	DIR_EAST = 2,
	DIR_SOUTH = 3,
	DIR_WEST = 4,
	DIR_UP = 5,
	DIR_DOWN = 6,
};

enum ENGINESTATE{
	ENGINESTOP = 0,
	ENGINERUN = 1,
	ENGINETITLE = 2,
};

enum ERRORS{
	ERR_NONE = 0,		// No errors
	ERR_MALLOC = -1,	// Memory allocation error
	ERR_OOB = -2,		// Out of bounds error
	ERR_NOCAT = -3,		// Invalid category for entity
	ERR_SHUTDOWN = -4,	// Engine shutdown signaled
};

enum FLAG_TYPES{
	FLAG_ISALIVE = 1,	// Entity is alive
	FLAG_ISVISIBLE = 2,	// Entity is visible
	FLAG_ISMOBILE = 4,	// Entity is mobile
	FLAG_ISAGGRESSIVE = 8,
};
/*
enum WEAR_FLAGS{
	WEAR_HEAD = 0,
	WEAR_BODY = 1,
	WEAR_WIELD_L = 2,
	WEAR_WIELD_R = 3,
	WEAR_HANDS = 4,
	WEAR_LEGS = 5,
	WEAR_FEET = 6,
};
*/
enum WEAR_FLAGS{
	WEAR_HEAD = 1,
	WEAR_BODY = 2,
	WEAR_WIELD_L = 4,
	WEAR_WIELD_R = 8,
	WEAR_HANDS = 16,
	WEAR_LEGS = 32,
	WEAR_FEET = 64,
};

enum WEAR_SIZES{
	SIZE_ALL = 0,
	SIZE_HUGE = 1,
	SIZE_VLARGE = 2,
	SIZE_LARGE = 3,
	SIZE_MEDIUM = 4,
	SIZE_SMALL = 5,
	SIZE_VSMALL = 6,
	SIZE_TINY = 7,
};

/*
*	Make sure entity.c is current with all
*	flag types any time a new FLAG_TYPE
*	is added to the enum.
*/
extern unsigned int FLAG_ARRAY[];

enum MAPTILES{
	TILE_ROCKFLOOR = 0,
	TILE_WALL = 1,
	TILE_DOORCLOSED = 2,
	TILE_DOOROPEN = 3,
	TILE_BLANK = 4,
	TILE_STAIRSDOWN = 5,
	TILE_STAIRSUP = 6,
};

enum MESSAGES{
	MSG_NONE = 0,		// No message
	MSG_MOVE = 1,		// Movement message
	MSG_ATTACK = 2,		// Attack message
	MSG_LOOK = 3,		// Look message
	MSG_EXAM = 4,		// Examine message
	MSG_OPEN = 5,		// Open message
	MSG_CLOSE = 6,		// Close message
	MSG_USE = 7,		// Use message
	MSG_CAST = 8,		// Cast message
	MSG_GET = 9,		// Get message
	MSG_DROP = 10,		// Drop message
};

enum LOADTYPE{
	TYPE_CREATURE = 0,
	TYPE_OBJECT = 1,
	TYPE_ITEM = 2,
	TYPE_ART = 3,
	TYPE_CLASS = 4,
	TYPE_RACE = 5,
};

enum HITTYPE{
	MISS = -1,
	HIT = 0,
	CRIT = 1,
};

int textHistorySize;

#endif
