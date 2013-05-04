
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "tree.h"

ListNode* newListNode(void *data, ListNode *next)
{
	ListNode *p = (ListNode*) malloc(sizeof(ListNode));
	if(!p)
	{
		fputs("Malloc FAILED: newListNode\n", stderr);
		exit(1);
	}

	p->data = data;
	p->next = next;

	return p;
}

ProgramNode* newProgramNode(ListNode *dec)
{
	ProgramNode *p = (ProgramNode*) malloc(sizeof(ProgramNode));
	if(!p)
	{
		fputs("Malloc FAILED: newProgramNode\n", stderr);
		exit(1);
	}

	p->dec = dec;

	return p;
}

DeclarationNode* newDeclarationNode(DeclarationType type, ... )
{
	va_list args;
	DeclarationNode *p = (DeclarationNode*) malloc(sizeof(DeclarationNode));
	if(!p)
	{
		fputs("Malloc FAILED: newDeclarationNode\n", stderr);
		exit(1);
	}

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
	DecVarNode *p = (DecVarNode*) malloc(sizeof(DecVarNode));
	if(!p)
	{
		fputs("Malloc FAILED: newDecVarNode\n", stderr);
		exit(1);
	}
	
	p->type = type;
	p->name = name;
	
	return p;
}

TypeNode* newTypeNode(TypeType type, ... )
{
	va_list args;
	TypeNode *p = (TypeNode*) malloc(sizeof(TypeNode));
	if(!p)
	{
		fputs("Malloc FAILED: newTypeNode\n", stderr);
		exit(1);
	}
	
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
	DecFuncNode *p = (DecFuncNode*) malloc(sizeof(DecFuncNode));
	if(!p)
	{
		fputs("Malloc FAILED: newDecFuncNode\n", stderr);
		exit(1);
	}
	
	p->type = type;
	p->id = id;
	p->params = params;
	p->block = block;
	
	return p;
}

ParamNode* newParamNode(TypeNode *type, char *id )
{
	ParamNode *p = (ParamNode*) malloc(sizeof(ParamNode));
	if(!p)
	{
		fputs("Malloc FAILED: newParamNode\n", stderr);
		exit(1);
	}
	
	p->type = type;
	p->id = id;
	
	return p;
}

BlockNode* newBlockNode(ListNode *var, ListNode *cmd )
{
	BlockNode *p = (BlockNode*) malloc(sizeof(BlockNode));
	if(!p)
	{
		fputs("Malloc FAILED: newBlockNode\n", stderr);
		exit(1);
	}
	
	p->var = var;
	p->cmd = cmd;
	
	return p;
}

CmdNode* newCmdNode(CmdType type, ... )
{
	va_list args;
	CmdNode *p = (CmdNode*) malloc(sizeof(CmdNode));
	if(!p)
	{
		fputs("Malloc FAILED: newCmdNode\n", stderr);
		exit(1);
	}
	
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
	VarNode *p = (VarNode*) malloc(sizeof(VarNode));
	if(!p)
	{
		fputs("Malloc FAILED: newVarNode\n", stderr);
		exit(1);
	}

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
	ExpNode *p = (ExpNode*) malloc(sizeof(ExpNode));
	if(!p)
	{
		fputs("Malloc FAILED: newExpNode\n", stderr);
		exit(1);
	}
	
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
	CallNode *p = (CallNode*) malloc(sizeof(CallNode));
	if(!p)
	{
		fputs("Malloc FAILED: newCallNode\n", stderr);
		exit(1);
	}
	
	p->id = id;
	p->exp = exp;
	
	return p;
}


