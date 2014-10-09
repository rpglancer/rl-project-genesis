#ifndef C_ENGINE
#define C_ENGINE
#include "../include/engine.h"

bool checkCombat(ENTP src, unsigned int dir){
	for(ENTCURRENT = ENTROOT; ENTCURRENT != NULL; ENTCURRENT = ENTCURRENT->next){
		if(ENTCURRENT == src) continue;
		if(src->category == C_CREATURE && ENTCURRENT->category == C_CREATURE){
			if(src->locX == ENTCURRENT->locX - 1 &&
			src->locY == ENTCURRENT->locY &&
			dir == DIR_EAST) return true;

			else if(src->locX == ENTCURRENT->locX + 1 &&
			src->locY == ENTCURRENT->locY &&
			dir == DIR_WEST) return true;

			else if(src->locY == ENTCURRENT->locY - 1 &&
			src->locX == ENTCURRENT->locX &&
			dir == DIR_SOUTH) return true;

			else if(src->locY == ENTCURRENT->locY + 1 &&
			src->locX == ENTCURRENT->locX &&
			dir == DIR_NORTH) return true;
		}
	}
	return false;
}

bool checkCreature(ENTP entity, uint dir){
	if(entity == NULL) return false;
	if(ENTCURRENT == NULL) return true;
	switch(dir){
		case DIR_NORTH:
			if(ENTCURRENT->locY == entity->locY - 1 && ENTCURRENT->locX == entity->locX) return false;
			break;
		case DIR_EAST:
			if(ENTCURRENT->locY == entity->locY && ENTCURRENT->locX == entity->locX + 1) return false;
			break;
		case DIR_SOUTH:
			if(ENTCURRENT->locY == entity->locY + 1 && ENTCURRENT->locX == entity->locX) return false;
			break;
		case DIR_WEST:
			if(ENTCURRENT->locY == entity->locY && ENTCURRENT->locX == entity->locX - 1) return false;
			break;
	}
	return true;
}

bool checkMove(ENTP entity, uint dir){
	if(entity == NULL) return false;
	if(!checkTile(entity, dir))return false;
	for(ENTCURRENT = ENTROOT; ENTCURRENT != NULL; ENTCURRENT = ENTCURRENT->next){
		switch(ENTCURRENT->category){
			case C_CREATURE:
				if(!checkCreature(entity, dir)) return false;
				break;
			case C_OBJECT:
				if(!checkObject(entity, dir)) return false;
				break;
		}
	}
	return true;
}

bool checkObject(ENTP entity, uint dir){
	if(entity == NULL) return false;
	if(ENTCURRENT == NULL) return true;
	switch(dir){
		case DIR_NORTH:
			if(ENTCURRENT->locY == entity->locY - 1 && ENTCURRENT->locX == entity->locX){
				return ((OBJECT *)ENTCURRENT->ent)->isPassable;
			}
			break;
		case DIR_EAST:
			if(ENTCURRENT->locX == entity->locX + 1 && ENTCURRENT->locY == entity->locY){
				return ((OBJECT *)ENTCURRENT->ent)->isPassable;
			}
			break;
		case DIR_SOUTH:
			if(ENTCURRENT->locY == entity->locY + 1 && ENTCURRENT->locX == entity->locX){
				return ((OBJECT *)ENTCURRENT->ent)->isPassable;
			}
			break;
		case DIR_WEST:
			if(ENTCURRENT->locX == entity->locX - 1 && ENTCURRENT->locY == entity->locY){
				return ((OBJECT *)ENTCURRENT->ent)->isPassable;
			}
			break;
	}
	return true;
}

/*	Should work without the if statements		*/
bool checkTile(ENTP entity, uint dir){
	switch(dir){
		case DIR_NORTH:
			return TILE_INDEX[MAP[N(entity->locY, genesis->maxX, entity->locX)].tT].tPass;
		case DIR_EAST:
			return TILE_INDEX[MAP[E(entity->locY, genesis->maxX, entity->locX)].tT].tPass;
		case DIR_SOUTH:
			return TILE_INDEX[MAP[S(entity->locY, genesis->maxX, entity->locX)].tT].tPass;
		case DIR_WEST:
			return TILE_INDEX[MAP[W(entity->locY, genesis->maxX, entity->locX)].tT].tPass;
		default:
			return false;
	}
}

