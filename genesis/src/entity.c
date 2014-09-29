#ifndef C_ENTITY
#define C_ENTITY
#include "../include/entity.h"
#include "../include/map.h"

unsigned int FLAG_ARRAY[] = {
	FLAG_ISALIVE,
	FLAG_ISVISIBLE,
	FLAG_ISMOBILE,
};

/*
*	This will need to be reworded to handle multiple types
*	of objects without a switch/case for each one.
*/
ENTITY *doClose(ENTITY *entity){
	if(entity == NULL) return NULL;
	if(entity->category != C_OBJECT) return entity;
	if(entity->ent == NULL) return entity;
	((OBJECT *)entity->ent)->isOpen = false;
	OBJECTSTATS *temp = seekObject(OBJECTLIST, entity->name);
	entity->ch = temp->chClosed;
	switch( ((OBJECT *)entity->ent)->type){
		case OBJ_DOOR:
			((OBJECT *)entity->ent)->isPassable = false;
			break;
		default:
			break;
	}
	return entity;
}
/*
*	See: "doClose()"
*/
ENTITY *doOpen(ENTITY *entity){
	if(entity == NULL) return NULL;
	if(entity->category != C_OBJECT) return entity;
	if(entity->ent == NULL) return entity;
	((OBJECT *)entity->ent)->isOpen = true;
	OBJECTSTATS *temp = seekObject(OBJECTLIST, entity->name);
	entity->ch = temp->chOpen;
	switch( ((OBJECT *)entity->ent)->type){
		case OBJ_DOOR:
			((OBJECT *)entity->ent)->isPassable = true;
			break;
		default:
			break;
	}
	return entity;
}

/*	Find a specific entity				*/
ENTITY *seekEntity(ENTITY *entity){
	if(entity == NULL) return NULL;
	if(ENTROOT == NULL) return NULL;
	for(ENTCURRENT = ENTROOT; ENTCURRENT != NULL; ENTCURRENT = ENTCURRENT->next){
		if(ENTCURRENT == entity) return ENTCURRENT;
	}
	return NULL;
}

/*	Spawn specified creature of level at location	*/
ENTITY *spawnCreature(ENTITY *list, CREATURESTATS *creature, unsigned int level, unsigned int y, unsigned int x){
	if(creature == NULL) return list;
	ENTITY *e = list;
	int slotCount = 0;
	size_t i;
	for(i = 0; i < 7; i++){
		if(creature->eqSlots[i] == true) slotCount++;
	}
	size_t s = sizeof(ENTITY) + sizeof(CREATURE) + (sizeof(EQUIPMENT) * slotCount) + (sizeof(_ITEM) * slotCount);
	if(list == NULL){
		e = (ENTITY *)calloc(1, s);
		if(e == NULL) return NULL;
		list = e;
	}
	else{
		while(e->next != NULL) e = e->next;
		e->next = (ENTITY *)calloc(1,s);
		if(e->next == NULL) return list;
		e = e->next;
	}
	if(!(e->ent = newCreature())){
		free(e);
		e = NULL;
		return list;
	}
	e->category = C_CREATURE;
	strcpy(e->name, creature->name);
	strcpy(e->shortDesc, creature->shortDesc);
	strcpy(e->longDesc, creature->longDesc);
	e->ch = creature->ch;
	e->color = creature->color;
	e->flags = creature->flags;
	e->locY = y;
	e->locX = x;
	setCreatureStats( (CREATURE *)e->ent, creature, genesis->floor);
	return list;
}

ENTITY *spawnItem(ENTITY *list, _ITEM *item, unsigned int level, unsigned int y, unsigned int x){
	if(item == NULL) return list;
	ENTITY *e = list;
	size_t s = sizeof(ENTITY) + sizeof(_ITEM);
	if(list == NULL){
		e = (ENTITY *)calloc(1, s);
		if(e == NULL) return NULL;
		list = e;
	}
	else{
		while(e->next != NULL) e = e->next;
		e->next = (ENTITY *)calloc(1,s);
		if(e->next == NULL) return list;
		e = e->next;
	}
	if(!(e->ent = newItem())){
		free(e);
		e = NULL;
		return list;
	}
	e->category = C_ITEM;
	e->locY = y;
	e->locX = x;
}

ENTITY *spawnObject(ENTITY *list, OBJECTSTATS *object, unsigned int level, unsigned int y, unsigned int x){
	if(object == NULL) return list;
	ENTITY *o = list;
	size_t s = sizeof(ENTITY) + sizeof(OBJECT);
	if(list == NULL){
		o = (ENTITY *)calloc(1, s);
		if(o == NULL) return NULL;
		list = o;
	}
	else{
		while(o->next != NULL) o = o->next;
		o->next = (ENTITY *)calloc(1,s);
		if(o->next == NULL) return list;
		o = o->next;
	}
	if(!(o->ent = newObject())){
		free(o);
		o = NULL;
		return list;
	}
	o->category = C_OBJECT;
	strcpy(o->name, object->name);
	strcpy(o->shortDesc, object->shortDesc);
	strcpy(o->longDesc, object->longDesc);
	o->color = object->color;
	o->flags = object->flags;
	o->locY = y;
	o->locX = x;
	setObjectStats( (OBJECT *)o->ent, object);
	( ((OBJECT *)o->ent)->isOpen ) ? (o->ch = object->chOpen) : (o->ch = object->chClosed);
	return list;
}

