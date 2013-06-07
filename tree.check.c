
#include <string.h>
#include "tree.h"
#include "tree.check.h"
#include "macro.h"

ListNode *var_global, *var_local, *func_list;

DecVarNode* getGlobalVar(char *id)
{
	ListNode *l;

	for(l = var_global; l; l = l->next)
	{
		DecVarNode *found = (DecVarNode*)l->data;

		if(!strcmp(found->name, id))
		{
			return found;
		}
	}

	return NULL;
}

DecVarNode* getLocalVar(char *id)
{
	ListNode *l;

	if(!var_local) return NULL;

	for(l = (ListNode*)var_local->data; l; l = l->next)
	{
		DecVarNode *found = (DecVarNode*)l->data;

		if(!strcmp(found->name, id))
		{
			return found;
		}
	}

	return NULL;
}

DecVarNode* getVar(char *id)
{
	DecVarNode *p;

	p = getLocalVar(id);
	if(p) return p;

	p = getGlobalVar(id);
	if(p) return p;

	return NULL;
}

void pushLocalVarList()
{
	NEW(ListNode, c);
	
	c->data = NULL;
	c->next = var_local;

	var_local = c;
}

DecFuncNode* getFunc(char *id)
{
	ListNode *l;

	for(l = func_list; l; l = l->next)
	{
		DecFuncNode *found = (DecFuncNode*)l->data;

		if(!strcmp(found->id, id))
		{
			return found;
		}
	}

	return NULL;
}

/* -------------------------------------------------------------------- */

TypeNode* copyTypeNode(TypeNode* t)
{
	NEW(TypeNode, p);

	p->prim = t->prim;
	p->dims = t->dims;

	return p;
}

/* return: 0 -> different ; 1 -> equal ; */
int compareTypeNode(TypeNode* a, TypeNode* b)
{
	if(!a || !b) 
		return 0;

	if(a->prim != b->prim)
		return 0;

	if(a->dims != b->dims)
		return 0;

	return 1;
}

TypeNode* getTypeFromVar(VarNode* v)
{
	NEW(TypeNode, p);
	p->prim = TypeInt;
	p->dims = 0;
	/* TODO */
	return p;
}

TypeNode* getTypeFromCall(CallNode* c)
{
	if(!c) return NULL;
	return copyTypeNode(c->dec->type);
}

TypeNode* getTypeFromExp(ExpNode *e)
{
	if(!e) return NULL;
	return copyTypeNode(e->atype);
}

/* -------------------------------------------------------------------- */

void checkProgramNode(ProgramNode* p)
{
	ListNode* ln;

	if(!p) return;

	for(ln = p->dec; ln; ln = ln->next)
	{
		checkDeclarationNode((DeclarationNode*)ln->data);
	}
}

void checkDeclarationNode(DeclarationNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case DecVar:
			checkDecVarNode(p->u.var, DecVarGlobal);
			break;
		case DecFunc:
			checkDecFuncNode(p->u.func);
			break;
	}
}

void checkDecVarNode(ListNode* p, DecVarType type)
{
	ListNode* l;

	if(!p) return;

	for(l = p; l; l = l->next)
	{
		DecVarNode *v = (DecVarNode*)l->data;

		v->context = type;

		switch(type)
		{
			case DecVarGlobal:
				if(!getGlobalVar(v->name))
				{
					var_global = newListNode(v, var_global);
				}
				else
				{
					/* TODO: ERROR (duplicated global variable) */
				}
				break;

			case DecVarLocal:
				if(!getLocalVar(v->name))
				{
					var_local->data = newListNode(v, (ListNode*)var_local->data);
				}
				else
				{
					/* TODO: ERROR (duplicated local variable) */
				}
				break;
		}
	}
}

void checkDecFuncNode(DecFuncNode* p)
{
	if(!p) return;

	if(!getFunc(p->id))
	{
		func_list = newListNode(p, func_list);
	}
	else
	{
		/* TODO: ERROR (duplicated function) */
	}

	pushLocalVarList();
	checkBlockNode(p->block);
}

