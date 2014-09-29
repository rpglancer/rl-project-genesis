#ifndef _C_ITEM
#define _C_ITEM
#include "../include/item.h"

EQUIPMENT *addSlot(EQUIPMENT *list, int slotType){
	EQUIPMENT *e = list;
	if(e == NULL){
		e = (EQUIPMENT *)calloc(1, sizeof(EQUIPMENT));
		if(e == NULL) return NULL;
		list = e;

	}
	else{
		while(e->next != NULL) e = e->next;
		e->next = (EQUIPMENT *)calloc(1, sizeof(EQUIPMENT));
		if(e->next == NULL) return list;
		e = e->next;
	}
	e->slot = slotType;
	return list;
}

_ITEM *newItem(){
	_ITEM *i = (_ITEM *)calloc(1, sizeof(_ITEM));
	return i;
}

#endif
