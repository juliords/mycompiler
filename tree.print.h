
#ifndef TREE_PRINT_H
#define TREE_PRINT_H

#include "tree.h"

#define MINIFIED 0
#if MINIFIED
	#define print_space(c)
#else
	#define print_space(c) printf(c)
#endif

void printProgramNode(ProgramNode* p);
void printDeclarationNode(DeclarationNode* p, int tabs);
void printDecVarNode(ListNode* p, int tabs);
void printTypeNode(TypeNode* p);
void printDecFuncNode(DecFuncNode* p, int tabs);
void printParamNode(DecVarNode* p);
void printBlockNode(BlockNode* p, int tabs);
void printCmdNode(CmdNode* p, int tabs);
void printVarNode(VarNode* p);
void printExpNode(ExpNode* p);
void printCallNode(CallNode* p);

#endif