int didHit(CREP src, CREP tgt){
	int hit = rollDice(1, 20, 0);
	if(hit == 1) return MISS;
	if(hit == 20){
		int crit = rollDice(1, 20, 0);
		if(crit == 20) return CRIT;
		return HIT;
	}
	hit = hit + src->STR;
	if(hit > tgt->BAC) return HIT;
	return MISS;
}

int getDmg(CREP src, CREP tgt){
	int dmg = 0;
	EQUIPMENT *eq = NULL;
	size_t i;
	for(i = 0; i < 7; i++){
		if(src->equipment[i].slot == WEAR_WIELD_R && !isNull(src->equipment[i].item)){
			eq = &src->equipment[i];
		}
	}
	if(eq == NULL){
		// Unarmed
		dmg = rollDice(1, 4, 0) + src->STR;
	}
	else{
		_ITEM *item = eq->item;
		dmg = rollDice(item->itemDmgRoll, item->itemDmgSize, item->itemDmgMod) + src->STR;
	}
	return dmg;
}

/* 	This should be moved to creature.c		*/
void displayInventory(CREP creature){
	if(creature == NULL) return;
	char select = '>';
	uchar cursorWin = 0;
	int cursY = 1;
	int cursX = 0;
	int action = ACTION_NONE;
	bool invOK = false;
	WINDOW *inventory = newWindow(8, 20, 11, 20);		// y, x, h, w
	WINDOW *equipment = newWindow(8, 40, 11, 20);		// y, x, h, w
	while(!invOK){
		wclear(equipment);
		wclear(inventory);
		mvwprintw(inventory, 0, 0, "-- Inventory --");
		size_t i;
		int y;
		mvwprintw(equipment, 0, 0, "-- Equipment --");
		for(i = 0, y = 1; i < 10; i++, y++){
			if(creature->inventory[i].itemType != ITEM_NONE) mvwprintw(inventory, y, 1, "%s", creature->inventory[i].name);
			else mvwprintw(inventory, y, 1, "[None]");
		}
		wrefresh(inventory);
		for(i = 0, y = 1; i < 7; i++, y++){
			EQUIPMENT *eq = &creature->equipment[i];
			if(eq->item != NULL) mvwprintw(equipment, y, 1, "%s", eq->item->itemName);
			else mvwprintw(equipment, y, 1, "[Empty]");
		}
		(cursorWin == 0) ? (mvwaddch(inventory, cursY, cursX, select)) : (mvwaddch(equipment,cursY, cursX, select));
		wrefresh(equipment);
		wrefresh(inventory);
		switch(getch()){
			case '\n':
				if(cursorWin == 0) action = contextMenu(&creature->inventory[cursY - 1], getbegy(inventory) + cursY, getbegx(inventory) + 10);
				if(cursorWin == 1) action = contextMenu(creature->equipment[cursY - 1].item, getbegy(equipment) + cursY, getbegx(equipment) + 10);
				if(action != ACTION_NONE) invOK = true;
				break;
			case 'i':
				invOK = true;
				break;
			case KEY_DOWN:
				if (cursorWin == 0 && cursY < 10) ++cursY;
				else if (cursorWin == 1 && cursY < 7) ++ cursY;
				break;
			case KEY_UP:
				if(cursY > 1) --cursY;
				break;
			case KEY_RIGHT:
				if(cursorWin == 0)++cursorWin;
				break;
			case KEY_LEFT:
				if(cursorWin == 1)--cursorWin;
				break;
			default:
				break;
		}
	}
	if(action == ACTION_EQUIP) pushMsg(player, NULL, MSG_EQUIP, cursY - 1);
	if(action == ACTION_DROP) pushMsg(player, NULL, MSG_DROP, cursY - 1);
	if(action == ACTION_REMOVE) pushMsg(player, NULL, MSG_REMOVE, cursY - 1);
	delWindow(equipment);
	delWindow(inventory);
	refresh();
}

