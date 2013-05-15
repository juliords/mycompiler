
#include <stdio.h>
#include "tree.h"
#include "tree_print.h"

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



