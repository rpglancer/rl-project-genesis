#ifndef C_ENTITY
#define C_ENTITY
#include "../include/entity.h"
#include "../include/map.h"

UINT FLAG_ARRAY[] = {
	FLAG_ISALIVE,
	FLAG_ISVISIBLE,
	FLAG_ISMOBILE,
};


/*	Find a specific entity				*/
ENTITY *seekEntity(ENTP entity){
	if(entity == NULL) return NULL;
	if(ENTROOT == NULL) return NULL;
	for(ENTCURRENT = ENTROOT; ENTCURRENT != NULL; ENTCURRENT = ENTCURRENT->next){
		if(ENTCURRENT == entity) return ENTCURRENT;
	}
	return NULL;
}

/*	Get a count of all currently active entities.	*/
UINT countEnt(){
	UINT count = 0;
	ENTCURRENT = ENTROOT;
	if(ENTROOT == NULL || ENTCURRENT == NULL) return 0;
	for(ENTCURRENT = ENTROOT; ENTCURRENT != NULL; ENTCURRENT = ENTCURRENT->next){
		count++;
	}
	return count;
}

void addFlag(ENTP e, uint flag){
	if(e->flags[flag] == false) e->flags[flag] = true;
}

void delFlag(ENTP e, uint flag){
	if(e->flags[flag] == true) e->flags[flag] = false;
}

/*	Check applied flags of specified entity		*/
bool checkFlag(ENTP e, uint flag){
	return e->flags[flag];
}

bool canHear(ENTP src, ENTP tgt){
	if(src == NULL || tgt == NULL){
		 return false;
	}
	return (checkArea(src, tgt, ((CREP)src->ent)->WIS));
}

/*	Delete specified entity		*/
void delEnt(ENTP entity){
	if(entity == player) return;
	ENTP temp = ENTROOT;
	void *e = NULL;
	while(temp->next != entity) temp = temp->next;
	if(entity->next == NULL) temp->next = NULL;
	else temp->next = entity->next;
	switch(entity->category){
		case C_CREATURE:
			e = (CREP)entity->ent;
			delCreature(e);
			break;
		case C_ITEM:
			e = (ITEMP)entity->ent;
			delItem(e);
			break;
		case C_OBJECT:
			e = (OBJP)entity->ent;
			delObject(e);
			break;
	}
	free(entity);
	entity = NULL;
}

/*	Delete all entities		*/
ENTITY *delEntList(ENTP entity){
	if(entity == NULL) return NULL;
	while(entity != NULL){
		ENTP nextE = entity->next;
		switch(entity->category){
			case C_CREATURE:
				// No longer allocated, should free up with the ent being removed...
				break;
			case C_OBJECT:
				if( ((OBJP)entity->ent)->contents != NULL){
					while( ((OBJP)entity->ent)->contents != NULL ) {
						INVENTORY *temp = (INVENTORY *)((OBJECT *)entity->ent)->contents->next;
						((OBJECT *)entity->ent)->contents = NULL;
						((OBJECT *)entity->ent)->contents = temp;
					}
				}
				break;
			case C_ITEM:
				break;
			default:
				break;
		}
		free(entity->ent);
		entity->ent = NULL;
		free(entity);
		entity = NULL;
		entity = nextE;
	}
	return entity;
}

/*	Draw entities to screen		*/
void drawEnt(){
	for(ENTCURRENT = ENTROOT; ENTCURRENT != NULL; ENTCURRENT = ENTCURRENT->next){
		if(ENTCURRENT->locY - VIEW->y + WINSTARTY >= WINSTARTY){
			char ch = '\0';
			uchar color = 0;
			switch(ENTCURRENT->category){
				case C_CREATURE:
					ch = ((CREP)ENTCURRENT->ent)->ch;
					color = ((CREP)ENTCURRENT->ent)->color;
					break;
				case C_ITEM:
					ch = ((ITEMP)ENTCURRENT->ent)->ch;
					color = ((ITEMP)ENTCURRENT->ent)->color;
					break;
				case C_OBJECT:
					ch = ((OBJP)ENTCURRENT->ent)->ch;
					color = ((OBJP)ENTCURRENT->ent)->color;
					break;
			}
			attron(COLOR_PAIR(color));
			mvaddch(OFFSETY(ENTCURRENT->locY), OFFSETX(ENTCURRENT->locX), ch);
			attroff(COLOR_PAIR(color));
		}
	}
}

