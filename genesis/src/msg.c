#ifndef C_MSG
#define C_MSG
#include "../include/msg.h"

MESSAGE *freeMsgList(MESSAGE *list){
	if(list == NULL) return NULL;
	while(list != NULL){
		MESSAGE *temp = list->next;
		free(list);
		list = NULL;
		list = temp;
	}
	return list;
}

int initQueue(){
//	MSGQUEUE = (MESSAGE *)malloc(sizeof(MESSAGE));
	MSGQUEUE = (MESSAGE *)calloc(1, sizeof(MESSAGE));
	if(MSGQUEUE == NULL)return ERR_MALLOC;
	MSGCURRENT = MSGQUEUE;
	MSGQUEUE->msgUID = 0;
	MSGQUEUE->msgType = -1;
	MSGQUEUE->msgFlag = -1;
	MSGQUEUE->source = NULL;
	MSGQUEUE->target = NULL;
	MSGCURRENT = MSGQUEUE;
	return ERR_NONE;
}

int initHistory(){
	if(ACTHISTORY != NULL){
		while(ACTHISTORY != NULL){
			_TEXT *temp = ACTHISTORY->next;
			free(ACTHISTORY);
			ACTHISTORY = ACTHISTORY->next;
		}
	}
	ACTHISTORY = (_TEXT *)calloc(1, sizeof(_TEXT));
	if(ACTHISTORY == NULL) return ERR_MALLOC;
	textHistorySize = 1;
	return ERR_NONE;
}

void printHistory(){
	if(ACTHISTORY == NULL) return;
	int size = textHistorySize;
	int i, y, x;
	_TEXT *m = ACTHISTORY;
	for(i = 0, y = 2, x = 40; i < 3, m != NULL; i++, y--, m = m->next){
		mvaddstr(y, x, m->message);
		// use mvprintw() dummy
	}
	return;
}

int popMsg(MESSAGE *msg){
	if(msg == MSGQUEUE){
		if(msg->next == NULL){
			msg = NULL;
			free(msg);
			MSGQUEUE = NULL;
			return 0;
		}
		else{
			MSGQUEUE = msg->next;
			free(msg);
			msg = NULL;
			return 0;
		}
	}
	else{
		MESSAGE *temp = MSGQUEUE;
		while(temp->next != msg) temp = temp->next;
		temp->next = msg->next;
		free(msg);
		msg = NULL;
		return 0;
	}
	return 0;
}

int pushMsg(ENTITY *src, ENTITY *tgt, int type, int flag){
//	MESSAGE *newMsg = (MESSAGE *)malloc(sizeof(MESSAGE));
	MESSAGE *newMsg = (MESSAGE *)calloc(1,sizeof(MESSAGE));
	if(newMsg == NULL)return ERR_MALLOC;
	newMsg->source = src;
	newMsg->target = tgt;
	newMsg->msgType = type;
	newMsg->msgFlag = flag;
	newMsg->next = MSGQUEUE;
	MSGQUEUE = newMsg;
	return ERR_NONE;
}

int popHistory(_TEXT *toPop){
	if(toPop == NULL) return ERR_OOB;
	if(toPop == ACTHISTORY){
		_TEXT *temp = ACTHISTORY->next;
		free(ACTHISTORY);
		ACTHISTORY = temp;
	}
	else if(toPop->next == NULL){
		_TEXT *temp = ACTHISTORY;
		while(temp->next != toPop) temp = temp->next;
		free(toPop);
		toPop = NULL;
		temp->next = NULL;
		toPop = temp;
	}
	else{
		_TEXT *temp = ACTHISTORY;
		while(temp->next != toPop) temp = temp->next;
		temp->next = toPop->next;
		free(toPop);
		toPop = NULL;
		toPop = temp;
	}
	return ERR_NONE;
}

int pushHistory(ENTITY *source, ENTITY *target, int msgType, int msgFlag){
	_TEXT *m = ACTHISTORY;
	void *s = NULL;
	void *t = NULL;
	char sName[20], tName[20];
	if(source != NULL){
		switch(source->category){
			case C_CREATURE:
				s = ((CREP)source->ent);
				strncpy(sName, ((CREP)source->ent)->name, sizeof(((CREP)source->ent)->name));
				break;
			case C_ITEM:
				s = ((ITEMP)source->ent);
				strncpy(sName, ((ITEMP)source->ent)->name, sizeof(((ITEMP)source->ent)->name));
				break;
			case C_OBJECT:
				s = ((OBJP)source->ent);
				strncpy(sName, ((OBJP)source->ent)->name, sizeof(((OBJP)source->ent)->name));
			default:
				t = target->ent;
		}
	}
	if(target != NULL){
		switch(target->category){
			case C_CREATURE:
				t = ((CREP)target->ent);
				strncpy(tName, ((CREP)target->ent)->name, sizeof(((CREP)target->ent)->name));
				break;
			case C_ITEM:
				t = ((ITEMP)target->ent);
				strncpy(tName, ((ITEMP)target->ent)->name, sizeof(((ITEMP)target->ent)->name));
				break;
			case C_OBJECT:
				t = ((OBJP)target->ent);
				strncpy(tName, ((OBJP)target->ent)->name, sizeof(((OBJP)target->ent)->name));
				break;
			default:
				t = target->ent;
		}
	}
	int count = 0;
	for(m; m->next != NULL; m = m->next){
		count++;
	}
	if(count == 99){
		popHistory(m);
	}
	_TEXT *temp = (_TEXT *)calloc(1, sizeof(_TEXT));
	if(temp == NULL) return ERR_MALLOC;
	temp->next = ACTHISTORY;
	ACTHISTORY = temp;
	if(source != NULL){
		if(source == player) strcpy(ACTHISTORY->source, "You");
		else strncpy(ACTHISTORY->source, sName, 20);
	}
	if(target != NULL){
		if(target == player) strcpy(ACTHISTORY->target, "you");
		else strncpy(ACTHISTORY->target, tName, 20);
	}
	switch(msgType){
		case MSG_ATTACK:
			strcpy(ACTHISTORY->message, ACTHISTORY->source);
			strcat(ACTHISTORY->message, " attacks ");
			strcat(ACTHISTORY->message, ACTHISTORY->target);
			break;
		case MSG_MOVE:
			switch(msgFlag){
				case DIR_NORTH:
					strcpy(ACTHISTORY->message, ACTHISTORY->source);
					strcat(ACTHISTORY->message, " move north.");
					break;
				case DIR_EAST:
					strcpy(ACTHISTORY->message, ACTHISTORY->source);
					strcat(ACTHISTORY->message, " move east.");
					break;
				case DIR_SOUTH:
					strcpy(ACTHISTORY->message, ACTHISTORY->source);
					strcat(ACTHISTORY->message, " move south.");
					break;
				case DIR_WEST:
					strcpy(ACTHISTORY->message, ACTHISTORY->source);
					strcat(ACTHISTORY->message, " move west.");
					break;
				default:
					strcpy(ACTHISTORY->message, ACTHISTORY->source);
					strcat(ACTHISTORY->message, " move somewhere.");
					break;
			}
			break;
		case MSG_OPEN:
			strcpy(ACTHISTORY->message, ACTHISTORY->source);
			strcat(ACTHISTORY->message, " open ");
			strcat(ACTHISTORY->message, ACTHISTORY->target);
			break;
		case MSG_CLOSE:
			strcpy(ACTHISTORY->message, ACTHISTORY->source);
			strcat(ACTHISTORY->message, " close ");
			strcat(ACTHISTORY->message, ACTHISTORY->target);
			break;
		default:
			break;
	}
	return ERR_NONE;
}
#endif
