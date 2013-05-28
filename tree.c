
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "tree.h"
#include "macro.h"

ListNode* newListNode(void *data, ListNode *list)
{
	NEW(ListNode, p);

	p->data = data;
	p->next = NULL;
	p->last = p;

	if(list)
	{
		list->last->next = p;
		list->last = p;
		return list;
	}
	else
	{
		return p;
	}
}

ProgramNode* newProgramNode(ListNode *dec)
{
	NEW(ProgramNode, p);

	p->dec = dec;

	return p;
}

DeclarationNode* newDeclarationNodeTypeDecVar(DecVarNode* var)
{
	NEW(DeclarationNode, p);

	p->type = DecVar;
	p->u.var = var;

	return p;
}

DeclarationNode* newDeclarationNodeTypeDecFunc(DecFuncNode* func)
{
	NEW(DeclarationNode, p);

	p->type = DecFunc;
	p->u.func = func;

	return p;
}

DecVarNode* newDecVarNode(TypeNode *type, ListNode *name)
{
	NEW(DecVarNode, p);
	
	p->type = type;
	p->name = name;
	
	return p;
}

TypeNode* newTypeNode(TypeType type, ... )
{
	va_list args;
	NEW(TypeNode, p);
	
	p->type = type;

	va_start(args, type);
	switch(type)
	{
		case TypePrim:
			p->u.prim = va_arg(args, BaseType);
			break;
		case TypeArray:
			p->u.array = va_arg(args, TypeNode*);
			break;
	}
	
	va_end(args);

	return p;
}

TypeNode* newTypeNodeTypePrim(BaseType prim)
{
	NEW(TypeNode, p);
	
	p->type = TypePrim;
	p->u.prim = prim;

	return p;
}

TypeNode* newTypeNodeTypeArray(TypeNode *array)
{
	NEW(TypeNode, p);
	
	p->type = TypeArray;
	p->u.array = array;

	return p;
}

DecFuncNode* newDecFuncNode(TypeNode *type, char *id, ListNode *params, BlockNode *block)
{
	NEW(DecFuncNode, p);
	
	p->type = type;
	p->id = id;
	p->params = params;
	p->block = block;
	
	return p;
}

ParamNode* newParamNode(TypeNode *type, char *id )
{
	NEW(ParamNode, p);
	
	p->type = type;
	p->id = id;
	
	return p;
}

BlockNode* newBlockNode(ListNode *var, ListNode *cmd )
{
	NEW(BlockNode, p);
	
	p->var = var;
	p->cmd = cmd;
	
	return p;
}

CmdNode* newCmdNodeCmdIf(ExpNode *cond, CmdNode *cmd_if, CmdNode *cmd_else)
{
	NEW(CmdNode, p);
	
	p->type = CmdIf;
	p->u.i.cond = cond;
	p->u.i.cmd_if = cmd_if;
	p->u.i.cmd_else = cmd_else;
	
	return p;
}

CmdNode* newCmdNodeCmdWhile(ExpNode *cond, CmdNode *cmd)
{
	NEW(CmdNode, p);
	
	p->type = CmdWhile;
	p->u.w.cond = cond;
	p->u.w.cmd = cmd;

	return p;
}

CmdNode* newCmdNodeCmdAssig(VarNode *var, ExpNode *exp)
{
	NEW(CmdNode, p);
	
	p->type = CmdAssig;
	p->u.a.var = var;
	p->u.a.exp = exp;

	return p;
}

CmdNode* newCmdNodeCmdRet(ExpNode *exp)
{
	NEW(CmdNode, p);
	
	p->type = CmdRet;
	p->u.r.exp = exp;

	return p;
}

CmdNode* newCmdNodeCmdCall(CallNode *call)
{
	NEW(CmdNode, p);
	
	p->type = CmdCall;
	p->u.c.call = call;

	return p;
}

CmdNode* newCmdNodeCmdBlock(BlockNode *block)
{
	NEW(CmdNode, p);
	
	p->type = CmdBlock;
	p->u.b.block = block;

	return p;
}

VarNode* newVarNodeVarId(char *id)
{
	NEW(VarNode, p);

	p->type = VarId;
	p->u.id = id;

	return p;
}

VarNode* newVarNodeVarArray(VarNode *array, ExpNode* exp)
{
	NEW(VarNode, p);

	p->type = VarArray;
	p->u.v.array = array;
	p->u.v.exp = exp;

	return p;
}

ExpNode* newExpNodeExpValueInt(int i)
{
	NEW(ExpNode, p);
	
	p->type = ExpValue;
	p->u.prim.type = PrimInt;
	p->u.prim.u.i = i;
	
	return p;
}

ExpNode* newExpNodeExpValueFloat(float f)
{
	NEW(ExpNode, p);
	
	p->type = ExpValue;
	p->u.prim.type = PrimFloat;
	p->u.prim.u.f = f;
	
	return p;
}

ExpNode* newExpNodeExpValueChar(char c)
{
	NEW(ExpNode, p);
	
	p->type = ExpValue;
	p->u.prim.type = PrimChar;
	p->u.prim.u.c = c;
	
	return p;
}

ExpNode* newExpNodeExpValueStr(char *s)
{
	NEW(ExpNode, p);
	
	p->type = ExpValue;
	p->u.prim.type = PrimChar;
	p->u.prim.u.s = s;
	
	return p;
}

