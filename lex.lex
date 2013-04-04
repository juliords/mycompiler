
%{
/* Need this for the call to atof() below. */
#include <math.h>
/* Need this for printf(), fopen(), and stdin below. */
#include <stdio.h>

#include <limits.h>

enum Tokens
{
	TK_INT = UCHAR_MAX+1,
	TK_INT_I,
	TK_CHAR,
	TK_CHAR_I,
	TK_FLOAT,
	TK_FLOAT_I,
	TK_VOID,
	TK_STR,
	TK_NEW,

	TK_ID,
	TK_IF,
	TK_ELSE,
	TK_WHILE,
	TK_CMT,
	TK_RET,

	TK_OP
};

union 
{
	int i;
	float f;
	char c;
	char *s;
} Info;

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
OPERATOR	"+"|"-"|"*"|"/"|"=="|"<="|">="|"<"|">"|"!"|"&&"|"||"


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

{INTEGER} {
	Info.i = atoi(yytext);
	return TK_INT_I;
}

{CHAR} {
	convert_escaped_char(yytext);

	Info.c = yytext[1];
	return TK_CHAR_I;
}

{FLOAT} {
	Info.f = atof(yytext);
	return TK_FLOAT_I;
}

{STRING} {
	char *s = remove_wrap_char(yytext);
	convert_escaped_char(s);

	Info.s = s;
	return TK_STR;
}

{IDENTIFIER} {
	Info.s = yytext;
	return TK_ID;
}

{COMMENT} {
	Info.s = yytext;
	return TK_CMT;
}

{OPERATOR} {
	Info.s = yytext;
	return TK_OP;
}

[ \t\n]+	/* Eat up white space. */

. return yytext[0];



%%



int main(int argc, char *argv[])
{
	int tk;

	++argv, --argc;	/* Skip over program name. */
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
				printf("TK_INT_I (%d)\n", Info.i); 
				break;

			case TK_CHAR: 
				printf("TK_CHAR\n"); 
				break;

			case TK_CHAR_I: 
				printf("TK_CHAR_I (%c)\n", Info.c); 
				break;

			case TK_FLOAT: 
				printf("TK_FLOAT\n"); 
				break;

			case TK_FLOAT_I: 
				printf("TK_FLOAT_I (%f)\n", Info.f); 

			case TK_VOID:
				printf("TK_VOID\n");
				break;

			case TK_STR:
				printf("TK_STR (%s)\n", Info.s);
				break;

			case TK_NEW:
				printf("TK_NEW\n");
				break;

			case TK_OP:
				printf("TK_OP (%s)\n", Info.s);
				break;

			case TK_ID:
				printf("TK_ID (%s)\n", Info.s);
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
				printf("TK_CMT (%s)\n", Info.s);
				break;

			case TK_RET:
				printf("TK_RET\n");
				break;

			default:
				printf("OTHER: %c\n", tk);
				break;
		}
	}
}

