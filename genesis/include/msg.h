#ifndef H_MSG
#define H_MSG
#include "const.h"
#include "entity.h"

/*		Message structure		*/
typedef struct MESSAGE{
	unsigned int msgUID;		// unique ID of message, probably irrelevant
	int msgType;			// Type of message
	int msgFlag;			// Flags pertinent to type of message
	ENTITY *source;			// Source of the message
	ENTITY *target;			// Target of the message
	struct MESSAGE *next;		// The next message
}MESSAGE;

/*		Global message pointers		*/
MESSAGE *MSGQUEUE;			// Root of the message queue
MESSAGE *MSGCURRENT;			// Currently selected message

/*		Text structure			*/
typedef struct _TEXT{
	char message[40];		// Text to display
	char source[20];		// Name of source of text
	char target[20];		// Name of target of text
	struct _TEXT *next;		// The next text
}_TEXT;

/*		Global text pointers		*/
_TEXT *ACTHISTORY;

int initHistory();
int initQueue();
int popHistory(_TEXT *);
int popMsg(MESSAGE *);
int pushHistory(ENTITY *, ENTITY *, int, int);
int pushMsg(ENTITY *, ENTITY *, int, int);

MESSAGE *freeMsgList(MESSAGE *list);
_TEXT *freeTextList(_TEXT *list);

void printHistory();

#endif
