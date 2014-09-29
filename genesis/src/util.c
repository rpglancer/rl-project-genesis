#ifndef C_UTIL
#define C_UTIL
#include "../include/util.h"

ART *addArt(ART *list){
	if(list == NULL){
		list = (ART *)calloc(1, sizeof(ART));
		if(list == NULL) return NULL;
		return list;
	}
	else{
		ART *temp = list;
		while(temp->next != NULL) temp = temp->next;
		temp->next = (ART *)calloc(1, sizeof(ART));
		if(temp->next == NULL) return NULL;
		return list;
	}
}

ART *delArtList(ART *list){
	if(list == NULL) return NULL;
	while(list != NULL){
		ART *temp = list->next;
		free(list);
		list = NULL;
		list = temp;
	}
	return list;
}

ART *loadArt(LL *list){
	if(list == NULL) return NULL;
	LL *l;
	ART *art = NULL;
	ART *a = NULL;
	for(l = list; l != NULL; l = l->next){
		FILE *load;
		int i;
		char check;
		load = fopen(l->fileToLoad, "r");
		if(load == NULL) return NULL;
		art = addArt(art);
		a = art;
		while(a->next != NULL) a = a->next;
		fscanf(load, "%25s", a->name);
		fscanf(load, "%i %i", &a->sizeY, &a->sizeX);
		check = fgetc(load);
		a = realloc(a, sizeof(ART) + sizeof(char) * (a->sizeY * a->sizeX) + sizeof(char) * (a->sizeY * a->sizeX));
		if(a == NULL) return NULL;
		a->contents = (char *)calloc(1, sizeof(char) * (a->sizeY * a->sizeX));
		a->color = (char *)calloc(1, sizeof(char) * (a->sizeY * a->sizeX));
		if(a->contents == NULL){
			free(a);
			a = NULL;
			return NULL;
		}
		char buffer[82];
		int y, x;
		for(y = 0; y < a->sizeY; y++){
			fgets(buffer, 81, load);
			for(x = 0; x < a->sizeX; x++){
				a->contents[y * a->sizeX + x] = buffer[x];
			}
		}
		for(y = 0; y < a->sizeY; y++){
			fgets(buffer, 81, load);
			for(x = 0; x < a->sizeX; x++){
				a->color[y * a->sizeX + x] = buffer[x];
			}
		}
		fclose(load);
	}
	return art;
}

ART *seekArt(ART *list, char *name){
	ART *a;
	for(a = list; a != NULL; a = a->next){
		if(!strcmp(a->name, name))break;
	}
	return a;
}

void replaceAll(char *text, size_t count){
	size_t i;
	for(i = 0; i < count; i++){
		text[i] = '\0';
	}
	return;
}

void replaceChar(char *text, size_t count, char toReplace, char withChar){
	size_t i;
	for(i = 0; i < count; i++){
		if(text[i] == toReplace) text[i] = withChar;
	}
	return;
}

/*	Move to engine?		*/
void drawArt(char *name, unsigned int y, unsigned int x){
	if(ARTLIST == NULL)return;
	ART *a = seekArt(ARTLIST, name);
	if(a == NULL) return;
	if(y < 0 || x < 0 || y > MAXHEIGHT || x > MAXWIDTH) return;
	if(y + a->sizeY >= MAXHEIGHT || x + a->sizeX >= MAXWIDTH) return;
	int sy, sx, ty, tx;
	sy = sx = 0;
	for(ty = y; ty < y + a->sizeY; ty++){
		for(tx = x; tx < x + a->sizeX; tx++){
			int test = a->color[sy * a->sizeX + sx] - '0';
			attron(COLOR_PAIR(test));
			mvaddch(ty, tx, a->contents[sy * a->sizeX + sx]);
			attroff(COLOR_PAIR(test));
			sx++;
			if(sx == a->sizeX){
				sx = 0;
				sy++;
			}
		}
	}
}

LL *delLoadList(LL *list){
	if(list == NULL) return NULL;
	while(list != NULL){
		LL *temp = list->next;
		free(list);
		list = NULL;
		list = temp;
	}
	return list;
}

