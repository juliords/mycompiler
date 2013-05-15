#include <stdio.h>
#include "tree.h"
#include "yacc.h"

YYSTYPE yylval;
extern FILE *yyin;
int yylex();

int main(int argc, char *argv[])
{
	int tk;

	if (argc < 2) yyin = stdin;
	else yyin = fopen(argv[1], "r");

	while( (tk = yylex()) )
	{
		switch(tk)
		{
			case TK_INT: 
				printf("TK_INT\n"); 
				break;

			case TK_INT_I: 
				printf("TK_INT_I (%d)\n", yylval.i); 
				break;

			case TK_CHAR: 
				printf("TK_CHAR\n"); 
				break;

			case TK_CHAR_I: 
				printf("TK_CHAR_I (%c)\n", yylval.c); 
				break;

			case TK_FLOAT: 
				printf("TK_FLOAT\n"); 
				break;

			case TK_FLOAT_I: 
				printf("TK_FLOAT_I (%f)\n", yylval.f); 
				break;

			case TK_VOID:
				printf("TK_VOID\n");
				break;

			case TK_STR:
				printf("TK_STR (%s)\n", yylval.s);
				break;

			case TK_NEW:
				printf("TK_NEW\n");
				break;

			case TK_ID:
				printf("TK_ID (%s)\n", yylval.s);
				break;

			case TK_IF:
				printf("TK_IF\n");
				break;

			case TK_ELSE:
				printf("TK_ELSE\n");
				break;

			case TK_WHILE:
				printf("TK_WHILE\n");
				break;
/*
			case TK_CMT:
				printf("TK_CMT (%s)\n", yylval.s);
				break;
*/
			case TK_RET:
				printf("TK_RET\n");
				break;

			case TK_PLUS:
				printf("TK_PLUS\n");
				break;

			case TK_MINUS:
				printf("TK_MINUS\n");
				break;

			case TK_AST:
				printf("TK_AST\n");
				break;

			case TK_SLASH:
				printf("TK_SLASH\n");
				break;

			case TK_EQUAL:
				printf("TK_EQUAL\n");
				break;

			case TK_LEQUAL:
				printf("TK_LEQUAL\n");
				break;

			case TK_GEQUAL:
				printf("TK_GEQUAL\n");
				break;

			case TK_LESS:
				printf("TK_LESS\n");
				break;

			case TK_GREATER:
				printf("TK_GREATER\n");
				break;

			case TK_NOT:
				printf("TK_NOT\n");
				break;

			case TK_AND:
				printf("TK_AND\n");
				break;

			case TK_OR:
				printf("TK_OR\n");
				break;

			default:
				printf("OTHER: %c\n", tk);
				break;
		}
	}

	return 0;
}


