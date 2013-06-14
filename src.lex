%{

#include "tree.h"
#include "yacc.h"
#include "macro.h"

int fileno(FILE*);

int linenum;

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

int mygetc(FILE *s)
{
	int c = getc(s);

	if(c == '\n') 
	{
		linenum++;
	}

	return c;
}
#undef getc
#define getc(fp) mygetc(fp)

%}

%option always-interactive

IDENTIFIER	[_a-zA-Z][a-zA-Z0-9]*
COMMENT		"/*"([^*]|"\n"|"*"[^/])*"*"*"*/"
INTEGER		"0"[0-7]*|"0"[xX][0-9A-Fa-f]+|[1-9][0-9]*
CHAR		"'"([^']?|\\.)"'"
FLOAT		[0-9]+"."[0-9]+([eE][-+]?[0-9]+)?[fF]?
STRING		\"([^\\"]|\\.)*\"


%%

int 	{ yylval.i = linenum; return TK_INT;	}
char 	{ yylval.i = linenum; return TK_CHAR;	}
float 	{ yylval.i = linenum; return TK_FLOAT;	}
void 	{ yylval.i = linenum; return TK_VOID;	}
new 	{ yylval.i = linenum; return TK_NEW;	}

if 	{ yylval.i = linenum; return TK_IF;	}
else 	{ yylval.i = linenum; return TK_ELSE;	}
while 	{ yylval.i = linenum; return TK_WHILE;	}
return 	{ yylval.i = linenum; return TK_RET;	}

"+"	{ yylval.i = linenum; return TK_PLUS;	}
"-" 	{ yylval.i = linenum; return TK_MINUS;	}
"*"	{ yylval.i = linenum; return TK_AST;	}
"/"	{ yylval.i = linenum; return TK_SLASH;	}
"=="	{ yylval.i = linenum; return TK_EQUAL;	}
"<="	{ yylval.i = linenum; return TK_LEQUAL;	}
">="	{ yylval.i = linenum; return TK_GEQUAL;	}
"<"	{ yylval.i = linenum; return TK_LESS;	}
">"	{ yylval.i = linenum; return TK_GREATER;	}
"!"	{ yylval.i = linenum; return TK_NOT;	}
"&&"	{ yylval.i = linenum; return TK_AND;	}
"||"	{ yylval.i = linenum; return TK_OR;	}


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

. { yylval.i = linenum; return yytext[0]; }



%%