/*	Create a list of files to be loaded from disk into memory 	*/
LL *loadList(unsigned int listType){
	char loadPath[25];
	char loadFile[50];
	char loadList[50];
	switch(listType){
		case TYPE_CREATURE:
			strcpy(loadPath, "data/creature/");
			strcpy(loadFile, "data/creature/");
			break;
		case TYPE_OBJECT:
			strcpy(loadPath, "data/object/");
			strcpy(loadFile, "data/object/");
			break;
		case TYPE_ITEM:
			strcpy(loadPath, "data/item/");
			strcpy(loadFile, "data/item/");
			break;
		case TYPE_ART:
			strcpy(loadPath, "data/art/");
			strcpy(loadFile, "data/art/");
			break;
		case TYPE_CLASS:
			strcpy(loadPath, "data/class/");
			strcpy(loadFile, "data/class/");
			break;
		case TYPE_RACE:
			strcpy(loadPath, "data/race/");
			strcpy(loadFile, "data/race/");
			break;
		default:
			return NULL;
	}
	FILE *load;
	strcpy(loadList, loadPath);
	strcat(loadList, "list.txt");
	load = fopen(loadList, "r");
	if(load == NULL) return NULL;
	LL *newList = NULL;
	LL *curList = NULL;
	unsigned int lineCount = 0;
	int c, i;
	while( (c=fgetc(load)) != EOF){
		if(c == '\n') lineCount++;
	}
	rewind(load);
	for(i = 0; i < lineCount; i++){
		if(newList == NULL){
			newList = (LL *)calloc(1, sizeof(LL));
			if(newList == NULL) return NULL;
			strcpy(newList->fileToLoad, loadPath);
			fscanf(load, "%s", loadFile);
			strcat(newList->fileToLoad, loadFile);
			strcat(newList->fileToLoad, ".dat");
		}
		else{
			curList = newList;
			while(curList->next != NULL) curList = curList->next;
			curList->next = (LL *)calloc(1, sizeof(LL));
			if(curList->next == NULL) return NULL;
			strcpy(curList->next->fileToLoad, loadPath);
			fscanf(load, "%s", loadFile);
			strcat(curList->next->fileToLoad, loadFile);
			strcat(curList->next->fileToLoad, ".dat");
		}
	}
	fclose(load);
	return newList;
}

bool yesNo(){
//	if(genesis == NULL) return false;
	bool answerOK = false;
	while(!answerOK){
		mvprintw(MAXHEIGHT / 2 - 5, MAXWIDTH / 2 - 12, "Okay to continue? [y/n]");
		refresh();
//		gensis->ch = getch();
		switch(getch()){
			case 'Y':
				return true;
				break;
			case 'y':
				return true;
				break;
			case 'N':
				return false;
				break;
			case 'n':
				return false;
				break;
			default:
				break;
		}
	}
}

void testGetRand(){
	FILE *log = fopen("rand.txt", "w");
	if(log == NULL) return;
	int i;
	for(i = 0; i < 100; i++){
		fprintf(log, "%i\n", getRand_i(1,4));
	}
	fclose(log);
}

void testRollDice(){
	FILE *log = fopen("testdice.txt", "w");
	if(log == NULL) return;
	int i;
	for(i = 0; i < 100; i++){
		fprintf(log, "1d20 = %i\n",rollDice(1,20,0));
	}
	fclose(log);
}

int getMinLevel(int level){
	if(level - 2 < 1) return 1;
	return level - 2;
}

int getMaxLevel(int level){
	if(level + 2 > 99) return 99;
	return level + 2;
}

/*	Get a random integer in specified range		*/
int getRand_i(int min, int max){
	if(min == max) return min;
	if(min > max){
		int temp = max;
		max = min;
		min = temp;
	}
	return rand() %(max - min) + min;	// This seems too easy.
}

/*	Seed the random number generator		*/
/*	Should return a failure value			*/
void seedRand(){
	FILE *f = fopen("/dev/urandom","r");
	char seed = getc(f);
	srand(seed);
	fclose(f);
}

#endif
