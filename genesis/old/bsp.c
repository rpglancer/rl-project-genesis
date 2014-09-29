#ifndef C_BSP
#define C_BSP
#include "../include/bsp.h"
#include "../include/util.h"

TREE *treeNew(){
	return (TREE *)calloc(1,sizeof(TREE));
}

void treeAddSon(TREE *node, TREE *son){
	TREE *lastson = node->sons;
	son->father = node;
	while(lastson && lastson->next) lastson = lastson->next;
	if(lastson){
		lastson->next = son;
	}
	else{
		node->sons = son;
	}
}

BSP *bspNew(){
	return (BSP *)calloc(sizeof(BSP),1);
}

BSP *bspNewSized(int y, int x, int h, int w){
	BSP *bsp = (BSP *)calloc(sizeof(BSP),1);
	bsp->y = y;
	bsp->x = x;
	bsp->h = h;
	bsp->w = w;
	return bsp;
}

BSP *bspFind(BSP *node, int y, int x){
	if(!bspContains(node,y,x))return NULL;
	if(!bspIsLeaf(node)){
		BSP *left, *right;
		left = bspLeft(node);
		if(bspContains(left,y,x)) return bspFind(left,y,x);
		right = bspRight(node);
		if(bspContains(right,y,x)) return bspFind(right,y,x);
	}
	return node;
}

BSP *bspLeft(BSP *node){
	if(node == NULL) return NULL;
	return (BSP *)node->tree.sons;
}

BSP *bspRight(BSP *node){
	if(node == NULL) return NULL;
	return node->tree.sons ? (BSP *)node->tree.sons->next : NULL;
}

BSP *bspFather(BSP *node){
	if(node == NULL) return NULL;
	return (BSP *)node->tree.father;
}

static BSP *bspNewIntern(BSP *father, bool left){
	BSP *bsp = (BSP *)calloc(sizeof(BSP),1);
	if(father->horizontal){
		bsp->x = father->x;
		bsp->w = father->w;
		bsp->y = left ? father->y : father->position;
		bsp->h = left ? father->position - bsp->y : father->y + father->h - father->position;
	}
	else{
		bsp->y = father->y;
		bsp->h = father->h;
		bsp->x = left ? father->x : father->position;
		bsp->w = left ? father->position - bsp->x : father->x + father->w - father->position;
	}
	bsp->level = father->level + 1;
	return bsp;
}

bool bspIsLeaf(BSP *node){
	if(node == NULL) return false;
	return node->tree.sons == NULL;
}

bool bspContains(BSP *node, int y, int x){
	return (x >= node->x && y >= node->y && x < node->x + node->w && y < node->y + node->h);
}

bool bspTraversePre(BSP *node, bspCallback listener, void *data){
	if(node == NULL && listener == NULL) return false;
	if(!listener(node, data)) return false;
	if(bspLeft(node) && !bspTraversePre(bspLeft(node), listener, data)) return false;
	if(bspRight(node) && !bspTraversePre(bspRight(node), listener, data)) return false;
	return true;
}

bool bspTraverseIn(BSP *node, bspCallback listener, void *data){
	if(node == NULL && listener == NULL) return false;
	if(bspLeft(node) && !bspTraverseIn(bspLeft(node), listener, data)) return false;
	if(!listener(node, data)) return false;
	if(bspRight(node) && !bspTraverseIn(bspRight(node), listener, data)) return false;
	return true;
}

/*		An example of a callback to find shit in the BSP		*/
bool testCallback(BSP *node, void *data){
	int query = (int)data;	// Cast it to what you're looking for!
	if(node->level == query){
		printf("Level %i Starty: %i StartX: %i - EndY: %i EndX: %i\n", node->level, node->y, node->x, node->y + node->h, node->x + node->w);
		return true;
	}
}

void bspDelete(BSP *node){
	if(node == NULL)return;
	bspRemoveSons(node);
	free(node);
}

void bspRemoveSons(BSP *node){
	BSP *temp;
	if(node == NULL) return;
	temp = (BSP *)node->tree.sons;
	while(temp){
		BSP *nextNode = (BSP *)temp->tree.next;
		bspRemoveSons(temp);
		free(temp);
		temp = nextNode;
	}
	node->tree.sons = NULL;
}

void bspSplitOnce(BSP *node, bool horizontal, int position){
	if(node == NULL) return;
	node->horizontal = horizontal;
	node->position = position;
	treeAddSon(&node->tree, &bspNewIntern(node, true)->tree);
	treeAddSon(&node->tree, &bspNewIntern(node, false)->tree);
}

void bspSplitRecursive(BSP *node, int depth, int minH, int minW){
	float maxHRatio, maxWRatio;
	maxHRatio = maxWRatio = 1.50;
	int horiz, position;
	if(depth == 0 || node->h < 2*minH && node->w < 2*minW)return;
	if(node->h > node->w) horiz = true;
	else if(node-> w > node->h) horiz = false;
	else horiz = rand() %2;
//	if(node->h < 2*minH || node->w > node->h * maxHRatio) horiz = false;
//	else if(node->w < 2*minW || node->h > node->w * maxWRatio) horiz = true;
	if(horiz){
		position = getRand_i(node->y + minH, node->h - minH);
		//position = (node->y + node->h)/2;
		//position = getRand_i( (node->y + node->h) * 0.4, (node->y + node->h) * 0.6);
	}
	else{
		position = getRand_i(node->x + minW, node->w - minW);
//		position = (node->x + node->w)/2;
		//position = getRand_i( (node->x + node->w) * 0.4, (node->x + node->w) * 0.6);
	}
	bspSplitOnce(node, horiz, position);
	bspSplitRecursive(bspLeft(node), depth - 1, minH, minW);
	bspSplitRecursive(bspRight(node), depth - 1, minH, minW);
}

#endif
