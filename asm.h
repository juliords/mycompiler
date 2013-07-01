
#ifndef TREE_asm_H
#define TREE_asm_H

#include "tree.h"

void asmProgramNode(ProgramNode* p);
void asmDeclarationNode(DeclarationNode* p);
void asmDecVarNode(ListNode* p);
void asmTypeNode(TypeNode* p);
void asmDecFuncNode(DecFuncNode* p);
void asmParamNode(DecVarNode* p);
void asmBlockNode(BlockNode* p);
void asmCmdNode(CmdNode* p);
void asmVarNode(VarNode* p);
void asmExpNode(ExpNode* p);
void asmCallNode(CallNode* p);

#endif