void doCombat(CREP src, CREP tgt){
	switch(didHit(src, tgt)){
		case CRIT:
			tgt->hp -= (getDmg(src, tgt) * 2);
			break;
		case HIT:
			tgt->hp -= getDmg(src, tgt);
			break;
		case MISS:
			break;
	}
}

void drawHeader(){
	CREP pc = (CREP)player->ent;
	mvprintw(0, 0,"%.10s the %.10s", pc->name, pc->name);		// Playername the Title
	mvprintw(1, 0,"%.12s %.12s", pc->name, PLAYERCLASS->name);	// Raceish Class
	attron(COLOR_PAIR(3));
	attron(A_BOLD);
	mvprintw(2, 0,"%3i/%3i", pc->hp, pc->hpMax);
	attroff(COLOR_PAIR(3));
	attron(COLOR_PAIR(4));
	mvprintw(2, 8,"%3i/%3i", pc->mp, pc->mpMax);
	attroff(COLOR_PAIR(4));
	attron(COLOR_PAIR(5));
	mvprintw(2, 16,"%3i/%3i", pc->ap, pc->apMax);
	attroff(COLOR_PAIR(5));
	attroff(A_BOLD);
	mvprintw(0, 26, "STR:%2i INT:%2i", pc->STR, pc->INT);
	mvprintw(1, 26, "CON:%2i WIS:%2i", pc->CON, pc->WIS);
	mvprintw(2, 26, "DEX:%2i CHA:%2i", pc->DEX, pc->CHA);
	printHistory();
}

void engineDraw(){
	drawMap();
	drawEnt();
	drawHeader();
}

void engineInput(){
	genesis->ch = getch();
	switch(genesis->ch){
		case 'c':
			selectObject(MSG_CLOSE);
			break;
		case 'd':
			pushMsg(player, NULL, MSG_DROP, selectItem( (CREATURE *)player->ent));
			// Select item to drop
			break;
		case 'g':
			selectObject(MSG_GET);
			break;
		case 'i':
			displayInventory((CREATURE *)player->ent);
			break;
		case 'l':
			// Load
			break;
		case 'o':
			selectObject(MSG_OPEN);
			break;
		case 'q':
			genesis->engineStatus = ENGINESTOP;
			break;
		case 's':
			// Save
			break;
		case KEY_LEFT:
			if(checkCombat(player, DIR_WEST)){
				pushMsg(player,ENTCURRENT,MSG_ATTACK, 0);
				break;
			}
			pushMsg(player, NULL, MSG_MOVE, DIR_WEST);
			break;
		case KEY_RIGHT:
			if(checkCombat(player, DIR_EAST)){
				pushMsg(player, ENTCURRENT, MSG_ATTACK, 0);
				break;
			}
			pushMsg(player, NULL, MSG_MOVE, DIR_EAST);
			break;
		case KEY_UP:
			if(checkCombat(player, DIR_NORTH)){
				pushMsg(player, ENTCURRENT, MSG_ATTACK, 0);
				break;
			}
			pushMsg(player, NULL, MSG_MOVE, DIR_NORTH);
			break;
		case KEY_DOWN:
			if(checkCombat(player, DIR_SOUTH)){
				pushMsg(player, ENTCURRENT, MSG_ATTACK, 0);
				break;
			}
			pushMsg(player, NULL, MSG_MOVE, DIR_SOUTH);
			break;
	}
}

void engineShutdown(){
	MSGQUEUE = freeMsgList(MSGQUEUE);
	ENTROOT = delEntList(ENTROOT);
	OBJECTLIST = delObjectList(OBJECTLIST);
	CREATURELIST = delCreatureList(CREATURELIST);
	ARTLIST = delArtList(ARTLIST);
	MAP = mapFree(MAP);
	free(genesis);
	genesis = NULL;
	free(VIEW);
	VIEW = NULL;
}

bool checkArea(ENTP src, ENTP tgt, uint rad){
	int tempOffset = 0;
	int y, x;
	for(y = tgt->locY - rad; y < tgt->locY + rad; y++){
		for(x = tgt->locX - tempOffset; x < tgt->locX + tempOffset; x++){
			if(src->locY == y && src->locX == x)return true;
		}
		if(y < tgt->locY) tempOffset++;
		else if(y > tgt->locY) tempOffset--;
		else tempOffset = rad;
	}
	return false;
}

