#include <stdio.h>
#include "tree.h"
#include "tree.check.h"
#include "tree.print.h"
#include "asm.h"
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

	checkProgramNode(tree);
#if 0
	printProgramNode(tree);
#else
	asmProgramNode(tree);
#endif

	return ret;
}

