
#include <string.h>
#include "tree.h"
#include "tree.check.h"
#include "macro.h"

ListNode *var_global, *var_static, *var_local;

int addGlobalVar(DecVarNode* v)
{
	if(getGlobalVar(v->name)) return 0;

	var_global = newListNode(v, var_global);
	return 1;
}

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

int addContextVar(DecVarNode* v)
{
	if(!var_local) return 0;
	if(getContextVar(v->name)) return 0;

	var_local->data = newListNode(v, (ListNode*)var_local->data);
	return 1;
}

DecVarNode* getContextVar(char *id)
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

int addVarList(ListNode *vars, DecVarType type)
{
	ListNode *l;
	
	for(l = vars; l; l = l->next)
	{
		DecVarNode *v = (DecVarNode*)l->data;

		v->context = type;

		switch(type)
		{
			case DecVarGlobal:
				addGlobalVar(v);
				break;

			case DecVarLocal:
				addContextVar(v);
				break;
		}
	}

	return 1;
}

DecVarNode* getVar(char *id)
{
	DecVarNode *p;

	p = getContextVar(id);
	if(p) return p;

	p = getGlobalVar(id);
	if(p) return p;

	return NULL;
}

void pushContextVarList()
{
	NEW(ListNode, c);
	
	c->data = NULL;
	c->next = var_local;

	var_local = c;
}

void popContextVarList()
{
	ListNode *p, *next;

	if(!var_local) return;

	for(p = (ListNode*)var_local->data; p; p = next)
	{
		next = p->next;
		free(p);
	}

	p = var_local;
	free(var_local);
	var_local = p->next;
}

