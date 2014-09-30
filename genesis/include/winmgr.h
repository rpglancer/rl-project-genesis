#ifndef H_WINMGR
#define H_WINMGR
#include "const.h"

WINDOW *newWindow(int y, int x, int h, int w);
void delWindow(WINDOW *local_win);

#endif