int calcStatMod(int stat){
	int mod = -5;
	if(stat > 1) mod = -4;
	if(stat > 3) mod = -3;
	if(stat > 5) mod = -2;
	if(stat > 7) mod = -1;
	if(stat > 9) mod = 0;
	if(stat > 11) mod = 1;
	if(stat > 13) mod = 2;
	if(stat > 15) mod = 3;
	if(stat > 17) mod = 4;
	if(stat > 19) mod = 5;
	if(stat > 21) mod = 6;
	if(stat > 23) mod = 7;
	if(stat > 25) mod = 8;
	if(stat > 27) mod = 9;
	if(stat > 29) mod = 10;
	return mod;
}

void displayClass(int class){
	CLASSSTATS *cs = CLASSLIST;
	int i;
	int y = MAXHEIGHT / 2;
	int x = MAXWIDTH / 2 + (15 / 2);
	for(i = 0; i != class; i++){
		cs = cs->next;
		if(cs == NULL) return;
	}
	mvprintw(y, x, "maxSTR: %2i maxCON: %2i", cs->maxSTR, cs->maxCON);
	mvprintw(y+1, x, "maxDEX: %2i maxINT: %2i", cs->maxDEX, cs->maxINT);
	mvprintw(y+2, x, "maxWIS: %2i maxCHA: %2i", cs->maxWIS, cs->maxCHA);
	mvprintw(y+3, x, "bnsFOR: %2i bnsREF: %2i", cs->bonusFOR, cs->bonusREF);
	mvprintw(y+4, x, "bnsWIL: %2i bnsINI: %2i", cs->bonusWIL, cs->bonusINI);
	mvprintw(y+5, x, "bnsMLE: %2i bnsRNG: %2i", cs->bonusMLE, cs->bonusRNG);
	mvprintw(y+6, x, "baseAC: %2i baseAB: %2i", cs->baseAC, cs->baseAB);
	return;
}

void doClose(ENTP entity){
	if(entity == NULL || entity->ent == NULL || entity->category != C_OBJECT) return;
	OBJP object = (OBJP)entity->ent;
	object->ch = object->chClosed;
	object->isOpen = false;
	switch(object->type){
		case OBJ_DOOR:
			object->isPassable = false;
			break;
		default:
			break;
	}
}

void doOpen(ENTP entity){
	if(entity == NULL || entity->ent == NULL || entity->category != C_OBJECT) return;
	OBJP object = (OBJP)entity->ent;
	object->ch = object->chOpen;
	object->isOpen = true;
	switch(object->type){
		case OBJ_DOOR:
			object->isPassable = true;
			break;
		default:
			break;
	}
}

void dropItem(ENTP entity, ITEMP item){
	if(entity == NULL || item == NULL) return;
	if(entity->category != C_CREATURE) return;
	CREP c = (CREP)entity->ent;
	size_t slot;
	for(slot = 0; slot < 10; slot++){
		if(&c->inventory[slot] == item) break;
	}
	addEnt(ENTROOT, C_ITEM, entity->locY, entity->locX);
	ENTCURRENT = ENTROOT;
	while(ENTCURRENT->next != NULL) ENTCURRENT = ENTCURRENT->next;
	ENTCURRENT->ent = newItem();
	ITEMP i = (ITEMP)ENTCURRENT->ent;
	memcpy(i, item, sizeof(_ITEM));
	c->inventory[slot].itemType = ITEM_NONE;
	return;
}

void getItem(ENTP entity, ENTP item){
	if(entity == NULL || item == NULL) return;
	if(entity->category != C_CREATURE || item->category != C_ITEM)return;
	CREP c = (CREP)entity->ent;
	ITEMP i = (ITEMP)item->ent;
	if(inventoryFull(c)) return;
	size_t slot;
	for(slot = 0; slot < 10; slot++){
		if(c->inventory[slot].itemType == ITEM_NONE) break;
	}
	memcpy(&c->inventory[slot], i, sizeof(_ITEM));
	delEnt(item);
	return;
}