/*
*	This AI is not complete in any sense of the word.
*/
void quickAI(){
	if(ENTROOT->next == NULL) return;
	int bestH = 10000;
	ENTITY *source, *target, *bestE;
	for(source = ENTROOT->next; source != NULL; source = source->next){
		if(source->category != C_CREATURE) continue;
		for(target = ENTROOT; target != NULL; target = target->next){
			if(target != player) continue;
			if(target->category != C_CREATURE) continue;
			if(target == source) continue;
			bestE = NULL;
			if(canHear(source, target)){
				int h = getHeur(&MAP[CM(source->locY, genesis->maxX, source->locY)], &MAP[CM(target->locY, genesis->maxX, target->locX)]);
				if(h < bestH){
					bestH = h;
					bestE = target;
				}
			}
			unsigned int dir = 0;
			if(bestE == NULL){
				dir = getRand_i(1,5);
				if(checkCombat(source, dir)){
					bestE = ENTCURRENT;
					pushMsg(source, bestE, MSG_ATTACK, 0);
				}
				else pushMsg(source, NULL, MSG_MOVE, dir);
			}
			else{
				dir = findPath(&MAP[C(source->locY, genesis->maxX, source->locX)], &MAP[C(bestE->locY, genesis->maxX, bestE->locX)]);
				if(checkCombat(source, dir)){
					bestE = ENTCURRENT;
					pushMsg(source, bestE, MSG_ATTACK, 0);
				}
				else pushMsg(source, NULL, MSG_MOVE, dir);
			}
		}
	}
	return;
}

int contextMenu(ITEMP item, uint y, uint x){
	if(isNull(item)) return ACTION_NONE;
	if(item->itemType == ITEM_NONE) return ACTION_NONE;
	char select = '>';
	uchar maxSelect = 0;
	uchar cursY = 1;
	uchar cursX = 0;
	uchar action[7];
	bool contextOK = false;
	WINDOW *context = newWindow(y,x,8,8);
	while(!contextOK){
		wclear(context);
		mvwprintw(context, 0, 1, "ACTION:");
		mvwaddch(context, cursY, cursX, select);
		switch(item->itemType){
			case ITEM_CONSUMABLE:
				break;
			case ITEM_KEYITEM:
				break;
			case ITEM_WEAPON:
				if(isEquipped((CREP)player->ent, item)){
					maxSelect = 3;
					mvwprintw(context, 1, 1, "none");
					mvwprintw(context, 2, 1, "remove");
					mvwprintw(context, 3, 1, "exam");
					action[0] = ACTION_NONE;
					action[1] = ACTION_REMOVE;
					action[2] = ACTION_EXAM;
				}
				if(isCarried((CREP)player->ent, item)){
					maxSelect = 5;
					mvwprintw(context, 1, 1, "none");
					mvwprintw(context, 2, 1, "move");
					mvwprintw(context, 3, 1, "equip");
					mvwprintw(context, 4, 1, "drop");
					mvwprintw(context, 5, 1, "exam");
					action[0] = ACTION_NONE;
					action[1] = ACTION_MOVE;
					action[2] = ACTION_EQUIP;
					action[3] = ACTION_DROP;
					action[4] = ACTION_EXAM;
				}
				break;
			case ITEM_ARMOR:
				break;
			case ITEM_ACC:
				break;
		}
		wrefresh(context);
		switch(getch()){
			case KEY_DOWN:
				if(cursY < maxSelect) ++cursY;
				break;
			case KEY_UP:
				if(cursY > 1) --cursY;
				break;
			case '\n':
				delWindow(context);
				return action[cursY - 1];
			default:
				break;
		}
	}
}
int engineRun(){
	while(genesis->engineStatus != ENGINESTOP){
		engineDraw();
		refresh();
		engineInput();
		if(genesis->engineStatus == ENGINESTOP) break;
		quickAI();
		engineUpdate();
		clear();
	}
}

