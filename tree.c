
#include "tree.h"
#include "tree.check.h"
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

ListNode *catListNode(ListNode *a, ListNode *b)
{
	if(!a) return b;
	if(!b) return a;

	a->last->next = b;
	a->last = b->last;

	return a;
}

ProgramNode* newProgramNode(ListNode *dec)
{
	NEW(ProgramNode, p);

	p->dec = dec;

	return p;
}

DeclarationNode* newDeclarationNodeTypeDecVar(ListNode* var)
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

ListNode* newDecVarNode(TypeNode *type, ListNode *name)
{
	ListNode *p;
	
	for(p = name; p; p = p->next)
	{
		NEW(DecVarNode, v);

		v->type = type;
		v->name = (char*)p->data;
		v->nref = 0;
		p->data = v;
	}
	
	return name;
}

TypeNode* newTypeNodeTypePrim(BaseType prim)
{
	NEW(TypeNode, p);
	
	p->prim = prim;
	p->dims = 0;

	return p;
}

TypeNode* newTypeNodeTypeArray(TypeNode *t)
{
	t->dims++;
	return t;
}

DecFuncNode* newDecFuncNode(TypeNode *type, char *id, ListNode *params, BlockNode *block)
{
	NEW(DecFuncNode, p);
	
	p->type = type;
	p->id = id;
	p->params = params;
	p->block = block;

	p->nref = 0;
	
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
	p->u.b.id = id;
	p->u.b.dec = NULL;

	return p;
}

VarNode* newVarNodeVarArray(VarNode *var, ExpNode* exp)
{
	NEW(VarNode, p);

	p->type = VarArray;
	p->u.d.var = var;
	p->u.d.exp = exp;

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
	p->u.prim.type = PrimStr;
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

