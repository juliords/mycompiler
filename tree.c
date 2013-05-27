
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

DeclarationNode* newDeclarationNode(DeclarationType type, ... )
{
	va_list args;
	NEW(DeclarationNode, p);

	p->type = type;

	va_start(args, type);
	switch(type)
	{
		case DecVar:
			p->u.var = va_arg(args, DecVarNode*);
			break;
		case DecFunc:
			p->u.func = va_arg(args, DecFuncNode*);
			break;
	}
	va_end(args);

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
		case TypeBase:
			p->u.base = va_arg(args, BaseType);
			break;
		case TypeRec:
			p->u.next = va_arg(args, TypeNode*);
			break;
	}
	
	va_end(args);

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

CmdNode* newCmdNode(CmdType type, ... )
{
	va_list args;
	NEW(CmdNode, p);
	
	p->type = type;

	va_start(args, type);
	switch(type)
	{
		case CmdIf:
			p->u.i.cond = va_arg(args, ExpNode*);
			p->u.i.cmd_if = va_arg(args, CmdNode*);
			p->u.i.cmd_else = va_arg(args, CmdNode*);
			break;

		case CmdWhile:
			p->u.w.cond = va_arg(args, ExpNode*);
			p->u.w.cmd = va_arg(args, CmdNode*);
			break;

		case CmdAssig:
			p->u.a.var = va_arg(args, VarNode*);
			p->u.a.exp = va_arg(args, ExpNode*);
			break;

		case CmdRet:
			p->u.r.exp = va_arg(args, ExpNode*);
			break;

		case CmdCall:
			p->u.c.call = va_arg(args, CallNode*);
			break;

		case CmdBlock:
			p->u.b.block = va_arg(args, BlockNode*);
			break;
	}
	va_end(args);

	return p;
}

VarNode* newVarNode(VarType type, ... )
{
	va_list args;
	NEW(VarNode, p);

	p->type = type;

	va_start(args, type);
	switch(type)
	{
		case VarId:
			p->u.id = va_arg(args, char*);
			break;
		case VarRec:
			p->u.rec.next = va_arg(args, VarNode*);
			p->u.rec.exp = va_arg(args, ExpNode*);
			break;
	}

	va_end(args);

	return p;
}

ExpNode* newExpNode(ExpType type, ... )
{
	va_list args;
	NEW(ExpNode, p);
	
	p->type = type;
	va_start(args, type);

	switch(type)
	{
		case ExpVar:
			p->u.var.var = va_arg(args, VarNode*);
			break;
		case ExpValue:
			p->u.prim.type = va_arg(args, PrimitiveType);
			switch(p->u.prim.type)
			{
				case PrimInt:
					p->u.prim.u.i = va_arg(args, int);
					break;
				case PrimFloat:
					p->u.prim.u.f = (float)va_arg(args, double);
					break;
				case PrimChar:
					p->u.prim.u.c = (char)va_arg(args, int);
					break;
				case PrimStr:
					p->u.prim.u.s = va_arg(args, char*);
					break;
			}
			break;
		case ExpBin:
			p->u.bin.type = va_arg(args, ExpBinType);
			p->u.bin.left = va_arg(args, ExpNode*);
			p->u.bin.right = va_arg(args, ExpNode*);
			break;
		case ExpUn:
			p->u.un.type = va_arg(args, ExpUnType);
			p->u.un.exp = va_arg(args, ExpNode*);
			break;
		case ExpCall:
			p->u.call.call = va_arg(args, CallNode*);
			break;
		case ExpNew:
			p->u.enew.type = va_arg(args, TypeNode*);
			p->u.enew.exp = va_arg(args, ExpNode*);
			break;
	}
	va_end(args);
	
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

	if(p->type == TypeRec)
	{
		freeTypeNode(p->u.next);
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
		case VarRec:
			freeVarNode(p->u.rec.next);
			freeExpNode(p->u.rec.exp);
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

