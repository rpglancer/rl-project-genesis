#ifndef H_BSP
#define H_BSP
#include "const.h"

typedef struct TREE{
	struct TREE *next;
	struct TREE *father;
	struct TREE *sons;
}TREE;

typedef struct{
	TREE tree;
	int y, x, h, w;
	int position;
	unsigned int level;
	bool horizontal;
}BSP;

typedef bool (*bspCallback)(BSP *node, void *data);

BSP *bspNew();
BSP *bspNewSized(int y, int x, int h, int w);
void bspDelete(BSP *node);

BSP *bspLeft(BSP *node);
BSP *bspRight(BSP *node);
BSP *bspFather(BSP *node);
BSP *bspFind(BSP *node, int y, int x);
static BSP *bspNewIntern(BSP *father, bool left);

bool bspIsLeaf(BSP *node);
bool bspTraversePre(BSP *node, bspCallback listener, void *data);
bool bspTraverseIn(BSP *node, bspCallback listener, void *data);
bool bspTraversePost(BSP *node, bspCallback listener, void *data);
bool bspTraverseLevel(BSP *node, bspCallback listener, void *data);
bool bspTraverseInverted(BSP *node, bspCallback listener, void *data);
bool bspContains(BSP *node, int y, int x);

void bspResize(BSP *node, int y, int x, int h, int w);
void bspSplitOnce(BSP *node, bool horizontal, int position);
void bspSplitRecursive(BSP *node, int depth, int minH, int minW);
void bspRemoveSons(BSP *node);

#endif