/*	Get a count of all currently active entities.	*/
unsigned int countEnt(){
	unsigned int count = 0;
	ENTCURRENT = ENTROOT;
	if(ENTROOT == NULL || ENTCURRENT == NULL) return 0;
	for(ENTCURRENT = ENTROOT; ENTCURRENT != NULL; ENTCURRENT = ENTCURRENT->next){
		count++;
	}
	return count;
}

void addFlag(ENTITY *e, int flag){
	if(!checkFlag(e, flag)){
		e->flags += flag;
	}
	return;
}

void delFlag(ENTITY *e, int flag){
	if(checkFlag(e, flag)){
		e->flags -= flag;
	}
	return;
}

/*	Check applied flags of specified entity		*/
bool checkFlag(ENTITY *e, int flag){
	if(flag > e->flags) return false;
	if(flag == e->flags) return true;
	int i = ARRAYSIZE(FLAG_ARRAY) - 1;
	int check = e->flags;
	while(i >= 0){
		if(flag == FLAG_ARRAY[i]){
			i--;
		}
		else{
			check -= FLAG_ARRAY[i];
			i--;
		}
	}
	if(check - flag == 0) return true;
	return false;
}

bool canHear(ENTITY *src, ENTITY *tgt){
	if(src == NULL || tgt == NULL){
		 return false;
	}
	if(checkArea(src, tgt, ((CREATURE *)src->ent)->WIS)){
//		FILE *log = fopen("log.txt", "a");
///		fprintf(log, "function canHear() src = %s, tgt = %s returning TRUE\n", src->name, tgt->name);
//		fclose(log);
		return true;
	}
	return false;
}

/*	Delete specified entity		*/
int delEnt(ENTITY *del){
	if(del == player) return 1;
	for(ENTCURRENT = ENTROOT; ENTCURRENT->next != del; ENTCURRENT = ENTCURRENT->next){
		if(ENTCURRENT == NULL) return 1;
	}
	if(ENTCURRENT->next->next == NULL){
		free(ENTCURRENT->next->ent);
		ENTCURRENT->next->ent = NULL;
		free(ENTCURRENT->next);
		ENTCURRENT->next = NULL;
	}
	else{
		ENTITY *temp = ENTCURRENT->next->next;
		free(ENTCURRENT->next->ent);
		ENTCURRENT->next->ent = NULL;
		free(ENTCURRENT->next);
		ENTCURRENT->next = NULL;
		ENTCURRENT->next = temp;
	}
	return ERR_NONE;
}

/*	Delete all entities		*/
ENTITY *delEntList(ENTITY *entity){
	if(entity == NULL) return NULL;
	while(entity != NULL){
		ENTITY *nextE = entity->next;
		switch(entity->category){
			case C_CREATURE:
				if( ((CREATURE *)entity->ent)->equipment != NULL){
					while( ((CREATURE *)entity->ent)->equipment != NULL ){
						EQUIPMENT *temp = (EQUIPMENT *)((CREATURE *)entity->ent)->equipment->next;
						free( ((CREATURE *)entity->ent)->equipment );
						((CREATURE *)entity->ent)->equipment = NULL;
						((CREATURE *)entity->ent)->equipment = temp;
					}
				}
				break;
			case C_OBJECT:
				if( ((OBJECT *)entity->ent)->contents != NULL){
					while( ((OBJECT *)entity->ent)->contents != NULL ) {
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
	ENTCURRENT = ENTROOT;
	for(ENTCURRENT; ENTCURRENT != NULL; ENTCURRENT = ENTCURRENT->next){
		if(ENTCURRENT->locY - VIEW->y + WINSTARTY >= WINSTARTY){
			attron(COLOR_PAIR(ENTCURRENT->color));
			mvaddch(ENTCURRENT->locY - VIEW->y + WINSTARTY, ENTCURRENT->locX - VIEW->x, ENTCURRENT->ch);
			attroff(COLOR_PAIR(ENTCURRENT->color));
		}
	}
//	Draw the player last, lest he end up beneath other entities.
	if(ENTROOT->locY - VIEW->y + WINSTARTY >= WINSTARTY){
		attron(COLOR_PAIR(player->color));
		mvaddch(player->locY - VIEW->y + WINSTARTY, player->locX - VIEW->x, player->ch);
		attroff(COLOR_PAIR(player->color));
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

void rollStats(CREATURE *creature){
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

void setClass(ENTITY *entity){
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
	rollStats( (CREATURE *)entity->ent );
}

void setName(ENTITY *entity){
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
		replaceAll(player->name, sizeof(player->name) / sizeof(player->name[0]));
		strcpy(player->name, eName);
		nameOK = true;
	}
}

/*	Add the player entity to the root of the entities list		*/
int initEnt(){
	ENTROOT = spawnCreature(ENTROOT, CREATURELIST, 1, 0, 0);
	if(ENTROOT == NULL) return ERR_MALLOC;
	player = ENTROOT;
	player->ent = ENTROOT->ent;
	return ERR_NONE;
}

bool seedCreature(int level, unsigned int ft){
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
		unsigned int s = getRand_i(0, creaturecount);
		ENTROOT = spawnCreature(ENTROOT, &seed[s], level, y, x);
	}
	free(seed);
	seed = NULL;
	return true;
}
#endif
