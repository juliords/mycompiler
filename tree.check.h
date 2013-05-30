
#ifndef TREE_CHECK_H
#define TREE_CHECK_H

#include "tree.h"

int addGlobalVar(DecVarNode*);
DecVarNode* getGlobalVar(char *id);

int addContextVar(DecVarNode* v);
DecVarNode* getContextVar(char *id);

DecVarNode* getVar(char *id);
int addVarList(ListNode *vars, DecVarType type);

void pushContextVarList();
void popContextVarList();

#endif