int engineUpdate(){
	for(MSGCURRENT = MSGQUEUE; MSGCURRENT != NULL; MSGCURRENT = MSGCURRENT->next){
		if(!checkFlag(MSGCURRENT->source, FLAG_ISALIVE)){
			popMsg(MSGCURRENT);
			continue;
		}
		switch(MSGCURRENT->msgType){
			case MSG_MOVE:
				if(!checkFlag(MSGCURRENT->source, FLAG_ISMOBILE)) continue;
				if(checkMove(MSGCURRENT->source, MSGCURRENT->msgFlag)){
					switch(MSGCURRENT->msgFlag){
						case DIR_NORTH:
							MSGCURRENT->source->locY -= 1;
							break;
						case DIR_EAST:
							MSGCURRENT->source->locX += 1;
							break;
						case DIR_SOUTH:
							MSGCURRENT->source->locY += 1;
							break;
						case DIR_WEST:
							MSGCURRENT->source->locX -= 1;
							break;
					}
					if(MSGCURRENT->source == player) pushHistory(MSGCURRENT->source, MSGCURRENT->target, MSGCURRENT->msgType, MSGCURRENT->msgFlag);
					if(MSGCURRENT->source == player) setViewCurrent(player->locY, player->locX);
				}
				break;
                        case MSG_ATTACK:
				doCombat( (CREATURE *)MSGCURRENT->source->ent, (CREATURE *)MSGCURRENT->target->ent  );
				pushHistory(MSGCURRENT->source, MSGCURRENT->target, MSGCURRENT->msgType, 0);
				if(MSGCURRENT->target != player){
					if(((CREATURE *)MSGCURRENT->target->ent)->hp <= 0) delFlag( MSGCURRENT->target, FLAG_ISALIVE);
				}
				break;
			case MSG_GET:
				getItem( MSGCURRENT->source, MSGCURRENT->target);
				break;
			case MSG_DROP:
				dropItem(MSGCURRENT->source, &((CREP)MSGCURRENT->source->ent)->inventory[MSGCURRENT->msgFlag]);
				break;
			case MSG_EQUIP:{
				CREP c = (CREP)MSGCURRENT->source->ent;
				ITEMP i = &c->inventory[MSGCURRENT->msgFlag];
				equipItem(c,i);
				break;
			}
			case MSG_REMOVE:{
				CREP c = (CREP)MSGCURRENT->source->ent;
				ITEMP i = c->equipment[MSGCURRENT->msgFlag].item;
				removeItem(c,i);
			}
			case MSG_OPEN:
				doOpen(seekEntity(MSGCURRENT->target));
				break;
			case MSG_CLOSE:
				doClose(seekEntity(MSGCURRENT->target));
				break;
			default:
				break;
		}
	popMsg(MSGCURRENT);
	}
	for(ENTCURRENT = ENTROOT; ENTCURRENT != NULL; ENTCURRENT = ENTCURRENT->next){
		if(ENTCURRENT->category != C_CREATURE)continue;
		if(!checkFlag(ENTCURRENT, FLAG_ISALIVE)) delEnt(ENTCURRENT);
	}
}

void setColors(){
	start_color();
	init_pair(1, COLOR_CYAN, COLOR_BLACK);		// PC
	init_pair(2, COLOR_WHITE, COLOR_BLACK);		// STD Color
	init_pair(3, COLOR_RED, COLOR_BLACK);		// HP
	init_pair(4, COLOR_BLUE, COLOR_BLACK);		// MP
	init_pair(5, COLOR_GREEN, COLOR_BLACK);		// AP
	return;
}

int loadAssets(){
	LL *artLoad = loadList(TYPE_ART);
	ARTLIST = loadArt(artLoad);
	LL *creatureLoad = loadList(TYPE_CREATURE);
	CREATURELIST = loadCreature(creatureLoad);
	LL *itemLoad = loadList(TYPE_ITEM);
	ITEMLIST = loadItem(itemLoad);
	LL *objectLoad = loadList(TYPE_OBJECT);
	OBJECTLIST = loadObject(objectLoad);
	LL *classLoad = loadList(TYPE_CLASS);
	CLASSLIST = loadClass(classLoad);
	if(isNull(ARTLIST) || isNull(CLASSLIST) ||
		isNull(CREATURELIST) || isNull(OBJECTLIST)) return ERR_MALLOC;
	artLoad = delLoadList(artLoad);
	classLoad = delLoadList(classLoad);
	creatureLoad = delLoadList(creatureLoad);
	itemLoad = delLoadList(itemLoad);
	objectLoad = delLoadList(objectLoad);
	return ERR_NONE;
}

