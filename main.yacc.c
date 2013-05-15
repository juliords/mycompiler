#include <stdio.h>
#include "tree.h"
#include "tree_print.h"
#include "yacc.h"

extern ProgramNode *tree;
int yyparse();

ListNode *exp_lst, *cmd_lst;

void yyerror(char *s)
{
	printf("%s\n", s);
}

int main()
{
	int ret = yyparse();	

	printProgramNode(tree);
	freeProgramNode(tree);

	return ret;
}

