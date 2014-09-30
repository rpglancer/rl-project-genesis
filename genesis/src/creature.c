#ifndef _C_CREATURE
#define _C_CREATURE
#include "../include/creature.h"

CLASSSTATS *addClass(CLASSSTATS *list){
	if(list == NULL){
		list = (CLASSSTATS *)calloc(1, sizeof(CLASSSTATS));
		if(list == NULL) return NULL;
		return list;
	}
	else{
		CLASSSTATS *temp = list;
		while(temp->next != NULL) temp = temp->next;
		temp->next = (CLASSSTATS *)calloc(1, sizeof(CLASSSTATS));
		if(temp->next == NULL) return NULL;
		return list;
	}
}

CLASSSTATS *delClassList(CLASSSTATS *list){
	while(list != NULL){
		CLASSSTATS *temp = list->next;
		free(list);
		list = NULL;
		list = temp;
	}
	return list;
}

CLASSSTATS *getClass(CLASSSTATS *list, CREATURE *creature){
	if(!list || !creature) return NULL;
	CLASSSTATS *cs = CLASSLIST;
	int i;
	for(i = 0; i != creature->class; i++){
		cs = cs->next;
	}
	return cs;
}

CLASSSTATS *loadClass(LL *list){
	if(list == NULL) return NULL;
	LL *l;
	CLASSSTATS *class = NULL;
	CLASSSTATS *c = NULL;
	for(l = list; l != NULL; l = l->next){
		size_t i;
		FILE *load = fopen(l->fileToLoad, "r");
		if(load == NULL) return NULL;
		class = addClass(class);
		rewind(load);
		c = class;
		while(c->next != NULL) c = c->next;
		fscanf(load, "%14s", c->name);
		fscanf(load, "%i %i", &c->maxSTR, &c->maxCON);
		fscanf(load, "%i %i", &c->maxDEX, &c->maxINT);
		fscanf(load, "%i %i", &c->maxWIS, &c->maxCHA);
		fscanf(load, "%i %i", &c->bonusFOR, &c->bonusREF);
		fscanf(load, "%i %i", &c->bonusWIL, &c->bonusINI);
		fscanf(load, "%i %i", &c->bonusMLE, &c->bonusRNG);
		fscanf(load, "%i %i", &c->baseAC, &c->baseAB);
		fclose(load);
	}
	return class;
}

CREATURE *newCreature(){
	CREATURE *c = (CREATURE *)calloc(1, sizeof(CREATURE));
	return c;
}

/*	Aloocate memory for a creature file to be loaded into	*/
CREATURESTATS *addCreature(CREATURESTATS *list){
	if(list == NULL){
		list = (CREATURESTATS *)calloc(1,sizeof(CREATURESTATS));
		if(list == NULL) return NULL;
		return list;
	}
	else{
		CREATURESTATS *temp = list;
		while(temp->next != NULL) temp = temp->next;
		temp->next = (CREATURESTATS *)calloc(1, sizeof(CREATURESTATS));
		if(temp->next == NULL) return NULL;		// Not sure if want
		return list;
	}
}

/*	Clear all creatures loaded into memory		*/
CREATURESTATS *delCreatureList(CREATURESTATS *list){
//	if(list == NULL) return NULL;		// Don't think I need this ...
	while(list != NULL){
		CREATURESTATS *temp = list->next;
		free(list);
		list = NULL;
		list = temp;
	}
	return list;
}

/*	Load the creature file into memory		*/
CREATURESTATS *loadCreature(LL *list){
	if(list == NULL) return NULL;
	LL *l;
	CREATURESTATS *creature = NULL;
	CREATURESTATS *c = NULL;
	for(l = list; l != NULL; l = l->next){
		FILE *load;
		size_t i;
		load = fopen(l->fileToLoad, "r");
		if(load == NULL) return NULL;
		creature = addCreature(creature);
		rewind(load);
		c = creature;
		while(c->next != NULL) c = c->next;
		fscanf(load, "%c", &c->ch);
		fscanf(load, "%15s", c->name);
		fscanf(load, "%25s", c->shortDesc);
		fscanf(load, "%50s", c->longDesc);
		for(i = 0; i < 50; i++){
			if(i < 15){
				if(c->name[i] == '_') c->name[i] = ' ';
			}
			if(i < 25){
				if(c->shortDesc[i] == '_') c->shortDesc[i] = ' ';
			}
			if(c->longDesc[i] == '_') c->longDesc[i] = ' ';
		}
		fscanf(load, "%i", &c->color);
		fscanf(load, "%i", &c->class);
		fscanf(load, "%i %i", &c->dLevelMin, &c->dLevelMax);
		fscanf(load, "%i %i", &c->hpMin, &c->hpMax);
		fscanf(load, "%i %i", &c->mpMin, &c->mpMax);
		fscanf(load, "%i %i", &c->apMin, &c->apMax);
		fscanf(load, "%i %i", &c->STR, &c->CON);
		fscanf(load, "%i %i", &c->DEX, &c->INT);
		fscanf(load, "%i %i", &c->WIS, &c->CHA);
		fscanf(load, "%i %i", &c->FOR, &c->REF);
		fscanf(load, "%i %i", &c->WIL, &c->INI);
		fscanf(load, "%i %i", &c->MLE, &c->RNG);
		fscanf(load, "%i %i", &c->BAC, &c->BAB);
		fscanf(load, "%i", &c->flags);
		fscanf(load, "%i", &c->eqSlots[0]);
		fscanf(load, "%i", &c->eqSlots[1]);
		fscanf(load, "%i", &c->eqSlots[2]);
		fscanf(load, "%i", &c->eqSlots[3]);
		fscanf(load, "%i", &c->eqSlots[4]);
		fscanf(load, "%i", &c->eqSlots[5]);
		fscanf(load, "%i", &c->eqSlots[6]);
		fclose(load);
	}
	return creature;
}

size_t getClassCount(CLASSSTATS *list){
	if(list == NULL) return 0;
	CLASSSTATS *c = list;
	size_t count = 0;
	while(c != NULL){
		count++;
		c = c->next;
	}
	return count;
}

void delCreature(CREATURE *creature){
	if(creature == NULL) return;
	free(creature);
	creature = NULL;
}

void setCreatureStats(CREATURE *creature, CREATURESTATS *stats, int level){
	if(!creature || !stats) return;
	int sT = 1;
	size_t i;
	for(i = 0; i < 7; i++){
		if(stats->eqSlots[i] == true) creature->equipment = addSlot(creature->equipment, sT), ++creature->slotCount;
		sT = sT * 2;
	}
	creature->hpMax = getRand_i(stats->hpMin, stats->hpMax);
	creature->hp = creature->hpMax;
	creature->mpMax = getRand_i(stats->mpMin, stats->mpMax);
	creature->mp = creature->mpMax;
	creature->apMax = getRand_i(stats->apMin, stats->apMax);
	creature->ap = creature->apMax;
	creature->STR = stats->STR;
	creature->CON = stats->CON;
	creature->DEX = stats->DEX;
	creature->INT = stats->INT;
	creature->WIS = stats->WIS;
	creature->CHA = stats->CHA;
	creature->level = getRand_i(getMinLevel(level), getMaxLevel(level));
	return;
}

#endif