int newGame(){
	if(genesis == NULL) return ERR_MALLOC;
	genesis->engineStatus = ENGINERUN;
	genesis->floor = 1;
	genesis->maxY = 0;
	genesis->maxX = 0;
	VIEW = (struct RECT *)malloc(sizeof(struct RECT));
	if(VIEW == NULL) return ERR_MALLOC;
	if(initEnt() != ERR_NONE) return ERR_MALLOC;
	// Call player generator here
	newPlayer();
//	if(initQueue() != ERR_NONE) return ERR_MALLOC;
	if(initHistory() != ERR_NONE) return ERR_MALLOC;
	if(initMap(100, 100, 5, TILE_ROCKFLOOR, TILE_WALL) != ERR_NONE) return ERR_MALLOC;
	setViewCurrent(player->locY, player->locX);
	return ERR_NONE;
}

void newPlayer(){
	bool playerOK = false;
	while(!playerOK){
		setName(player);
//		setRace(player);
		setClass(player);
		playerOK = true;
	}
}

int initEngine(){
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	setColors();
	genesis = (ENGINE *)calloc(1, sizeof(ENGINE));
	if(genesis == NULL)return ERR_MALLOC;
	if(loadAssets() != ERR_NONE) return ERR_MALLOC;
	return ERR_NONE;
}

int titleScreen(){
	while(genesis->engineStatus == ENGINETITLE){
		drawArt("title", 0, 11);
		mvaddstr(14,45, "version 0.3.0a");
		mvaddstr(18,36, "(N)ew Game");
		mvaddstr(19,36, "(L)oad Game");
		mvaddstr(20,36, "(Q)uit Game");
		refresh();
		genesis->ch = getch();
		switch(genesis->ch){
			case 'n':
				clear();
				if(newGame() == ERR_NONE) return ERR_NONE;
				else return ERR_MALLOC;
			case 'q':
				return ERR_SHUTDOWN;
			default:
				break;
		}
	}
	return;
}

void loadCurrent(){
	FILE *f;
	f = fopen("debugsave.txt","r");
	if(f == NULL)return;
	fread(genesis, sizeof(ENGINE), 1, f);
	// Need to free the existing map and reallocate for the new one here
	fread(MAP, sizeof(struct TILE) * (genesis->maxY * genesis->maxX), 1, f);
	fread(VIEW, sizeof(struct RECT), 1, f);
	delEntList(ENTROOT);
	ENTROOT = (ENTITY *)malloc(sizeof(ENTITY));
	if(ENTROOT == NULL)return;
	fread(ENTROOT, sizeof(ENTITY), 1, f);
	ENTROOT->next = NULL;
	ENTROOT->ent = NULL;
	if(ENTROOT->category == C_CREATURE){
		ENTROOT = realloc(ENTROOT, sizeof(ENTITY) + sizeof(CREATURE));
		if(ENTROOT == NULL) return;
		ENTROOT->ent = (CREATURE *)malloc(sizeof(CREATURE));
		if(ENTROOT->ent == NULL)return;
		fread(ENTROOT->ent, sizeof(CREATURE), 1, f);
		ENTCURRENT = ENTROOT;
	}
	size_t i;
	for(i = 1; i < genesis->entC; i++){
		ENTCURRENT = ENTROOT;
		while(ENTCURRENT->next != NULL) ENTCURRENT = ENTCURRENT->next;
		ENTCURRENT->next = (ENTITY *)malloc(sizeof(ENTITY));
		if(ENTCURRENT->next == NULL)return;
		ENTCURRENT = ENTCURRENT->next;
		fread(ENTCURRENT, sizeof(ENTITY), 1, f);
		ENTCURRENT->next = NULL;
		ENTCURRENT->ent = NULL;
		switch(ENTCURRENT->category){
			case C_CREATURE:
				ENTCURRENT = realloc(ENTCURRENT, sizeof(ENTITY) + sizeof(CREATURE));
				if(ENTCURRENT == NULL)return;
				ENTCURRENT->ent = (CREATURE *)malloc(sizeof(CREATURE));
				if(ENTCURRENT->ent == NULL)return;
				fread(ENTCURRENT->ent, sizeof(CREATURE), 1, f);
				break;
			case C_OBJECT:
				ENTCURRENT = realloc(ENTCURRENT, sizeof(ENTITY) + sizeof(OBJECT));
				if(ENTCURRENT == NULL)return;
				ENTCURRENT->ent = (OBJECT *)malloc(sizeof(OBJECT));
				if(ENTCURRENT->ent == NULL)return;
				fread(ENTCURRENT->ent, sizeof(OBJECT), 1, f);
				break;
			default:
				free(ENTCURRENT);
				ENTCURRENT = NULL;
				break;
		}
	}
	player = ENTROOT;
	player->ent = ENTROOT->ent;
	fclose(f);
}

