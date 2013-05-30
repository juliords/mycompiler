%{

#include "tree.h"
#include "yacc.h"
#include "macro.h"

int fileno(FILE*);

char* strcpy_malloc(char* str)
{
	NEWN(char, s, strlen(str)+1);

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


IDENTIFIER	[_a-zA-Z][a-zA-Z0-9]*
COMMENT		"/*"([^*]|"\n"|"*"[^/])*"*"*"*/"
INTEGER		"0"[0-7]*|"0"[xX][0-9A-Fa-f]+|[1-9][0-9]*
CHAR		"'"([^']?|\\.)"'"
FLOAT		[0-9]+"."[0-9]+([eE][-+]?[0-9]+)?[f]?
STRING		\"([^\\"]|\\.)*\"


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
	yylval.i = strtol(yytext, NULL, 0);
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
/*
	yylval.s = strcpy_malloc(yytext);
	return TK_CMT;
*/
}


[ \t\n]+	/* Eat up white space. */

. return yytext[0];



%%

