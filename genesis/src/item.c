#ifndef _C_ITEM
#define _C_ITEM
#include "../include/item.h"

_ITEM *newItem(){
	_ITEM *i = (_ITEM *)calloc(1, sizeof(_ITEM));
	return i;
}

_ITEMSTATS *addItem(_ITEMSTATS *list){
	if(list == NULL){
		list = (_ITEMSTATS *)calloc(1, sizeof(_ITEMSTATS));
		if(list == NULL) return NULL;
		return list;
	}
	_ITEMSTATS *temp = list;
	while(temp->next != NULL) temp = temp->next;
	temp->next = (_ITEMSTATS *)calloc(1, sizeof(_ITEMSTATS));
	if(temp->next == NULL) return NULL;
	return list;
}

_ITEMSTATS *delItemList(_ITEMSTATS *list){
	while(list != NULL){
		_ITEMSTATS *temp = list->next;
		free(list);
		list = NULL;
		list = temp;
	}
	return list;
}

_ITEMSTATS *loadItem(LL *list){
	if(list == NULL) return NULL;
	LL *l;
	_ITEMSTATS *item = NULL;
	_ITEMSTATS *i = NULL;
	for(l = list; l != NULL; l = l->next){
		FILE *load = fopen(l->fileToLoad, "r");
		if(load == NULL) return NULL;
		item = addItem(item);
		rewind(load);
		i = item;
		while(i->next != NULL) i = i->next;
		fscanf(load, "%c", &i->ch);
		fscanf(load, "%19s", i->itemName);
		fscanf(load, "%i", &i->itemSize);
		fscanf(load, "%i", &i->itemType);
		fscanf(load, "%i", &i->itemWearF);
		fscanf(load, "%i", &i->itemUseF);
		fscanf(load, "%i", &i->itemWeight);
		fscanf(load, "%i", &i->itemAC);
		fscanf(load, "%i", &i->itemDmgType);
		fscanf(load, "%i", &i->itemDmgRoll);
		fscanf(load, "%i", &i->itemDmgSize);
		fscanf(load, "%i", &i->itemDmgMod);
		fclose(load);
	}
	return item;
}

_ITEMSTATS *seekItem(_ITEM *item){
	_ITEMSTATS *list = ITEMLIST;
	for(list; list != NULL; list = list->next){
		if(!strcmp(list->itemName, item->itemName)) return list;
	}
	return NULL;
}

void delItem(_ITEM *item){
	if(item == NULL) return;
	free(item);
	item = NULL;
}

#endif