/*
void saveCurrent(){
	FILE *g;
	g = fopen("plaintext.txt","w");
	if(g == NULL) return;
	fprintf(g, "<ENGINE>\n");
	fprintf(g, "FLOOR: %i\n",genesis->floor);
	fprintf(g, "STATUS: %i\n",genesis->engineStatus);
	fprintf(g, "MAXY: %i\n",genesis->maxY);
	fprintf(g, "MAXX: %i\n",genesis->maxX);
	fprintf(g, "ENTC: %i\n",genesis->entC);
	fprintf(g, "</ENGINE>\n");
	size_t i;
	fprintf(g, "<MAP>\n");
	for(i = 0; i < genesis->maxY * genesis->maxX; i++){
	fprintf(g, "%i,%i,%i ",MAP[i].tY, MAP[i].tX, MAP[i].tT);
	if(i != 0 && i % 99 == 0) fprintf(g,"\n");
	}
	fprintf(g, "</MAP>\n");
	for(ENTCURRENT = ENTROOT; ENTCURRENT != NULL; ENTCURRENT = ENTCURRENT->next){
		fprintf(g,"<ENTITY>\n");
		fprintf(g,"%s, %c, %i\n", ENTCURRENT->name, ENTCURRENT->ch, ENTCURRENT->color);
		fprintf(g,"%i, %i, %i\n", ENTCURRENT->locY, ENTCURRENT->locX, ENTCURRENT->flags);
		switch(ENTCURRENT->category){
			case C_CREATURE:
				fprintf(g,"\t<CREATURE>\n");
				fprintf(g,"\t%i, %i\n", ((CREATURE *)ENTCURRENT->ent)->hp, ((CREATURE *)ENTCURRENT->ent)->hpMax);
				fprintf(g,"\t%i, %i\n", ((CREATURE *)ENTCURRENT->ent)->mp, ((CREATURE *)ENTCURRENT->ent)->mpMax);
				fprintf(g,"\t%i, %i\n", ((CREATURE *)ENTCURRENT->ent)->ap, ((CREATURE *)ENTCURRENT->ent)->apMax);
				fprintf(g,"\t%i, %i\n", ((CREATURE *)ENTCURRENT->ent)->STR, ((CREATURE *)ENTCURRENT->ent)->CON);
				fprintf(g,"\t%i, %i\n", ((CREATURE *)ENTCURRENT->ent)->DEX, ((CREATURE *)ENTCURRENT->ent)->INT);
				fprintf(g,"\t%i, %i\n", ((CREATURE *)ENTCURRENT->ent)->WIS, ((CREATURE *)ENTCURRENT->ent)->CHA);
				fprintf(g,"\t%i, %i\n", ((CREATURE *)ENTCURRENT->ent)->atk, ((CREATURE *)ENTCURRENT->ent)->def);
				fprintf(g,"\t%i, %i\n", ((CREATURE *)ENTCURRENT->ent)->level, ((CREATURE *)ENTCURRENT->ent)->level);
				fprintf(g,"\t</CREATURE>\n");
				break;
			case C_ITEM:
				break;
			case C_OBJECT:
				fprintf(g,"\t<OBJECT>\n");
				fprintf(g,"\t%i, %i\n", ((OBJECT *)ENTCURRENT->ent)->isOpen, ((OBJECT *)ENTCURRENT->ent)->isPassable);
				fprintf(g,"\t%i, %i\n", ((OBJECT *)ENTCURRENT->ent)->isLocked, ((OBJECT *)ENTCURRENT->ent)->keyNum);
				fprintf(g,"\t</OBJECT>\n");
		}
	}
	fclose(g);
	FILE *f;
	f = fopen("debugsave.txt","w");
	if(f == NULL) return;
	genesis->entC = countEnt();
	fwrite(genesis, sizeof(ENGINE), 1, f);
	fwrite(MAP, sizeof(struct TILE) * (genesis->maxY * genesis->maxX), 1, f);
	fwrite(VIEW, sizeof(struct RECT), 1, f);
	for(ENTCURRENT = ENTROOT; ENTCURRENT != NULL; ENTCURRENT = ENTCURRENT->next){
		fwrite(ENTCURRENT, sizeof(ENTITY), 1, f);
		switch(ENTCURRENT->category){
			case C_CREATURE:
				fwrite( ENTCURRENT->ent, sizeof(CREATURE), 1, f);
				break;
			case C_OBJECT:
				fwrite( ENTCURRENT->ent, sizeof(OBJECT), 1, f);
			default:
				break;
		}
	}
	fclose(f);
}
*/

