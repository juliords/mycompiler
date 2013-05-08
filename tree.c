
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "tree.h"

ListNode* newListNode(void *data, ListNode *list)
{
	ListNode *p = (ListNode*) malloc(sizeof(ListNode)), *l;
	if(!p)
	{
		fputs("Malloc FAILED: newListNode\n", stderr);
		exit(1);
	}

	p->data = data;
	p->next = NULL;

	if(list)
	{
		for(l = list; l->next; l = l->next); 

		l->next = p;
		return list;
	}
	else
	{
		return p;
	}
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

/* ------------------------------------------------------ */

static int num_tabs = 0;

void printTabs(int i)
{
	while(i--)
	{
		printf("\t");
	}
}

void printProgramNode(ProgramNode* p)
{
	ListNode* ln;

	if(!p) return;

	for(ln = p->dec; ln; ln = ln->next)
	{
		printDeclarationNode((DeclarationNode*)ln->data);
	}
}

void printDeclarationNode(DeclarationNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case DecVar:
			printDecVarNode(p->u.var);
			printf("\n");
			break;
		case DecFunc:
			printDecFuncNode(p->u.func);
			printf("\n");
			break;
	}
}

void printDecVarNode(DecVarNode* p)
{
	ListNode* l;

	if(!p) return;

	printTabs(num_tabs);
	printTypeNode(p->type);
	printf(" ");

	for(l = p->name; l; l = l->next)
	{
		if(l != p->name) printf(", ");
		printf("%s", (char*)l->data);
	}
	printf(";\n");
}

void printTypeNode(TypeNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case TypeBase:

			switch(p->u.base)
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
			break;

		case TypeRec:
			printTypeNode(p->u.next);
			printf("[]");
			break;
	}
}

void printDecFuncNode(DecFuncNode* p)
{
	ListNode *l;

	if(!p) return;

	printTypeNode(p->type);
	printf(" %s(", p->id);

	for(l = p->params; l; l = l->next)
	{
		if(l != p->params) printf(", ");
		printParamNode((ParamNode*)l->data);
	}
	printf(")");

	num_tabs++;
	printBlockNode(p->block);
	num_tabs--;
}

void printParamNode(ParamNode* p)
{
	if(!p) return;

	printTypeNode(p->type);
	printf(" %s", p->id);
}

void printBlockNode(BlockNode* p)
{
	ListNode* l;

	printf("\n");
	printTabs(num_tabs-1);
	printf("{\n");

	if(p)
	{
		for(l = p->var; l; l = l->next)
		{
			printDecVarNode((DecVarNode*)l->data);
		}

		for(l = p->cmd; l; l = l->next)
		{
			printCmdNode((CmdNode*)l->data);
		}
	}

	printTabs(num_tabs-1);
	printf("}\n");
}

void printCmdNode(CmdNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case CmdIf:
			printTabs(num_tabs);
			printf("if( ");
			printExpNode(p->u.i.cond);
			printf(" )");

			num_tabs++;
			printCmdNode(p->u.i.cmd_if);
			num_tabs--;

			if(p->u.i.cmd_else)
			{
				printTabs(num_tabs);
				printf("else");

				num_tabs++;
				printCmdNode(p->u.i.cmd_else);
				num_tabs--;
			}
			printf("\n");
			break;

		case CmdWhile:
			printTabs(num_tabs);
			printf("while( ");
			printExpNode(p->u.w.cond);
			printf(" )");

			num_tabs++;
			printCmdNode(p->u.w.cmd);
			num_tabs--;

			printf("\n");
			break;

		case CmdAssig:
			printTabs(num_tabs);
			printVarNode(p->u.a.var);
			printf(" = ");
			printExpNode(p->u.a.exp);
			printf(";\n");
			break;

		case CmdRet:
			printTabs(num_tabs);
			printf("return ");
			printExpNode(p->u.r.exp);
			printf(";\n");
			break;

		case CmdCall: 
			printTabs(num_tabs);
			printCallNode(p->u.c.call);
			printf(";\n");
			break;

		case CmdBlock:
			printBlockNode(p->u.b.block);
			break;
	}
}

void printVarNode(VarNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case VarId:
			printf("%s", p->u.id);
			break;
		case VarRec:
			printVarNode(p->u.rec.next);
			printf("[");
			printExpNode(p->u.rec.exp);
			printf("]");
			break;
	}
}

void printExpNode(ExpNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case ExpVar:
			printVarNode(p->u.var.var);
			break;

		case ExpValue:
			switch(p->u.prim.type)
			{
				case PrimInt:
					printf("%d", p->u.prim.u.i);
					break;

				case PrimFloat:
					printf("%f", p->u.prim.u.f);
					break;

				case PrimChar:
					printf("'");
					switch(p->u.prim.u.c)
					{
						case '\n':
							printf("\\n");
							break;

						case '\t':
							printf("\\t");
							break;

						case '\'':
							printf("\\'");
							break;

						default:
							printf("%c", p->u.prim.u.c);
							break;
					}
					printf("'");

					break;

				case PrimStr:
				{
					char *s;
					printf("\"");
					for(s = p->u.prim.u.s; *s; s++)
					{
						switch(*s)
						{
							case '\n':
								printf("\\n");
								break;

							case '\t':
								printf("\\t");
								break;

							case '"':
								printf("\\\"");
								break;

							default:
								printf("%c", *s);
								break;
						}
					}
					printf("\"");
					break;
				}
			}
			break;

		case ExpBin:
			printf("(");
			printExpNode(p->u.bin.left);

			switch(p->u.bin.type)
			{
				case ExpBinPlus: printf(" + "); break;
				case ExpBinMinus: printf(" - "); break;
				case ExpBinMult: printf(" * "); break;
				case ExpBinDiv: printf(" / "); break;
				case ExpBinEQ: printf(" == "); break;
				case ExpBinLE: printf(" <= "); break;
				case ExpBinGE: printf(" >= "); break;
				case ExpBinLT: printf(" < "); break;
				case ExpBinGT: printf(" > "); break;
				case ExpBinAnd: printf(" && "); break;
				case ExpBinOr: printf(" || "); break;
			}

			printExpNode(p->u.bin.right);
			printf(")");
			break;

		case ExpUn:
			switch(p->u.un.type)
			{
				case ExpUnMinus: printf("-"); break;
				case ExpUnNot: printf("!"); break;
			}
			printExpNode(p->u.un.exp);
			break;

		case ExpCall:
			printCallNode(p->u.call.call); 
			break;

		case ExpNew:
			printf("new ");
			printTypeNode(p->u.enew.type);
			printf("[");
			printExpNode(p->u.enew.exp);
			printf("]");
			break;
	}
}

void printCallNode(CallNode *p)
{
	ListNode *l;

	if(!p) return;

	printf("%s(", p->id);

	for(l = p->exp; l; l = l->next)
	{
		if(l != p->exp) printf(", ");
		printExpNode((ExpNode*)l->data);
	}

	printf(")");
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

