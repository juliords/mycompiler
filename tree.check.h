
#ifndef TREE_CHECK_H
#define TREE_CHECK_H

#include "tree.h"

typedef struct 
{
	ListNode *var_list;
	ListNode *func_list;

	DecFuncNode *curr_func;
} EnvData;

void checkProgramNode(ProgramNode* p);
void checkDeclarationNode(EnvData* env, DeclarationNode* p);
void checkDecVarNode(EnvData* env, ListNode* p, DecVarType type);
void checkDecFuncNode(EnvData* env, DecFuncNode* p);
void checkBlockNode(EnvData* env, BlockNode* p);
void checkCmdNode(EnvData* env, CmdNode* p);
void checkVarNode(EnvData* env, VarNode* p);
void checkExpNode(EnvData* env, ExpNode* p);
void checkCallNode(EnvData* env, CallNode *p);


#endif

