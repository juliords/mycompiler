
%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "tree.h"
#include "y.tab.h"

#if MAINLEX
YYSTYPE yylval;
#endif

char* strcpy_malloc(char* str)
{
	char *s = (char*) malloc(strlen(str)+1);
	if(!s)
	{
		fputs("Malloc FAILED: LEX\n", stderr);
		exit(1);
	}
	strcpy(s, str);
	return s;
}

char* remove_wrap_char (char *s)
{
	int i = 0;

	while( s[i] ) i++;
	s[i-1] = '\0';

	return s+1;
}

void convert_escaped_char (char *s)
{
	int i, j;

	for(i = j = 0; s[j]; i++, j++)
	{
		if( s[j] == '\\' )
		{
			j++;

			switch( s[j] )
			{
				case 'n':
					s[i] = '\n';
					break;

				case 't':
					s[i] = '\t';
					break;

				default: s[i] = s[j];
			}
		}
		else s[i] = s[j];
	}

	s[i] = '\0';
}

%}


ESCAPE          ['"a-zA-Z]
IDENTIFIER	[_a-zA-Z][a-zA-Z0-9]*
COMMENT		"/*"([^*]|"\n"|"*"[^/])*"*"*"*/"
INTEGER		"0"[0-7]*|"0"[xX][0-9A-Fa-f]+|[1-9][0-9]*
CHAR		"'"([^']?|\\{ESCAPE})"'"
FLOAT		[0-9]*"."[0-9]+([eE][-+]?[0-9]+)?[f]?
STRING		\"([^\\"]|\\{ESCAPE})*\"


%%

int 	return TK_INT;
char 	return TK_CHAR;
float 	return TK_FLOAT;
void 	return TK_VOID;
new 	return TK_NEW;

if 	return TK_IF;
else 	return TK_ELSE;
while 	return TK_WHILE;
return 	return TK_RET;

"+"	return TK_PLUS;
"-" 	return TK_MINUS;
"*"	return TK_AST;
"/"	return TK_SLASH;
"=="	return TK_EQUAL;
"<="	return TK_LEQUAL;
">="	return TK_GEQUAL;
"<"	return TK_LESS;
">"	return TK_GREATER;
"!"	return TK_NOT;
"&&"	return TK_AND;
"||"	return TK_OR;


{INTEGER} {
	yylval.i = atoi(yytext);
	return TK_INT_I;
}

{CHAR} {
	convert_escaped_char(yytext);

	yylval.c = yytext[1];
	return TK_CHAR_I;
}

{FLOAT} {
	yylval.f = atof(yytext);
	return TK_FLOAT_I;
}

{STRING} {
	char *s = remove_wrap_char(yytext);
	convert_escaped_char(s);

	yylval.s = strcpy_malloc(s);
	return TK_STR;
}

{IDENTIFIER} {
	yylval.s = strcpy_malloc(yytext);
	return TK_ID;
}

{COMMENT} {
#if MAINLEX
	yylval.s = strcpy_malloc(yytext);
	return TK_CMT;
#endif
}


[ \t\n]+	/* Eat up white space. */

. return yytext[0];



%%

#if MAINLEX

int main(int argc, char *argv[])
{
	int tk;

	++argv, --argc;
	if (argc > 0)
		yyin = fopen(argv[0], "r");
	else
		yyin = stdin;


	while( tk = yylex() )
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

			case TK_CMT:
				printf("TK_CMT (%s)\n", yylval.s);
				break;

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
}

#endif