void rollStats(CREP creature){
	if(creature == NULL) return;
	bool statsOK = false;
	int stats[6] = {0,0,0,0,0,0};
	CLASSSTATS *cs = getClass(CLASSLIST, creature);
	size_t i;
	int y, x;
	while(!statsOK){
		clear();
		for(i = 0; i < 6; i++){
			stats[i] = rollDice(3,6,0);
		}
		y = MAXHEIGHT / 2;
		x = MAXWIDTH / 2 - 7;
		mvprintw(y, x, "STR: %2i", stats[0]);
		mvprintw(y+1, x, "CON: %2i", stats[1]);
		mvprintw(y+2, x, "DEX: %2i", stats[2]);
		mvprintw(y+3, x, "INT: %2i", stats[3]);
		mvprintw(y+4, x, "WIS: %2i", stats[4]);
		mvprintw(y+5, x, "CHA: %2i", stats[5]);
		mvprintw(y+6, x, "BAC: %2i", cs->baseAC);
		x = MAXWIDTH / 2 + 7;
		mvprintw(y, x, "FOR: %2i", cs->bonusFOR + calcStatMod(stats[1]));
		mvprintw(y+1, x, "REF: %2i", cs->bonusREF + calcStatMod(stats[2]));
		mvprintw(y+2, x, "WIL: %2i", cs->bonusWIL + calcStatMod(stats[4]));
		mvprintw(y+3, x, "INI: %2i", cs->bonusINI + calcStatMod(stats[2]));
		mvprintw(y+4, x, "MLE: %2i", cs->bonusMLE + calcStatMod(stats[0]));
		mvprintw(y+5, x, "RNG: %2i", cs->bonusRNG + calcStatMod(stats[2]));
		mvprintw(y+6, x, "BAB: %2i", cs->baseAB);
		refresh();
		statsOK = yesNo();
	}
	creature->STR = stats[0];
	creature->CON = stats[1];
	creature->DEX = stats[2];
	creature->INT = stats[3];
	creature->WIS = stats[4];
	creature->CHA = stats[5];
	creature->FOR = cs->bonusFOR + calcStatMod(stats[1]);
	creature->REF = cs->bonusREF + calcStatMod(stats[2]);
	creature->WIL = cs->bonusWIL + calcStatMod(stats[4]);
	creature->INI = cs->bonusINI + calcStatMod(stats[2]);
	creature->MLE = cs->bonusMLE + calcStatMod(stats[0]);
	creature->RNG = cs->bonusRNG + calcStatMod(stats[2]);
	creature->BAC = cs->baseAC;
	creature->BAB = cs->baseAB;
	creature->hpMax = rollDice(1,20,creature->CON);
	creature->hp = creature->hpMax;
	creature->mpMax = rollDice(1,20,creature->INT);
	creature->mp = creature->mpMax;
	creature->apMax = 100;
	creature->ap = creature->apMax;
	return;
}

void setClass(ENTP entity){
	if(entity == NULL) return;
	CLASSSTATS *cs = CLASSLIST;
	size_t i;
	bool classOK = false;
	char cur = '*';
	char *choices[getClassCount(CLASSLIST)];
	for(cs = CLASSLIST, i = 0; cs != NULL; cs = cs->next, i++){
		choices[i] = cs->name;
	}
	int cy = 0;
	while(!classOK){
		clear();
		mvprintw(MAXHEIGHT / 2 - 1, MAXWIDTH / 2 - (14/2), "Choose a class");
		int y;
		for(i = 0, y = MAXHEIGHT / 2; i < getClassCount(CLASSLIST); i++, y++){
			if(MAXHEIGHT / 2 + cy == y)attron(COLOR_PAIR(3));
			mvprintw(y, MAXWIDTH / 2 - (14/2), choices[i]);
			if(MAXHEIGHT / 2 + cy == y)attroff(COLOR_PAIR(3));
		}
		mvaddch(MAXHEIGHT / 2 + cy, MAXWIDTH / 2 - (14/2) - 1, cur);
		displayClass(cy);
		refresh();
		genesis->ch = getch();
		switch(genesis->ch){
			case KEY_UP:
				if(cy > 0) --cy;
				break;
			case KEY_DOWN:
				if(cy < getClassCount(CLASSLIST) - 1) ++cy;
				break;
			case '\n':
				((CREATURE *)entity->ent)->class = cy;
				classOK = true;
				break;
		}
	}
	cs = CLASSLIST;
	for(i = 0; i != cy; i++){
		cs = cs->next;
	}
	if(cs != NULL)PLAYERCLASS = cs;
	rollStats( (CREP)entity->ent );
}

