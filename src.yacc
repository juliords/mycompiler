%{
#include <stdio.h>
#include "tree.h"

int yydebug = 0;

void yyerror(char *);
int yylex();

ProgramNode *tree;
%}

%start programa

%token TK_INT TK_CHAR TK_FLOAT
%token TK_INT_I TK_CHAR_I TK_FLOAT_I
%token TK_VOID TK_STR TK_NEW

%token TK_ID
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
	TypeNode 	*tipo;
	BaseType 	tipo_base;
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

%type <i>		TK_INT_I
%type <c>		TK_CHAR_I
%type <f>		TK_FLOAT_I
%type <s>		TK_STR
%type <s>		TK_ID

%type <programa>	programa
%type <lista>		declaracoes
%type <declaracao>	declaracao
%type <lista>		dec_variavel
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
	: declaracoes { $$ = newProgramNode($1); tree = $$; }
	;

declaracoes
	: declaracao { $$ = newListNode($1, NULL); }
	| declaracoes declaracao { $$ = newListNode($2, $1); }
	;

declaracao 
	: dec_variavel { $$ = newDeclarationNodeTypeDecVar($1); }
	| dec_funcao { $$ = newDeclarationNodeTypeDecFunc($1); }
	;

dec_variavel 
	: tipo lista_nomes ';' { $$ = newDecVarNode($1, $2); }
	;

lista_nomes 
	: TK_ID { $$ = newListNode($1, NULL); }
	| lista_nomes ',' TK_ID { $$ = newListNode($3, $1); }
	;

tipo 
	: tipo_base { $$ = newTypeNodeTypePrim($1); }
	| tipo '[' ']' { $$ = newTypeNodeTypeArray($1); }
	;

tipo_base 
	: TK_INT { $$ = TypeInt; } 
	| TK_CHAR { $$ = TypeChar; } 
	| TK_FLOAT { $$ = TypeFloat; }
	| TK_VOID { $$ = TypeVoid; }
	;

dec_funcao 
	: tipo TK_ID '(' parametros ')' bloco { $$ = newDecFuncNode($1, $2, $4, $6); }
	;

parametros 
	: /* vazio */ { $$ = NULL; } 
	| parametro { $$ = newListNode($1, NULL); } 
	| parametros ',' parametro { $$ = newListNode($3, $1); }
	;

parametro 
	: tipo TK_ID { $$ = newParamNode($1, $2); }
	;

bloco 
	: '{' dec_variaveis comandos '}' { $$ = newBlockNode($2, $3); }
	| '{' comandos '}' { $$ = newBlockNode(NULL, $2); }
	| '{' /* vazio */ '}' { $$ = NULL; }
	;

dec_variaveis
	: dec_variavel { $$ = $1; }
	| dec_variaveis dec_variavel { $$ = catListNode($1, $2); }
	;

comandos
	: comando { $$ = newListNode($1, NULL); }
	| comandos comando { $$ = newListNode($2, $1); }
	;

comando 
	: TK_IF '(' exp ')' comando %prec TK_THEN { $$ = newCmdNodeCmdIf($3, $5, NULL); }
	| TK_IF '(' exp ')' comando TK_ELSE comando { $$ = newCmdNodeCmdIf($3, $5, $7); }
        | TK_WHILE '(' exp ')' comando { $$ = newCmdNodeCmdWhile($3, $5); }
        | var '=' exp ';' { $$ = newCmdNodeCmdAssig($1, $3); }
        | TK_RET exp ';' { $$ = newCmdNodeCmdRet($2); }
        | TK_RET ';' { $$ = newCmdNodeCmdRet(NULL); }
        | chamada ';' { $$ = newCmdNodeCmdCall($1); }
	| bloco { $$ = newCmdNodeCmdBlock($1); }
	;

var 
	: TK_ID { $$ = newVarNodeVarId($1); } 
	| var '[' exp ']' { $$ = newVarNodeVarArray($1, $3); }
	;

exp 
	: TK_INT_I { $$ = newExpNodeExpValueInt($1); } 
	| TK_FLOAT_I { $$ = newExpNodeExpValueFloat($1); } 
	| TK_CHAR_I { $$ = newExpNodeExpValueChar($1); } 
	| TK_STR { $$ = newExpNodeExpValueStr($1); } 
	| var { $$ = newExpNodeExpVar($1); }
	| '(' exp ')' { $$ = $2; }
	| chamada { $$ = newExpNodeExpCall($1); }
	| TK_NEW tipo '[' exp ']' { $$ = newExpNodeExpNew($2, $4); }
	| TK_MINUS exp %prec UNARY_MINUS { $$ = newExpNodeExpUn(ExpUnMinus, $2); }
	| exp TK_PLUS exp { $$ = newExpNodeExpBin(ExpBinPlus, $1, $3); }
	| exp TK_MINUS exp { $$ = newExpNodeExpBin(ExpBinMinus, $1, $3); }
	| exp TK_AST exp { $$ = newExpNodeExpBin(ExpBinMult, $1, $3); }
	| exp TK_SLASH exp { $$ = newExpNodeExpBin(ExpBinDiv, $1, $3); }
	| exp TK_EQUAL exp { $$ = newExpNodeExpBin(ExpBinEQ, $1, $3); }
	| exp TK_LEQUAL exp { $$ = newExpNodeExpBin(ExpBinLE, $1, $3); }
	| exp TK_GEQUAL exp { $$ = newExpNodeExpBin(ExpBinGE, $1, $3); }
	| exp TK_LESS exp { $$ = newExpNodeExpBin(ExpBinLT, $1, $3); }
	| exp TK_GREATER exp { $$ = newExpNodeExpBin(ExpBinGT, $1, $3); }
	| TK_NOT exp %prec UNARY_NOT { $$ = newExpNodeExpUn(ExpUnNot, $2); }
	| exp TK_AND exp { $$ = newExpNodeExpBin(ExpBinAnd, $1, $3); }
	| exp TK_OR exp { $$ = newExpNodeExpBin(ExpBinOr, $1, $3); }
	;

chamada 
	: TK_ID '(' lista_exp ')' { $$ = newCallNode($1, $3); }
	;

lista_exp 
	: /* vazio */ { $$ = NULL; } 
	| exp { $$ = newListNode($1, NULL); }
	| lista_exp ',' exp { $$ = newListNode($3, $1); }
	;


%%

