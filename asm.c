
#include <stdio.h>
#include "tree.h"
#include "asm.h"

int next_label()
{
	static int l = 0;
	return ++l;
}

int float_to_int(float f)
{
	union
	{
		int i;
		float f;
	} v;

	v.f = f;

	return v.i;
}

int multiple_of_n(int i, int n)
{
	if(i % n == 0) return i;
	return i + (n - (i % n));
}

int is_char(TypeNode* t)
{
	if(t && t->prim == TypeChar && t->dims == 0)
		return 1;

	return 0;
}

int max_esp(BlockNode *blk, int esp)
{
	ListNode *lst;
	int max = esp;

	if(!blk) return 0;

	for(lst = blk->var; lst; lst = lst->next)
	{
		DecVarNode *v = (DecVarNode*)lst->data;

		if(is_char(v->type))
		{
			max += 1;
		}
		else
		{
			max = multiple_of_n(max, 4);
			max += 4;
		}
		v->stack_offset = max;
	}
	max = multiple_of_n(max, 4);

	/* check nested blocks */
	for(lst = blk->cmd; lst; lst = lst->next)
	{
		CmdNode *cmd = (CmdNode*)lst->data;

		if(cmd->type == CmdBlock)
		{
			int t = max_esp(cmd->u.b.block, esp);
			if(t > max) t = max;
		}
	}

	return max;
}

/* -------------------------------------------------------------------- */

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

		if(v->context == DecVarGlobal)
		{
			int s;
			if(is_char(v->type)) s = 1;
			else s = 4;

			printf("\t.comm\t%s,%d,%d\n", v->name, s, s);
		}
	}
}

void asmDecFuncNode(DecFuncNode* p)
{
	ListNode *l;
	int esp;

	if(!p || !p->block) return;

	/*asmTypeNode(p->type);*/

	printf("\n\t.globl\t%s\n", p->id);
	printf("\t.type\t%s, @function\n", p->id);
	printf("%s:\n", p->id);
	printf("\tpushq\t%%rbp\n");
	printf("\tmovq\t%%rsp, %%rbp\n");

	/* count stack pointer max */
	esp = multiple_of_n(max_esp(p->block, 0), 16);
	if(esp) printf("\tsubq\t$%d, %%rsp\n", esp);

	esp = 8;
	for(l = p->params; l; l = l->next)
	{
		DecVarNode *v = (DecVarNode*)l->data;

		v->stack_offset = esp;
		esp += 4;
	}

	asmBlockNode(p->block);

	printf("\tleave\n");
	printf("\tret\n");
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
			/*printf("%s", p->u.b.id);*/
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
					/*printf("%d", p->u.prim.u.i);*/
					break;

				case PrimFloat:
					/*printf("%g", p->u.prim.u.f);*/
					break;

				case PrimChar:
					/*printf("%c", p->u.prim.u.c);*/
					break;

				case PrimStr:
					/*printf("%s", p->u.prim.u.s);*/
					break;
			}
			break;

		case ExpBin:
			asmExpNode(p->u.bin.left);

			switch(p->u.bin.type)
			{
				case ExpBinPlus: /*printf("+");*/ break;
				case ExpBinMinus: /*printf("-");*/ break;
				case ExpBinMult: /*printf("*");*/ break;
				case ExpBinDiv: /*printf("/");*/ break;
				case ExpBinEQ: /*printf("==");*/ break;
				case ExpBinLE: /*printf("<=");*/ break;
				case ExpBinGE: /*printf(">=");*/ break;
				case ExpBinLT: /*printf("<");*/ break;
				case ExpBinGT: /*printf(">");*/ break;
				case ExpBinAnd: /*printf("&&");*/ break;
				case ExpBinOr: /*printf("||");*/ break;
			}

			asmExpNode(p->u.bin.right);
			break;

		case ExpUn:
			switch(p->u.un.type)
			{
				case ExpUnMinus: /*printf("-");*/ break;
				case ExpUnNot: /*printf("!");*/ break;
			}
			asmExpNode(p->u.un.exp);
			break;

		case ExpCall:
			asmCallNode(p->u.call.call); 
			break;

		case ExpNew:
			/*asmTypeNode(p->u.enew.type);*/
			asmExpNode(p->u.enew.exp);
			break;

		case ExpCast:
			/*asmTypeNode(p->atype);*/
			asmExpNode(p->u.cast.exp);
	}
}

void asmCallNode(CallNode *p)
{
	ListNode *l;

	if(!p) return;

	/*printf("%s(", p->id);*/

	for(l = p->exp; l; l = l->next)
	{
		asmExpNode((ExpNode*)l->data);
	}
}