void setName(ENTP entity){
	if(entity == NULL) return;
	bool nameOK = false;
	char eName[15] = {'_', '_','_','_','_','_','_','_','_','_','_','_','_','_','\0'};
	int centerY = MAXHEIGHT / 2;
	int centerX = MAXWIDTH /2 - (15 / 2);
	while(!nameOK){
		clear();
		size_t i;
		for(i = 0; i < 14;){
			mvaddstr(centerY, centerX, eName);
			genesis->ch = getch();
			switch(genesis->ch){
				case '\n':
					i = 14;
					break;
				case KEY_BACKSPACE:
					if(i > 0) i--, eName[i] = '_';
					break;
				default:
					eName[i] = genesis->ch, i++;
					break;
			}
		}
		replaceChar(eName, sizeof(eName)/sizeof(eName[0]), '_', '\0');
		CREP p = (CREP)player->ent;
		replaceAll(p->name, ARRAYSIZE(p->name));
		strcpy(p->name, eName);
		nameOK = true;
	}
}

/*	Add the player entity to the root of the entities list		*/
int initEnt(){
	spawnCreature(ENTROOT, CREATURELIST, 1, 0, 0);
	if(ENTROOT == NULL) return ERR_MALLOC;
	player = ENTROOT;
	player->ent = ENTROOT->ent;
	return ERR_NONE;
}

bool seedCreature(int level, UINT ft){
	if(level < 1 || level > 99) return false;
	int i, creaturecount;
	CREATURESTATS *seed, *c;
	seed = NULL;
	for(c = CREATURELIST, creaturecount = 0; c != NULL; c = c->next){
		if(c->dLevelMin <= level && c->dLevelMax >= level && strcmp(c->name, "Player")){
			if(seed == NULL){
				seed = (CREATURESTATS *)calloc(1, sizeof(CREATURESTATS));
				if(seed == NULL) return false;
				seed[creaturecount] = *c;
			}
			else{
				seed = realloc(seed, sizeof(CREATURESTATS) * (creaturecount + 1));
				if(seed == NULL) return false;
				seed[creaturecount] = *c;
			}
			creaturecount++;
		}
	}
	if(creaturecount == 0) return false;
	else{
		int y,x;
		do{
			y = getRand_i(0, genesis->maxY);
			x = getRand_i(0, genesis->maxX);
		}while(MAP[CM(y, genesis->maxX, x)].tT != ft);
		UINT s = getRand_i(0, creaturecount);
		spawnCreature(ENTROOT, &seed[s], level, y, x);
	}
	free(seed);
	seed = NULL;
	return true;
}

void addEnt(ENTP list, UINT category, UINT y, UINT x){
	if(list == NULL) return;
	ENTP e = list;
	while(e->next != NULL) e = e->next;
	switch(category){
		case C_ITEM:
			e->next = (ENTP)calloc(1, sizeof(ENTITY) + sizeof(_ITEM));
			if(e->next == NULL) return;
			break;
		default:
			e->next = (ENTP)calloc(1, sizeof(ENTITY));
			if(e->next == NULL) return;
			break;
	}
	e->next->category = category;
	e->next->locY = y;
	e->next->locX = x;
}

