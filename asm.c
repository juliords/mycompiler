
#include <stdio.h>
#include "tree.h"
#include "asm.h"

void asmProgramNode(ProgramNode* p)
{
	ListNode* ln;

	if(!p) return;

	for(ln = p->dec; ln; ln = ln->next)
	{
		asmDeclarationNode((DeclarationNode*)ln->data);
	}
}

void asmDeclarationNode(DeclarationNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case DecVar:
			asmDecVarNode(p->u.var);
			break;
		case DecFunc:
			asmDecFuncNode(p->u.func);
	}
}

void asmDecVarNode(ListNode* p)
{
	ListNode* l;

	if(!p) return;

	for(l = p; l; l = l->next)
	{
		DecVarNode *v = (DecVarNode*)l->data;

		asmTypeNode(v->type);
		printf("%s;", v->name);
	}
}

void asmTypeNode(TypeNode* p)
{
	int i;
	if(!p) return;

	switch(p->prim)
	{
		case TypeInt:
			printf("int");
			break;

		case TypeChar:
			printf("char");
			break;

		case TypeFloat:
			printf("float");
			break;

		case TypeVoid:
			printf("void");
			break;
	}
	for (i = 0; i < p->dims; i++) 
	{
		printf("[]");
	}
}

void asmDecFuncNode(DecFuncNode* p)
{
	ListNode *l;

	if(!p) return;

	asmTypeNode(p->type);
	printf(" %s(", p->id);

	for(l = p->params; l; l = l->next)
	{
		asmParamNode((DecVarNode*)l->data);
	}

	asmBlockNode(p->block);
}

void asmParamNode(DecVarNode* p)
{
	if(!p) return;

	asmTypeNode(p->type);
	printf(" %s", p->name);
}

void asmBlockNode(BlockNode* p)
{
	ListNode* l;

	if(p)
	{
		asmDecVarNode(p->var);

		for(l = p->cmd; l; l = l->next)
		{
			asmCmdNode((CmdNode*)l->data);
		}
	}
}

void asmCmdNode(CmdNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case CmdIf:
			asmExpNode(p->u.i.cond);
			asmCmdNode(p->u.i.cmd_if);

			if(p->u.i.cmd_else)
			{
				asmCmdNode(p->u.i.cmd_else);
			}
			break;

		case CmdWhile:
			asmExpNode(p->u.w.cond);
			asmCmdNode(p->u.w.cmd);
			break;

		case CmdAssig:
			asmVarNode(p->u.a.var);
			asmExpNode(p->u.a.exp);
			break;

		case CmdRet:
			if(p->u.r.exp)
			{
				asmExpNode(p->u.r.exp);
			}
			break;

		case CmdCall: 
			asmCallNode(p->u.c.call);
			break;

		case CmdBlock:
			asmBlockNode(p->u.b.block);
			break;
	}
}

void asmVarNode(VarNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case VarId:
			printf("%s", p->u.b.id);
			break;
		case VarArray:
			asmVarNode(p->u.d.var);
			asmExpNode(p->u.d.exp);
			break;
	}
}

void asmExpNode(ExpNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case ExpVar:
			asmVarNode(p->u.var.var);
			break;

		case ExpConst:
			switch(p->u.prim.type)
			{
				case PrimInt:
					printf("%d", p->u.prim.u.i);
					break;

				case PrimFloat:
					printf("%g", p->u.prim.u.f);
					break;

				case PrimChar:
					printf("%c", p->u.prim.u.c);
					break;

				case PrimStr:
					printf("%s", p->u.prim.u.s);
					break;
			}
			break;

		case ExpBin:
			asmExpNode(p->u.bin.left);

			switch(p->u.bin.type)
			{
				case ExpBinPlus: printf("+"); break;
				case ExpBinMinus: printf("-"); break;
				case ExpBinMult: printf("*"); break;
				case ExpBinDiv: printf("/"); break;
				case ExpBinEQ: printf("=="); break;
				case ExpBinLE: printf("<="); break;
				case ExpBinGE: printf(">="); break;
				case ExpBinLT: printf("<"); break;
				case ExpBinGT: printf(">"); break;
				case ExpBinAnd: printf("&&"); break;
				case ExpBinOr: printf("||"); break;
			}

			asmExpNode(p->u.bin.right);
			break;

		case ExpUn:
			switch(p->u.un.type)
			{
				case ExpUnMinus: printf("-"); break;
				case ExpUnNot: printf("!"); break;
			}
			asmExpNode(p->u.un.exp);
			break;

		case ExpCall:
			asmCallNode(p->u.call.call); 
			break;

		case ExpNew:
			asmTypeNode(p->u.enew.type);
			asmExpNode(p->u.enew.exp);
			break;

		case ExpCast:
			asmTypeNode(p->atype);
			asmExpNode(p->u.cast.exp);
	}
}

void asmCallNode(CallNode *p)
{
	ListNode *l;

	if(!p) return;

	printf("%s(", p->id);

	for(l = p->exp; l; l = l->next)
	{
		asmExpNode((ExpNode*)l->data);
	}
}



