
#include <string.h>
#include "tree.h"
#include "tree.check.h"
#include "macro.h"

void debug(EnvData *env)
{
	ListNode* l;

	printf("Vars: ");
	for(l = env->var_list; l; l = l->next)
	{
		DecVarNode *v = (DecVarNode*)l->data;

		if(!v) printf("NULL ");
		else printf("%s ", v->name);
	}
	printf("\nFuncs: ");
	for(l = env->func_list; l; l = l->next)
	{
		DecFuncNode *v = (DecFuncNode*)l->data;
		printf("%s ", v->id);
	}
	if(env->curr_func) printf("\nCurr_func: %s\n", env->curr_func->id);
	printf("----------------------------------\n");
}

EnvData* initEnvData()
{
	NEW(EnvData, e);

	e->var_list = NULL;
	e->func_list = NULL;
	e->curr_func = NULL;

	return e;
}

DecVarNode* getDecVar(EnvData *env, char *id, bool lastLevel)
{
	ListNode *l;

	for(l = env->var_list; l; l = l->next)
	{
		DecVarNode *found = (DecVarNode*)l->data;
		if(!found)
		{
			if(lastLevel) break;
			else continue;
		}

		if(!strcmp(found->name, id))
		{
			return found;
		}
	}

	return NULL;
}

void pushBlockVar(EnvData *env)
{
	if(!env) return;
	env->var_list = newListNodeFirst(NULL, env->var_list);
}

void popBlockVar(EnvData *env)
{
	ListNode *l, *next;
	if(!env) return;

	for(l = env->var_list; l; l = next)
	{
		next = l->next;

		if(!l->data)
		{
			free(l);
			env->var_list = next;
			break;
		}

		free(l);
	}
}

DecFuncNode* getDecFunc(EnvData *env, char *id)
{
	ListNode *l;

	for(l = env->func_list; l; l = l->next)
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

	if(!t)
	{
		p->prim = TypeVoid;
		p->dims = 0;
		return p;
	}

	p->prim = t->prim;
	p->dims = t->dims;

	return p;
}

/* return: 0 -> not equal ; 1 -> equal ; */
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
	if(!v) return NULL;
	return copyTypeNode(v->atype);
}

TypeNode* getTypeFromCall(CallNode* c)
{
	if(!c || !c->dec) return NULL;
	return copyTypeNode(c->dec->type);
}

TypeNode* getTypeFromExp(ExpNode *e)
{
	if(!e)
	{
		NEW(TypeNode, p);
		p->prim = TypeVoid;
		p->dims = 0;
		return p;
	}
	return copyTypeNode(e->atype);
}

char *getCurrFuncName(EnvData *env)
{
	if(!env || !env->curr_func) return NULL;
	return env->curr_func->id;
}

TypeNode *getTypeFromCurrFunc(EnvData *env)
{
	if(!env || !env->curr_func) return NULL;
	return copyTypeNode(env->curr_func->type);
}

char *getVarName(VarNode* v)
{
	if(v) switch(v->type)
	{
		case VarId:
			return v->u.b.id;

		case VarArray:
			return getVarName(v->u.d.var);
	}
	return "< without name >";
}

/* -------------------------------------------------------------------- */

void checkProgramNode(ProgramNode* p)
{
	ListNode* ln;
	EnvData *env;

	if(!p) return;

	env = initEnvData();
	for(ln = p->dec; ln; ln = ln->next)
	{
		checkDeclarationNode(env, (DeclarationNode*)ln->data);
	}
}

void checkDeclarationNode(EnvData* env, DeclarationNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case DecVar:
			checkDecVarNode(env, p->u.var, DecVarGlobal);
			break;
		case DecFunc:
			checkDecFuncNode(env, p->u.func);
			break;
	}
}

void checkDecVarNode(EnvData* env, ListNode* p, DecVarType type)
{
	ListNode* l;

	if(!p) return;

	for(l = p; l; l = l->next)
	{
		DecVarNode *v = (DecVarNode*)l->data;

		v->context = type;

		if(!getDecVar(env, v->name, true))
		{
			env->var_list = newListNodeFirst(v, env->var_list);
		}
		else
		{
			fprintf(stderr,"ERROR: redeclaration of variable '%s'\n", v->name);
		}
	}
}

void checkDecFuncNode(EnvData* env, DecFuncNode* p)
{
	if(!p) return;

	if(!getDecFunc(env, p->id))
	{
		env->func_list = newListNodeFirst(p, env->func_list);
	}
	else
	{
		fprintf(stderr,"ERROR: redefinition of function '%s'\n", p->id);
	}

	pushBlockVar(env);
	env->curr_func = p;

	checkDecVarNode(env, p->params, DecVarParam);
	checkBlockNode(env, p->block);

	env->curr_func = NULL;
	popBlockVar(env);
}

void checkBlockNode(EnvData* env, BlockNode* p)
{
	ListNode* l;
	if(!p) return;

	pushBlockVar(env);
	
	checkDecVarNode(env, p->var, DecVarLocal);
	for(l = p->cmd; l; l = l->next)
	{
		checkCmdNode(env, (CmdNode*)l->data);
	}

	popBlockVar(env);
}