void selectObject(unsigned int msg){
	bool doSelect = true;
	unsigned int cursY, cursX;
	cursY = player->locY - VIEW->y + WINSTARTY;
	cursX = player->locX - VIEW->x;
	curs_set(1);
	move(cursY, cursX);
	while(doSelect){
		refresh();
		genesis->ch = getch();
		switch(genesis->ch){
			case 27:	// Escape key
				curs_set(0);
				doSelect = false;
				break;
			case KEY_UP:
				if(cursY > WINSTARTY) move(--cursY, cursX);
				break;
			case KEY_RIGHT:
				if(cursX < MAXWIDTH - 1)move(cursY, ++cursX);
				break;
			case KEY_DOWN:
				if(cursY < MAXHEIGHT - 1)move(++cursY, cursX);
				break;
			case KEY_LEFT:
				if(cursX > 0)move(cursY, --cursX);
				break;
			case '\n':
				for(ENTCURRENT = ENTROOT; ENTCURRENT != NULL; ENTCURRENT = ENTCURRENT->next){
					if(ENTCURRENT->locY == cursY + VIEW->y - WINSTARTY &&
					ENTCURRENT->locX == cursX + VIEW->x){
						switch(msg){
							case MSG_CLOSE:
								pushMsg(player, ENTCURRENT, msg, 0);
								break;
							case MSG_GET:
								pushMsg(player, ENTCURRENT, msg, 0);
								break;
							case MSG_OPEN:
								pushMsg(player, ENTCURRENT, msg, 0);
								break;
							default:
								break;
						}
					}
				}
				curs_set(0);
				doSelect = false;
				break;
			default:
				move(cursY, cursX);
				break;
		}
	}
}

void setViewCurrent(int y, int x){
	int maxX = MAPTAIL->tX + 1;
	int maxY = MAPTAIL->tY + 1;
	if(y < 0 || x < 0 || y > MAPTAIL->tY || x > MAPTAIL->tX) return;
	if(y > MAXHEIGHT / 2 - WINSTARTY){
		VIEW->y = y - (MAXHEIGHT/2) + WINSTARTY;
		if(VIEW->y > maxY - MAXHEIGHT + WINSTARTY){
			VIEW->y = maxY - MAXHEIGHT + WINSTARTY;
		}
	}
	else VIEW->y = 0;
	if(x > MAXWIDTH / 2){
		VIEW->x = x - (MAXWIDTH/2);
		if(VIEW->x > maxX - MAXWIDTH){
			VIEW->x = maxX - MAXWIDTH;
		}
	}
	else VIEW->x = 0;
}
#endif
