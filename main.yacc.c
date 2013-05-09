#include <stdio.h>
#include "tree.h"
#include "yacc.h"

extern ProgramNode *tree;
int yyparse();

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