void checkCmdNode(EnvData* env, CmdNode* p)
{
	TypeNode *tv, *te;

	if(!p) return;

	switch(p->type)
	{
		case CmdIf:
			checkExpNode(env, p->u.i.cond);
			checkCmdNode(env, p->u.i.cmd_if);
			checkCmdNode(env, p->u.i.cmd_else);
			break;

		case CmdWhile:
			checkExpNode(env, p->u.w.cond);
			checkCmdNode(env, p->u.w.cmd);
			break;

		case CmdAssig:
			checkVarNode(env, p->u.a.var);
			checkExpNode(env, p->u.a.exp);

			tv = getTypeFromVar(p->u.a.var);
			te = getTypeFromExp(p->u.a.exp);
			if(!compareTypeNode(tv, te))
			{
				fprintf(stderr,"ERROR: In function '%s': incompatible expression assigned to variable '%s'\n", getCurrFuncName(env),  getVarName(p->u.a.var));
			}
			free(te); free(tv);

			break;

		case CmdRet:
			checkExpNode(env, p->u.r.exp);

			te = getTypeFromExp(p->u.r.exp);
			tv = getTypeFromCurrFunc(env);
			if(!compareTypeNode(tv, te))
			{
				fprintf(stderr,"ERROR: In function '%s': incompatible return type\n", getCurrFuncName(env));
			}
			free(te); free(tv);

			break;

		case CmdCall: 
			checkCallNode(env, p->u.c.call);
			break;

		case CmdBlock:
			checkBlockNode(env, p->u.b.block);
			break;
	}
}

void checkVarNode(EnvData* env, VarNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case VarId:
			if( (p->u.b.dec = getDecVar(env, p->u.b.id, false)) )
			{
				p->u.b.dec->nref++;
				p->atype = copyTypeNode(p->u.b.dec->type);
			}
			else
			{
				fprintf(stderr,"ERROR: In function '%s': '%s' undeclared\n", getCurrFuncName(env), p->u.b.id);
			}
			break;
		case VarArray:
		{
			TypeNode* t;
			checkVarNode(env, p->u.d.var);
			checkExpNode(env, p->u.d.exp);

			t = p->u.d.exp->atype;
			if(t && t->prim == TypeInt && t->dims == 0)
			{
				p->atype = copyTypeNode(p->u.d.var->atype);
				p->atype->dims--;

				if(p->atype->dims < 0)
				{
					fprintf(stderr,"ERROR: In function '%s': invalid index to scalar variable '%s'\n", getCurrFuncName(env), getVarName(p->u.d.var));
				}
			}
			else
			{
				fprintf(stderr,"ERROR: In function '%s': index expression of var '%s' must be an integer\n", getCurrFuncName(env), getVarName(p->u.d.var));
			}
			break;
		}
	}
}

void checkExpNode(EnvData* env, ExpNode *p)
{
	if(!p) return;

	switch(p->type)
	{
		case ExpVar:
			checkVarNode(env, p->u.var.var);
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
			checkExpNode(env, p->u.bin.left);
			checkExpNode(env, p->u.bin.right);
			l = getTypeFromExp(p->u.bin.left);
			r = getTypeFromExp(p->u.bin.right);

			if(!l || !r || l->dims > 0 || r->dims > 0 ||
			   l->prim == TypeVoid || r->prim == TypeVoid)
			{
				fprintf(stderr,"ERROR: In function '%s': invalid expression of non-numeric type\n", getCurrFuncName(env));
				r->prim = TypeVoid;
			}
			else switch(p->u.bin.type)
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
			checkExpNode(env, p->u.un.exp);
			t = getTypeFromExp(p->u.un.exp);

			if(!t || t->dims > 0 || t->prim == TypeVoid)
			{
				fprintf(stderr,"ERROR: In function '%s': invalid expression using of non-numeric type\n", getCurrFuncName(env));
				t->prim = TypeVoid;
			}
			else switch(p->u.un.type)
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
			checkCallNode(env, p->u.call.call);
			p->atype = getTypeFromCall(p->u.call.call); 
			break;

		case ExpNew:
		{
			TypeNode *t = copyTypeNode(p->u.enew.type);
			checkExpNode(env, p->u.enew.exp);

			t->dims++;
			p->atype = t;
			break;
		}
	}
}

void checkCallNode(EnvData* env, CallNode *p)
{
	ListNode *expl, *parl;
	if(!p) return;

	if( (p->dec = getDecFunc(env, p->id)) ) 
	{
		p->dec->nref++;

		for(expl = p->exp,parl = p->dec->params; expl && parl; expl = expl->next, parl = parl->next)
		{
			ExpNode *exp = (ExpNode*)expl->data;
			DecVarNode* par = (DecVarNode*)parl->data;
			TypeNode *te, *tp;

			checkExpNode(env, exp);

			te = getTypeFromExp(exp);
			tp = par->type;

			if(!compareTypeNode(te, tp))
			{
				fprintf(stderr,"ERROR: In function '%s': incompatible type of param '%s' -> '%s'\n", getCurrFuncName(env), par->name, p->id);
			}
		}

		if(parl)
		{
			fprintf(stderr,"ERROR: In function '%s': insuficient number of arguments -> '%s'\n", getCurrFuncName(env), p->id);
		}

		if(expl)
		{
			fprintf(stderr,"ERROR: In function '%s': number of arguments exceeded -> '%s'\n", getCurrFuncName(env), p->id);
		}
	}
	else
	{
		fprintf(stderr,"ERROR: In function '%s': '%s' undeclared\n", getCurrFuncName(env), p->id);
	}

}

