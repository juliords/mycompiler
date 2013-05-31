
#ifndef TREE_CHECK_H
#define TREE_CHECK_H

#include "tree.h"

void checkProgramNode(ProgramNode* p);
void checkDeclarationNode(DeclarationNode* p);
void checkDecVarNode(ListNode* p, DecVarType type);
void checkDecFuncNode(DecFuncNode* p);
void checkBlockNode(BlockNode* p);
void checkCmdNode(CmdNode* p);
void checkVarNode(VarNode* p);
void checkExpNode(ExpNode* p);
void checkCallNode(CallNode *p);


#endif

