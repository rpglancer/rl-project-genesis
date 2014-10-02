#ifndef C_ENGINE
#define C_ENGINE
#include "../include/engine.h"

bool checkCombat(ENTITY *src, unsigned int dir){
	for(ENTCURRENT = ENTROOT; ENTCURRENT != NULL; ENTCURRENT = ENTCURRENT->next){
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

bool checkCreature(ENTITY *entity, unsigned int dir){
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

bool checkMove(ENTITY *entity, unsigned int dir){
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

bool checkObject(ENTITY *entity, unsigned int dir){
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

bool checkTile(ENTITY *entity, unsigned int dir){
	switch(dir){
		case DIR_NORTH:
			if(entity->locY == 0) return false;
			return TILE_INDEX[MAP[N(entity->locY, genesis->maxX, entity->locX)].tT].tPass;
		case DIR_EAST:
			if(entity->locX == genesis->maxX - 1) return false;
			return TILE_INDEX[MAP[E(entity->locY, genesis->maxX, entity->locX)].tT].tPass;
		case DIR_SOUTH:
			if(entity->locY == genesis->maxY - 1) return false;
			return TILE_INDEX[MAP[S(entity->locY, genesis->maxX, entity->locX)].tT].tPass;
		case DIR_WEST:
			if(entity->locX == 0) return false;
			return TILE_INDEX[MAP[W(entity->locY, genesis->maxX, entity->locX)].tT].tPass;
		default:
			return false;
	}
}

int didHit(CREATURE *src, CREATURE *tgt){
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

int getDmg(CREATURE *src, CREATURE *tgt){
	int dmg = 0;
/*
	EQUIPMENT *eq = src->equipment;
	while(eq != NULL){
		if(eq->slot == WEAR_WIELD_R){
			if(eq->item != NULL) break;
		}
		eq = eq->next;
	}
*/
	EQUIPMENT *eq = NULL;
	size_t i;
	for(i = 0; i < 7; i++){
		if(src->equipment[i].slot == WEAR_WIELD_R && src->equipment[i].item){
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

void doCombat(CREATURE *src, CREATURE *tgt){
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
	mvprintw(0, 0,"%.10s the %.10s", ((CREP)player->ent)->name, ((CREP)player->ent)->name);	// Playername the Title
	mvprintw(1, 0,"%.12s %.12s", ((CREP)player->ent)->name, PLAYERCLASS->name);	// Raceish Class
	attron(COLOR_PAIR(3));
	attron(A_BOLD);
	mvprintw(2, 0,"%3i/%3i", ((CREP)player->ent)->hp, ((CREP)player->ent)->hpMax);
	attroff(COLOR_PAIR(3));
	attron(COLOR_PAIR(4));
	mvprintw(2, 8,"%3i/%3i", ((CREP)player->ent)->mp, ((CREP)player->ent)->mpMax);
	attroff(COLOR_PAIR(4));
	attron(COLOR_PAIR(5));
	mvprintw(2, 16,"%3i/%3i", ((CREP)player->ent)->ap, ((CREP)player->ent)->apMax);
	attroff(COLOR_PAIR(5));
	attroff(A_BOLD);
	mvprintw(0, 26, "STR:%2i INT:%2i", ((CREP)player->ent)->STR, ((CREP)player->ent)->INT);
	mvprintw(1, 26, "CON:%2i WIS:%2i", ((CREP)player->ent)->CON, ((CREP)player->ent)->WIS);
	mvprintw(2, 26, "DEX:%2i CHA:%2i", ((CREP)player->ent)->DEX, ((CREP)player->ent)->CHA);
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
//			loadCurrent();
//			refresh();
//			engineInput();
			break;
		case 'o':
			selectObject(MSG_OPEN);
			break;
		case 'q':
			genesis->engineStatus = ENGINESTOP;
			break;
		case 's':
//			saveCurrent();
//			engineInput();
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

bool checkArea(ENTITY *src, ENTITY *tgt, unsigned int rad){
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
				getItem( (CREATURE *)MSGCURRENT->source->ent , (_ITEM *)MSGCURRENT->target->ent);
				if(MSGCURRENT->target){
					MSGCURRENT->target->ent = NULL;
					delEnt(MSGCURRENT->target);
				}
				break;
			case MSG_DROP:
				break;
			case MSG_OPEN:
				doOpen(seekEntity(MSGCURRENT->target));
				//ENTCURRENT = seekEntity(MSGCURRENT->target);
				//if(ENTCURRENT != NULL){
				//	pushHistory(MSGCURRENT->source, MSGCURRENT->target, MSGCURRENT->msgType, MSGCURRENT->msgFlag);
				//	ENTCURRENT = doOpen(ENTCURRENT);
				//}
				break;
			case MSG_CLOSE:
				doClose(seekEntity(MSGCURRENT->target));
				//ENTCURRENT = seekEntity(MSGCURRENT->target);
				//if(ENTCURRENT != NULL){
				//	pushHistory(MSGCURRENT->source, MSGCURRENT->target, MSGCURRENT->msgType, MSGCURRENT->msgFlag);
				//	ENTCURRENT = doClose(ENTCURRENT);
				//}
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
	if(ARTLIST == NULL || CLASSLIST == NULL ||
		CREATURELIST == NULL || OBJECTLIST == NULL) return ERR_MALLOC;
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
	if(initQueue() != ERR_NONE) return ERR_MALLOC;
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
//	int i;
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

void displayEq(){
	char select = '>';
	bool eqOK = false;
	int cursY = 1;
	int cursX = 0;
	int y = 1;
	WINDOW *win_eq = newWindow(10, 20, MAXHEIGHT/2, MAXWIDTH / 2);
	EQUIPMENT *cur;
	while(!eqOK){
		wclear(win_eq);
		mvwprintw(win_eq, 0, 10, "-- Equipment --");
		mvwaddch(win_eq, cursY, cursX, select);
		wrefresh(win_eq);
		genesis->ch = getch();
		switch(genesis->ch){
			case 27:
				eqOK = true;
				break;
			case 'q':
				eqOK = true;
				break;
			case KEY_DOWN:
				if(cursY < 7) ++cursY;
				break;
			case KEY_UP:
				if(cursY > 1) --cursY;
				break;
		}
	}
	delWindow(win_eq);
	refresh();
	return;
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
