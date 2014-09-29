#ifndef C_DICE
#define C_DICE
#include "../include/dice.h"
/*
*	This should probably just be moved to util
*/
unsigned int rollDice(unsigned int numDice, unsigned int sideDice, int modDice){
	if(numDice == 0) return 0;
	int i, result;
	result = 0;
	for(i = 0; i < numDice; i++){
		result += rand() %sideDice + 1;
	}
	result = result + modDice;
	return result;
}

#endif
