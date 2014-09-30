#ifndef _C_OBJECT
#define _C_OBJECT
#include "../include/object.h"

OBJECT *newObject(){
	OBJECT *obj = (OBJECT *)calloc(1, sizeof(OBJECT));
	return obj;
}

OBJECTSTATS *addObject(OBJECTSTATS *list){
	if(list == NULL){
		list = (OBJECTSTATS *)calloc(1, sizeof(OBJECTSTATS));
		if(list == NULL) return NULL;
		return list;
	}
	else{
		OBJECTSTATS *temp = list;
		while(temp->next != NULL) temp = temp->next;
		temp->next = (OBJECTSTATS *)calloc(1, sizeof(OBJECTSTATS));
		if(temp->next == NULL) return NULL;	// Not sure if want
		return list;
	}
}

OBJECTSTATS *delObjectList(OBJECTSTATS *list){
	if(list == NULL) return NULL;
	while(list != NULL){
		OBJECTSTATS *temp = list->next;
		free(list);
		list = NULL;
		list = temp;
	}
	return list;
}

OBJECTSTATS *getObjects(OBJECTSTATS *list, unsigned int level, unsigned int type){
	if(OBJECTLIST == NULL) return NULL;
	OBJECTSTATS *o;
	unsigned int objCount;
	for(o = OBJECTLIST, objCount = 0; o != NULL; o = o->next){
		if(level >= o->dLevelMin && level <= o->dLevelMax && type == o->type){
			if(list == NULL){
				list = (OBJECTSTATS *)calloc(1, sizeof(OBJECTSTATS));
				if(list == NULL) return NULL;
				memcpy(list, o, sizeof(OBJECTSTATS));
				list->next = NULL;
			}
			else{
				OBJECTSTATS *temp = list;
				while(temp->next != NULL) temp = temp->next;
				temp->next = (OBJECTSTATS *)calloc(1, sizeof(OBJECTSTATS));
				if(temp->next == NULL) return list;
				memcpy(temp->next, o, sizeof(OBJECTSTATS));
				temp->next->next = NULL;
			}
		}
	}
	return list;
}

OBJECTSTATS *loadObject(LL *list){
	if(list == NULL) return NULL;
	LL *l;
	OBJECTSTATS *object = NULL;
	OBJECTSTATS *o = NULL;
	size_t i;
	for(l = list; l != NULL; l = l->next){
		FILE *load = fopen(l->fileToLoad, "r");
		if(load == NULL) return NULL;
		object = addObject(object);
		rewind(load);
		o = object;
		while(o->next != NULL) o = o->next;
		fscanf(load, "%c", &o->chClosed);
		fscanf(load, "%c", &o->chOpen);
		fscanf(load, "%15s", o->name);
		fscanf(load, "%25s", o->shortDesc);
		fscanf(load, "%50s", o->longDesc);
		for(i = 0; i < 50; i++){
			if(o->name[i] == '_' && i < 15) o->name[i] = ' ';
			if(o->shortDesc[i] == ' ' && i < 25) o->shortDesc[i] = ' ';
			if(o->longDesc[i] == ' ') o->longDesc[i] = ' ';
		}
		fscanf(load, "%i", &o->color);
		fscanf(load, "%i", &o->type);
		fscanf(load, "%i %i", &o->dLevelMin, &o->dLevelMax);
		fscanf(load, "%i", &o->isLocked);
		fscanf(load, "%i", &o->isOpen);
		fscanf(load, "%i", &o->isPassable);
		fscanf(load, "%i", &o->keyNum);		// Consider removing from object files
		fscanf(load, "%i", &o->flags);
		fclose(load);
	}
	return object;
}

OBJECTSTATS *seekObject(OBJECTSTATS *list, char *name){
	OBJECTSTATS *o;
	for(o = list; o != NULL; o = o->next){
		if(!strcmp(o->name, name)) break;
	}
	return o;
}

void delObject(OBJECT *obj){
	if(obj == NULL) return;
	free(obj);
	obj = NULL;
}

void setObjectStats(OBJECT *obj, OBJECTSTATS *stats){
	if(!obj || !stats)return;
	obj->type = stats->type;
	obj->isLocked = stats->isLocked;
	obj->isOpen = stats->isOpen;
	obj->isPassable = stats->isPassable;
	obj->keyNum = stats->keyNum;
}

#endif
