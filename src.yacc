%{
#include <stdio.h>
#include "tree.h"

int yydebug = 1;

ProgramNode *root;
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

%union 
{
	int i;
	float f;
	char c;
	char *s;

	ListNode 	*lista;
	ProgramNode 	*programa;
	DeclarationNode	*declaracao;
	DecVarNode 	*dec_variavel;
	TypeNode 	*tipo;
	TypeNode 	*tipo_base;
	DecFuncNode 	*dec_funcao;
	ParamNode	*parametro;
	BlockNode	*bloco;
	CmdNode 	*comando;
	VarNode		*var;
	ExpNode		*exp;
	CallNode	*chamada;
};

%left TK_EQUAL TK_LEQUAL TK_GEQUAL TK_LESS TK_GREATER
%left TK_NOT TK_AND TK_OR
%left TK_PLUS TK_MINUS
%left TK_AST TK_SLASH

%nonassoc UNARY_MINUS
%nonassoc UNARY_NOT

%nonassoc TK_THEN
%nonassoc TK_ELSE

%type <programa>	programa
%type <lista>		declaracoes
%type <declaracao>	declaracao
%type <dec_variavel>	dec_variavel
%type <lista>		lista_nomes
%type <tipo>		tipo
%type <tipo_base>	tipo_base
%type <dec_funcao>	dec_funcao
%type <lista>		parametros
%type <parametro>	parametro
%type <bloco>		bloco
%type <lista>		dec_variaveis
%type <lista>		comandos
%type <comando>		comando
%type <var>		var
%type <exp>		exp
%type <chamada>		chamada
%type <lista>		lista_exp	

%%

programa 
	: declaracoes { $$ = NULL; root = $$; }
	;

declaracoes
	: declaracao { $$ = NULL; }
	| declaracoes declaracao { $$ = NULL; }
	;

declaracao 
	: dec_variavel { $$ = NULL; }
	| dec_funcao { $$ = NULL; }
	;

dec_variavel 
	: tipo lista_nomes ';' { $$ = NULL; }
	;

lista_nomes 
	: TK_ID { $$ = NULL; }
	| lista_nomes ',' TK_ID { $$ = NULL; }
	;

tipo 
	: tipo_base { $$ = NULL; }
	| tipo '[' ']' { $$ = NULL; }
	;

tipo_base 
	: TK_INT { $$ = NULL; } 
	| TK_CHAR { $$ = NULL; } 
	| TK_FLOAT { $$ = NULL; }
	| TK_VOID { $$ = NULL; }
	;

dec_funcao 
	: tipo TK_ID '(' parametros ')' bloco { $$ = NULL; }
	;

parametros 
	: /* vazio */ { $$ = NULL; } 
	| parametro { $$ = NULL; } 
	| parametros ',' parametro { $$ = NULL; }
	;

parametro 
	: tipo TK_ID { $$ = NULL; }
	;

bloco 
	: '{' dec_variaveis comandos '}' { $$ = NULL; }
	| '{' comandos '}' { $$ = NULL; }
	| '{' /* vazio */ '}' { $$ = NULL; }
	;

dec_variaveis
	: dec_variavel { $$ = NULL; }
	| dec_variaveis dec_variavel { $$ = NULL; }
	;

comandos
	: comando { $$ = NULL; }
	| comandos comando { $$ = NULL; }
	;

comando 
	: TK_IF '(' exp ')' comando %prec TK_THEN { $$ = NULL; }
	| TK_IF '(' exp ')' comando TK_ELSE comando { $$ = NULL; }
        | TK_WHILE '(' exp ')' comando { $$ = NULL; }
        | var '=' exp ';' { $$ = NULL; }
        | TK_RET exp ';' { $$ = NULL; }
        | TK_RET ';' { $$ = NULL; }
        | chamada ';' { $$ = NULL; }
	| bloco { $$ = NULL; }
	;

var 
	: TK_ID { $$ = NULL; } 
	| var '[' exp ']' { $$ = NULL; }
	;

exp 
	: TK_INT_I { $$ = NULL; } 
	| TK_FLOAT_I { $$ = NULL; } 
	| TK_CHAR_I { $$ = NULL; } 
	| TK_STR { $$ = NULL; } 
	| var { $$ = NULL; }
	| '(' exp ')' { $$ = NULL; }
	| chamada { $$ = NULL; }
	| TK_NEW tipo '[' exp ']' { $$ = NULL; }
	| TK_MINUS exp %prec UNARY_MINUS { $$ = NULL; }
	| exp TK_PLUS exp { $$ = NULL; }
	| exp TK_MINUS exp { $$ = NULL; }
	| exp TK_AST exp { $$ = NULL; }
	| exp TK_SLASH exp { $$ = NULL; }
	| exp TK_EQUAL exp { $$ = NULL; }
	| exp TK_LEQUAL exp { $$ = NULL; }
	| exp TK_GEQUAL exp { $$ = NULL; }
	| exp TK_LESS exp { $$ = NULL; }
	| exp TK_GREATER exp { $$ = NULL; }
	| TK_NOT exp %prec UNARY_NOT { $$ = NULL; }
	| exp TK_AND exp { $$ = NULL; }
	| exp TK_OR exp { $$ = NULL; }
	;

chamada 
	: TK_ID '(' lista_exp ')' { $$ = NULL; }
	;

lista_exp 
	: /* vazio */ { $$ = NULL; } 
	| exp { $$ = NULL; }
	| lista_exp ',' exp { $$ = NULL; }
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
