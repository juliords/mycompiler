
#include <string.h>
#include "tree.h"
#include "tree.check.h"
#include "macro.h"

ListNode *var_global, *var_static, *var_context;

int addGlobalVar(DecVarNode* v)
{
	ListNode *l;

	for(l = var_global; l; l = l->next)
	{
		DecVarNode *found = (DecVarNode*)l->data;

		if(!strcmp(found->name, v->name))
		{
			return 0;
		}
	}

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

void addContextVar()
{
	NEW(ListNode, c);
	
	c->data = NULL;
	c->next = var_context;

	var_context = c;
}

void delContextVar()
{
	ListNode *p, *next;

	if(!var_context) return;

	for(p = (ListNode*)var_context->data; p; p = next)
	{
		next = p->next;
		free(p);
	}

	p = var_context;
	free(var_context);
	var_context = p->next;
}

