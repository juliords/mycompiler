
#include <stdio.h>
#include "tree.h"
#include "tree.print.h"

void printTabs(int i)
{
	while(i--)
	{
		print_space("\t");
	}
}

void printProgramNode(ProgramNode* p)
{
	ListNode* ln;

	if(!p) return;

	for(ln = p->dec; ln; ln = ln->next)
	{
		printDeclarationNode((DeclarationNode*)ln->data, 0);
		print_space("\n");
	}
}

void printDeclarationNode(DeclarationNode* p, int tabs)
{
	if(!p) return;

	switch(p->type)
	{
		case DecVar:
			printDecVarNode(p->u.var, tabs);
			break;
		case DecFunc:
			printDecFuncNode(p->u.func, tabs);
			print_space("\n");
			break;
	}
}

void printDecVarNode(ListNode* p, int tabs)
{
	ListNode* l;

	if(!p) return;

	for(l = p; l; l = l->next)
	{
		DecVarNode *v = (DecVarNode*)l->data;

		printTabs(tabs);
		printTypeNode(v->type);
		printf(" ");
		printf("%s;", v->name);
		print_space("\n");
	}
}

void printTypeNode(TypeNode* p)
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

void printDecFuncNode(DecFuncNode* p, int tabs)
{
	ListNode *l;

	if(!p) return;

	printTabs(tabs);
	printTypeNode(p->type);
	printf(" %s(", p->id);

	for(l = p->params; l; l = l->next)
	{
		if(l != p->params) 
		{
			printf(",");
			print_space(" ");
		}
		printParamNode((ParamNode*)l->data);
	}
	printf(")");

	printBlockNode(p->block, tabs);
}

void printParamNode(ParamNode* p)
{
	if(!p) return;

	printTypeNode(p->type);
	printf(" %s", p->id);
}

void printBlockNode(BlockNode* p, int tabs)
{
	ListNode* l;

	print_space("\n");
	printTabs(tabs);
	printf("{");
	print_space("\n");

	if(p)
	{
		printDecVarNode(p->var, tabs+1);

		for(l = p->cmd; l; l = l->next)
		{
			printCmdNode((CmdNode*)l->data, tabs+1);
			print_space("\n");
		}
	}

	printTabs(tabs);
	printf("}");
}

void printCmdNode(CmdNode* p, int tabs)
{
	if(!p) return;

	switch(p->type)
	{
		case CmdIf:
			printTabs(tabs);
			printf("if(");
			print_space(" ");
			printExpNode(p->u.i.cond);
			print_space(" ");
			printf(")");

			printCmdNode(p->u.i.cmd_if, tabs);

			if(p->u.i.cmd_else)
			{
				print_space("\n");
				printTabs(tabs);
				printf("else");

				printCmdNode(p->u.i.cmd_else, tabs);
			}
			break;

		case CmdWhile:
			printTabs(tabs);
			printf("while(");
			print_space(" ");
			printExpNode(p->u.w.cond);
			print_space(" ");
			printf(")");

			printCmdNode(p->u.w.cmd, tabs);
			break;

		case CmdAssig:
			printTabs(tabs);
			printVarNode(p->u.a.var);
			print_space(" ");
			printf("=");
			print_space(" ");
			printExpNode(p->u.a.exp);
			printf(";");
			break;

		case CmdRet:
			printTabs(tabs);
			printf("return");
			if(p->u.r.exp)
			{
				printf(" ");
				printExpNode(p->u.r.exp);
			}
			printf(";");
			break;

		case CmdCall: 
			printTabs(tabs);
			printCallNode(p->u.c.call);
			printf(";");
			break;

		case CmdBlock:
			printBlockNode(p->u.b.block, tabs);
			break;
	}
}

void printVarNode(VarNode* p)
{
	if(!p) return;

	switch(p->type)
	{
		case VarId:
			printf("%s", p->u.b.id);
			break;
		case VarArray:
			printVarNode(p->u.d.var);
			printf("[");
			printExpNode(p->u.d.exp);
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

			print_space(" ");
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
			print_space(" ");

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
		if(l != p->exp) 
		{
			printf(",");
			print_space(" ");
		}
		printExpNode((ExpNode*)l->data);
	}

	printf(")");
}



