#ifndef C_WINMGR
#define C_WINMGR
#include "../include/winmgr.h"

WINDOW *newWindow(int y, int x, int h, int w){
	WINDOW *local_win;
	local_win = newwin(h, w, y, x);
	wrefresh(local_win);
	return local_win;
}

void delWindow(WINDOW *local_win){
	delwin(local_win);
}

#endif
