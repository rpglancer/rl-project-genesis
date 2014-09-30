#ifndef H_ENGINE
#define H_ENGINE
#include "const.h"
#include "entity.h"
#include "map.h"
#include "msg.h"

typedef struct ENGINE{
	int ch;			// Engine input character
	int floor;		// The currently active floor
	int engineStatus;	// Engine running status
	unsigned int maxY;	// Dimensions of current map, Y
	unsigned int maxX;	// Dimensions of current map, X
	unsigned int entC;	// Count of entites currently active
}ENGINE;

/*	Global Vars	*/
ENGINE *genesis;

/*	Function Prototypes	*/
bool checkArea(ENTITY *src, ENTITY *tgt, unsigned int rad);
bool checkCombat(ENTITY *src, unsigned int dir);
bool checkCreature(ENTITY *entity, unsigned int dir);
bool checkMove(ENTITY *entity, unsigned int dir);
bool checkObject(ENTITY *entity, unsigned int dir);
bool checkTile(ENTITY *entity, unsigned int dir);

void doCombat(CREATURE *, CREATURE *);
void drawCoords();			// Test function for printing certain coordinates
void drawHeader();			// Draw relevant information at the top of the screen
void engineDraw();			// Draw everything
void engineInput();			// Get input and process input
void engineShutdown();			// Free everything
void newPlayer();
void quickAI();				// Crude AI processing
void setColors();

int engineRun();			// Run the engine
int engineUpdate();			// Update everything involved with the engine
int initEngine();			// Initialize the engine
int loadAssets();
int newGame();
int titleScreen();

void loadCurrent();			// Load from disk
void manageEq();			// Manage equipment
void saveCurrent();			// Save to disk
void selectObject(unsigned int msg);	// Replaced selectOpen for more universal support
void setViewCurrent(int, int);		// Force the view port to y,x

#endif
