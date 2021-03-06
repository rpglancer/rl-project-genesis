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

CLASSSTATS *getClass(CLASSSTATS *list, CREP creature){
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
//		size_t i;
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
	CREP c = (CREP)calloc(1, sizeof(CREATURE));
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
//		fscanf(load, "%i", &c->flags);
		fscanf(load, "%i %i %i %i", &c->flags[0], &c->flags[1], &c->flags[2], &c->flags[3]);
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

bool isCarried(CREP creature, ITEMP item){
	if(isNull(creature) || isNull(item)) return false;
	size_t i;
	for(i = 0; i < 10; i++){
		if(&creature->inventory[i] == item) return true;
	}
	return false;
}

bool isEquipped(CREP creature, ITEMP item){
	if(isNull(creature) || isNull(item)) return false;
	size_t i;
	for(i = 0; i < 7; i++){
		if(creature->equipment[i].item == item) return true;
	}
	return false;
}

bool inventoryFull(CREP creature){
	if(creature == NULL) return true;
	size_t i;
	for(i = 0; i < 10; i++){
		if(creature->inventory[i].itemType == ITEM_NONE) return false;
	}
	return true;
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

void delCreature(CREP creature){
	if(creature == NULL) return;
	free(creature);
	creature = NULL;
}

void equipItem(CREP creature, ITEMP item){
	if(creature == NULL || item == NULL) return;
	EQUIPMENT *eq = NULL;
	switch(item->itemWearFlags){
		case WEAR_HEAD:
			eq = &creature->equipment[0];
			break;
		case WEAR_BODY:
			eq = &creature->equipment[1];
			break;
		case WEAR_WIELD_L:
			eq = &creature->equipment[2];
			break;
		case WEAR_WIELD_R:
			eq = &creature->equipment[3];
			break;
		case WEAR_HANDS:
			eq = &creature->equipment[4];
			break;
		case WEAR_LEGS:
			eq = &creature->equipment[5];
			break;
		case WEAR_FEET:
			eq = &creature->equipment[6];
			break;
		default:
			return;
	}
	if(isNull(eq)) return;
	if(eq->item == NULL){
		eq->item = (ITEMP)malloc(sizeof(_ITEM));
		memcpy(eq->item, item, sizeof(_ITEM));
		item->itemType = ITEM_NONE;
	}
	else{
		ITEMP temp = eq->item;
		memcpy(eq->item, item, sizeof(_ITEM));
		memcpy(item, temp, sizeof(_ITEM));
	}
}

void removeItem(CREP creature, ITEMP item){
	if(isNull(creature) || isNull(item)) return;
	EQUIPMENT *eq = &creature->equipment[item->itemWearFlags];
	if(isNull(eq))return;
	size_t i;
	for(i = 0; i < 10; i++){
		if(creature->inventory[i].itemType == ITEM_NONE) break;
	}
	if(creature->inventory[i].itemType != ITEM_NONE) return;
	memcpy(&creature->inventory[i], item, sizeof(_ITEM));
	i = item->itemWearFlags;
	creature->equipment[i].item = NULL;
	free(creature->equipment[i].item);
	return;
}

size_t selectItem(CREP creature){
	if(creature == NULL) return -1;		// size_t is unsigned, dummy
	int cursY = 1;
	int cursX = 0;
	char select = '>';
	bool selectOK = false;
	WINDOW *select_item = newWindow(10, 20, MAXHEIGHT/2, MAXWIDTH/2);
	while(!selectOK){
		wclear(select_item);
		mvwprintw(select_item, 0, 10, "-- Inventory --");
		size_t i;
		int y;
		for(i = 0, y = 1; i < 10; i++, y++){
			if(creature->inventory[i].itemType != ITEM_NONE) mvwprintw(select_item, y, 1, "%s", creature->inventory[i].itemName);
			else mvwprintw(select_item, y, 1, "[None]");
		}
		mvwaddch(select_item, cursY, cursX, select);
		wrefresh(select_item);
		switch(getch()){
			case KEY_UP:
				if(cursY > 1) --cursY;
				break;
			case KEY_DOWN:
				if(cursY < 10) ++cursY;
				break;
			case '\n':
				i = cursY - 1;
				delWindow(select_item);
				refresh();
				return i;
		}
	}
}

void manageEq(CREP creature, UINT slot){
	return;
}

void setCreatureStats(CREP creature, CREATURESTATS *stats, int level){
	if(!creature || !stats) return;
	int sT = 1;
	size_t i;
	for(i = 0; i < 7; i++){
		if(stats->eqSlots[i] == true) creature->equipment[i].isAvailable = true;
		else creature->equipment[i].isAvailable = false;
		creature->equipment[i].slot = sT;
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
