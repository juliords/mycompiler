#include <stdio.h>
#include "tree.h"
#include "tree.print.h"
#include "yacc.h"

extern FILE *yyin;

extern ProgramNode *tree;
int yyparse();

void yyerror(char *s)
{
	printf("%s\n", s);
}

int main(int argc, char *argv[])
{
	int ret;	

	if (argc < 2) yyin = stdin;
	else yyin = fopen(argv[1], "r");

	ret = yyparse();

	printProgramNode(tree);

	return ret;
}