void checkBlockNode(BlockNode* p)
{
	ListNode* l;

	if(p)
	{
		checkDecVarNode(p->var, DecVarLocal);

		for(l = p->cmd; l; l = l->next)
		{
			checkCmdNode((CmdNode*)l->data);
		}
	}
}

void checkCmdNode(CmdNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case CmdIf:
			checkExpNode(p->u.i.cond);
			checkCmdNode(p->u.i.cmd_if);
			checkCmdNode(p->u.i.cmd_else);
			break;

		case CmdWhile:
			checkExpNode(p->u.w.cond);
			checkCmdNode(p->u.w.cmd);
			break;

		case CmdAssig:
			checkVarNode(p->u.a.var);
			checkExpNode(p->u.a.exp);
			/* TODO: check type */
			break;

		case CmdRet:
			checkExpNode(p->u.a.exp);
			/* TODO: check type */
			break;

		case CmdCall: 
			checkCallNode(p->u.c.call);
			break;

		case CmdBlock:
			checkBlockNode(p->u.b.block);
			break;
	}
}

void checkVarNode(VarNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case VarId:
			break;
		case VarArray:
			checkVarNode(p->u.v.array);
			checkExpNode(p->u.v.exp);
			break;
	}

	/* TODO: link */
	/* TODO: check exp */
}

void checkExpNode(ExpNode *p)
{
	if(!p) return;

	switch(p->type)
	{
		case ExpVar:
			checkVarNode(p->u.var.var);
			p->atype = getTypeFromVar(p->u.var.var);
			break;

		case ExpValue:
		{
			NEW(TypeNode, t);
			t->dims = 0;

			switch(p->u.prim.type)
			{
				case PrimInt:
					t->prim = TypeInt;
					break;

				case PrimFloat:
					t->prim = TypeFloat;
					break;

				case PrimChar:
					t->prim = TypeChar;
					break;

				case PrimStr:
					t->prim = TypeChar;
					t->dims = 1;
					break;
			}
			
			p->atype = t;
			break;
		}

		case ExpBin:
		{
			TypeNode *l, *r;
			checkExpNode(p->u.bin.left);
			checkExpNode(p->u.bin.right);
			l = getTypeFromExp(p->u.bin.left);
			r = getTypeFromExp(p->u.bin.right);

			if(!l || !r || l->dims > 0 || r->dims > 0 ||
			   l->prim == TypeVoid || r->prim == TypeVoid)
			{
				/* TODO: ERROR */
				p->atype = NULL;
			}

			switch(p->u.bin.type)
			{
				case ExpBinPlus:
				case ExpBinMinus:
				case ExpBinMult:
				case ExpBinDiv:
				{
					if(l->prim == TypeFloat || r->prim == TypeFloat)
						r->prim = TypeFloat;
					else
						r->prim = TypeInt;
					break;
				}

				case ExpBinEQ:
				case ExpBinLE:
				case ExpBinGE:
				case ExpBinLT:
				case ExpBinGT:
				case ExpBinAnd:
				case ExpBinOr:
				{
					r->prim = TypeInt;
					break;
				}
			}
			free(l);
			p->atype = r;
			break;
		}

		case ExpUn:
		{
			TypeNode *t;
			checkExpNode(p->u.un.exp);
			t = getTypeFromExp(p->u.un.exp);

			if(!t || t->dims > 0 || t->prim == TypeVoid)
			{
				/* TODO: ERROR */
				p->atype = NULL;
			}

			switch(p->u.un.type)
			{
				case ExpUnMinus: 
					if(t->prim == TypeChar)	t->prim = TypeInt;
					break;

				case ExpUnNot: 
					t->prim = TypeInt; 
					break;
			}

			p->atype = t;
			break;
		}

		case ExpCall:
			checkCallNode(p->u.call.call);
			p->atype = getTypeFromCall(p->u.call.call); 
			break;

		case ExpNew:
		{
			TypeNode *t = copyTypeNode(p->u.enew.type);
			t->dims++;
			p->atype = t;
			break;
		}
	}
}

void checkCallNode(CallNode *p)
{
	if(!p) return;

	if( (p->dec = getFunc(p->id)) ) 
	{
		p->dec->nref++;
	}
	else
	{
		/* TODO: ERROR(function not declared) */
	}

	/* TODO: check params type */
}