void spawnCreature(ENTP list, CREATURESTATS *creature, UINT level, UINT y, UINT x){
	if(creature == NULL) return;
	ENTP e = list;
	int slotCount = 0;
	size_t i;
	for(i = 0; i < 7; i++){
		if(creature->eqSlots[i] == true) slotCount++;
	}
	size_t s = sizeof(ENTITY) + sizeof(CREATURE) + (sizeof(_ITEM) * slotCount);
	if(list == NULL){
		e = (ENTP)calloc(1, s);
		if(e == NULL) return;
		ENTROOT = e;
//		list = e;
	}
	else{
		while(e->next != NULL) e = e->next;
		e->next = (ENTP)calloc(1, s);
		if(e->next == NULL) return;
		e = e->next;
	}
	if(!(e->ent = newCreature())){
		free(e);
		e = NULL;
		return;
	}
	e->category = C_CREATURE;
	e->locY = y;
	e->locX = x;
	memcpy( ((CREP)e->ent)->name, creature->name, sizeof(((CREP)e->ent)->name));
	memcpy( ((CREP)e->ent)->shortDesc, creature->shortDesc, sizeof(((CREP)e->ent)->shortDesc));
	memcpy( ((CREP)e->ent)->longDesc, creature->longDesc, sizeof(((CREP)e->ent)->longDesc));
	((CREP)e->ent)->ch = creature->ch;
	((CREP)e->ent)->color = creature->color;
//	((CREP)e->ent)->flags = creature->flags;
	// Set flags
	memcpy(e->flags, creature->flags, sizeof(bool) * 4);
	setCreatureStats( (CREP)e->ent, creature, genesis->floor);
}

void spawnItem(ENTP list, _ITEMSTATS *item, UINT level, UINT y, UINT x){
	if(item == NULL) return;
	ENTP e = list;
	size_t s = sizeof(ENTITY) + sizeof(_ITEM);
	if(list == NULL){
		e = (ENTP)calloc(1,s);
		if(e == NULL) return;
		list = e;
	}
	else{
		while(e->next != NULL) e = e->next;
		e->next = (ENTP)calloc(1,s);
		if(e->next == NULL) return;
		e = e->next;
	}
	if(!(e->ent = newItem())){
		free(e);
		e = NULL;
		return;
	}
	e->locY = y;
	e->locX = x;
	e->category = C_ITEM;
	ITEMP temp = (ITEMP)e->ent;
	memcpy(&temp->name, &item->itemName, sizeof(temp->itemName));
	memcpy(&temp->itemName, &item->itemName, sizeof(temp->itemName));
	memcpy(&temp->itemSize, &item->itemSize, sizeof(temp->itemSize));
	memcpy(&temp->itemType, &item->itemType, sizeof(temp->itemType));
	memcpy(&temp->itemWearFlags, &item->itemWearF, sizeof(temp->itemWearFlags));
	memcpy(&temp->itemUseFlags, &item->itemUseF, sizeof(temp->itemUseFlags));
	memcpy(&temp->itemWeight, &item->itemWeight, sizeof(temp->itemWeight));
	memcpy(&temp->itemAC, &item->itemAC, sizeof(temp->itemAC));
	memcpy(&temp->itemDmgType, &item->itemDmgType, sizeof(temp->itemDmgType));
	memcpy(&temp->itemDmgRoll, &item->itemDmgRoll, sizeof(temp->itemDmgRoll));
	memcpy(&temp->itemDmgSize, &item->itemDmgSize, sizeof(temp->itemDmgSize));
	memcpy(&temp->itemDmgMod, &item->itemDmgMod, sizeof(temp->itemDmgMod));
	temp->ch = item->ch;
	temp->color = 2;
}

void spawnObject(ENTP list, OBJECTSTATS *object, UINT level, UINT y, UINT x){
	if(object == NULL) return;
	ENTP e = list;
	size_t s = sizeof(ENTITY) + sizeof(OBJECT);
	if(list == NULL){
		e = (ENTP)calloc(1, s);
		if(e == NULL) return;
		list = e;
	}
	else{
		while(e->next != NULL) e = e->next;
		e->next = (ENTP)calloc(1, s);
		if(e->next == NULL) return;
		e = e->next;
	}
	if(!(e->ent = newObject())){
		free(e);
		e = NULL;
		return;
	}
	e->category = C_OBJECT;
	e->locY = y;
	e->locX = x;
	OBJP o = (OBJP)e->ent;
	o->color = object->color;
	o->flags = object->flags;
	o->chOpen = object->chOpen;
	o->chClosed = object->chClosed;
	setObjectStats(o, object);
	(o->isOpen) ? (o->ch = o->chOpen) : (o->ch = o->chClosed);
}

void TEST_seedItem(){
	spawnItem(ENTROOT, ITEMLIST, 0, player->locY - 1, player->locX - 1);
	return;
}
#endif
