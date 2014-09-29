#ifndef C_TEST
#define C_TEST
#include "../include/engine.h"

int main(int argc, char* argv[]){
	seedRand();
//	testGetRand();
//	testRollDice();
	if(initEngine() == ERR_NONE){
		genesis->engineStatus = ENGINETITLE;
		if(titleScreen() == ERR_NONE){
			engineRun();
		}
	}
	engineShutdown();
	endwin();
	return 0;
}

#endif