ExpNode* newExpNodeExpVar(VarNode *var)
{
	NEW(ExpNode, p);
	
	p->type = ExpVar;
	p->u.var.var = var;
	
	return p;
}

ExpNode* newExpNodeExpBin(ExpBinType type, ExpNode *left, ExpNode *right)
{
	NEW(ExpNode, p);
	
	p->type = ExpBin;
	p->u.bin.type = type;
	p->u.bin.left = left;
	p->u.bin.right = right;
	
	return p;
}

ExpNode* newExpNodeExpUn(ExpUnType type, ExpNode *exp)
{
	NEW(ExpNode, p);
	
	p->type = ExpUn;
	p->u.un.type = type;
	p->u.un.exp = exp;
	
	return p;
}

ExpNode* newExpNodeExpCall(CallNode *call)
{
	NEW(ExpNode, p);
	
	p->type = ExpCall;
	p->u.call.call = call;
	
	return p;
}

ExpNode* newExpNodeExpNew(TypeNode *type, ExpNode *exp)
{
	NEW(ExpNode, p);
	
	p->type = ExpNew;
	p->u.enew.type = type;
	p->u.enew.exp = exp;
	
	return p;
}

CallNode* newCallNode(char *id, ListNode *exp )
{
	NEW(CallNode, p);
	
	p->id = id;
	p->exp = exp;
	
	return p;
}

/* ------------------------------------------------------ */

void freeProgramNode(ProgramNode* p)
{
	ListNode* l, *next;

	if(!p) return;

	for(l = p->dec; l; l = next)
	{
		freeDeclarationNode((DeclarationNode*)l->data);

		next = l->next;
		free(l);
	}
	free(p);
}

void freeDeclarationNode(DeclarationNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case DecVar:
			freeDecVarNode(p->u.var);
			break;
		case DecFunc:
			freeDecFuncNode(p->u.func);
			break;
	}

	free(p);
}

void freeDecVarNode(DecVarNode* p)
{
	ListNode* l, *next;

	if(!p) return;

	freeTypeNode(p->type);

	for(l = p->name; l; l = next)
	{
		free(l->data);

		next = l->next;
		free(l);
	}
	free(p);
}

void freeTypeNode(TypeNode* p)
{
	if(!p) return;

	if(p->type == TypeArray)
	{
		freeTypeNode(p->u.array);
	}
	free(p);
}

void freeDecFuncNode(DecFuncNode* p)
{
	ListNode *l, *next;

	if(!p) return;

	freeTypeNode(p->type);
	free(p->id);

	for(l = p->params; l; l = next)
	{
		freeParamNode((ParamNode*)l->data);

		next = l->next;
		free(l);
	}

	freeBlockNode(p->block);
	free(p);
}

void freeParamNode(ParamNode* p)
{
	if(!p) return;

	free(p->id);
	freeTypeNode(p->type);
	free(p);
}

void freeBlockNode(BlockNode* p)
{
	ListNode* l, *next;

	if(!p) return;

	for(l = p->var; l; l = next)
	{
		freeDecVarNode((DecVarNode*)l->data);

		next = l->next;
		free(l);
	}

	for(l = p->cmd; l; l = next)
	{
		freeCmdNode((CmdNode*)l->data);

		next = l->next;
		free(l);
	}

	free(p);
}

void freeCmdNode(CmdNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case CmdIf:
			freeExpNode(p->u.i.cond);
			freeCmdNode(p->u.i.cmd_if);
			freeCmdNode(p->u.i.cmd_else);
			break;

		case CmdWhile:
			freeExpNode(p->u.w.cond);
			freeCmdNode(p->u.w.cmd);
			break;

		case CmdAssig:
			freeVarNode(p->u.a.var);
			freeExpNode(p->u.a.exp);
			break;

		case CmdRet:
			freeExpNode(p->u.r.exp);
			break;

		case CmdCall: 
			freeCallNode(p->u.c.call);
			break;

		case CmdBlock:
			freeBlockNode(p->u.b.block);
			break;
	}
	free(p);
}

void freeVarNode(VarNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case VarId:
			free(p->u.id);
			break;
		case VarArray:
			freeVarNode(p->u.v.array);
			freeExpNode(p->u.v.exp);
			break;
	}
	free(p);
}

void freeExpNode(ExpNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case ExpVar:
			freeVarNode(p->u.var.var);
			break;

		case ExpValue:
			if(p->u.prim.type == PrimStr)
			{
				free(p->u.prim.u.s);
			}
			break;

		case ExpBin:
			freeExpNode(p->u.bin.left);
			freeExpNode(p->u.bin.right);
			break;

		case ExpUn:
			freeExpNode(p->u.un.exp);
			break;

		case ExpCall:
			freeCallNode(p->u.call.call); 
			break;

		case ExpNew:
			freeTypeNode(p->u.enew.type);
			freeExpNode(p->u.enew.exp);
			break;
	}
	free(p);
}

void freeCallNode(CallNode *p)
{
	ListNode *l, *next;

	if(!p) return;

	free(p->id);

	for(l = p->exp; l; l = next)
	{
		freeExpNode((ExpNode*)l->data);

		next = l->next;
		free(l);
	}
	free(p);
}

