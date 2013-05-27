
#ifndef TREE_PRINT_H
#define TREE_PRINT_H

#define MINIFIED 0
#if MINIFIED
	#define print_space(c)
#else
	#define print_space(c) printf(c)
#endif

void printProgramNode(ProgramNode* p);
void printDeclarationNode(DeclarationNode* p);
void printDecVarNode(DecVarNode* p);
void printTypeNode(TypeNode* p);
void printDecFuncNode(DecFuncNode* p);
void printParamNode(ParamNode* p);
void printBlockNode(BlockNode* p);
void printTypeNode(TypeNode* p);
void printCmdNode(CmdNode* p);
void printVarNode(VarNode* p);
void printExpNode(ExpNode* p);
void printCallNode(CallNode* p);

#endif

