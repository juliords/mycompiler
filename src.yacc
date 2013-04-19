%{
#include<stdio.h>

int yydebug = 1;
%}

%start programa

%token TK_INT TK_CHAR TK_FLOAT
%token TK_INT_I TK_CHAR_I TK_FLOAT_I
%token TK_VOID TK_STR TK_NEW

%token TK_ID TK_CMT
%token TK_IF TK_ELSE TK_WHILE
%token TK_RET

%token TK_PLUS TK_MINUS TK_AST TK_SLASH
%token TK_EQUAL TK_LEQUAL TK_GEQUAL TK_LESS TK_GREATER
%token TK_NOT TK_AND TK_OR

%left TK_EQUAL TK_LEQUAL TK_GEQUAL TK_LESS TK_GREATER
%left TK_NOT TK_AND TK_OR
%left TK_PLUS TK_MINUS
%left TK_AST TK_SLASH

%nonassoc UNARY_MINUS
%nonassoc UNARY_NOT

%nonassoc TK_THEN
%nonassoc TK_ELSE

%%

programa 
	: declaracoes
	;

declaracoes
	: declaracao
	| declaracoes declaracao
	;

declaracao 
	: dec_variavel 
	| dec_funcao
	;

dec_variavel 
	: tipo lista_nomes ';'
	;

lista_nomes 
	: TK_ID 
	| lista_nomes ',' TK_ID
	;

tipo 
	: tipo_base 
	| tipo '[' ']'
	;

tipo_base 
	: TK_INT 
	| TK_CHAR 
	| TK_FLOAT
	| TK_VOID
	;

dec_funcao 
	: tipo TK_ID '(' parametros ')' bloco
	;

parametros 
	: /* vazio */ 
	| parametro 
	| parametros ',' parametro
	;

parametro 
	: tipo TK_ID
	;

bloco 
	: '{' dec_variaveis comandos '}'
	| '{' comandos '}'
	| '{' /* vazio */ '}'
	;

dec_variaveis
	: dec_variavel
	| dec_variaveis dec_variavel
	;

comandos
	: comando
	| comandos comando
	;

comando 
	: TK_IF '(' exp ')' comando %prec TK_THEN
	| TK_IF '(' exp ')' comando TK_ELSE comando
        | TK_WHILE '(' exp ')' comando
        | var '=' exp ';'
        | TK_RET exp ';'
        | TK_RET ';'
        | chamada ';'
	| bloco
	;

var 
	: TK_ID 
	| var '[' exp ']'
	;

exp 
	: TK_INT_I 
	| TK_FLOAT_I 
	| TK_CHAR_I 
	| TK_STR 
	| var
	| '(' exp ')'
	| chamada
	| TK_NEW tipo '[' exp ']'
	| TK_MINUS exp %prec UNARY_MINUS
	| exp TK_PLUS exp
	| exp TK_MINUS exp
	| exp TK_AST exp
	| exp TK_SLASH exp
	| exp TK_EQUAL exp
	| exp TK_LEQUAL exp
	| exp TK_GEQUAL exp
	| exp TK_LESS exp
	| exp TK_GREATER exp
	| TK_NOT exp %prec UNARY_NOT
	| exp TK_AND exp
	| exp TK_OR exp
	;

chamada 
	: TK_ID '(' lista_exp ')'
	;

lista_exp 
	: /* vazio */ 
	| exp
	| lista_exp ',' exp
	;


%%


int yyerror(char *s)
{
	printf("%s\n", s);
}

#if MAINYACC

int main()
{
	return yyparse();	
}

#endif
