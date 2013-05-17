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
	DecVarNode 	*dec_variavel;
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
	: declaracoes { $$ = newProgramNode($1); tree = $$; }
	;

declaracoes
	: declaracao { $$ = newListNode($1, NULL); }
	| declaracoes declaracao { $$ = newListNode($2, $1); }
	;

declaracao 
	: dec_variavel { $$ = newDeclarationNode(DecVar, $1); }
	| dec_funcao { $$ = newDeclarationNode(DecFunc, $1); }
	;

dec_variavel 
	: tipo lista_nomes ';' { $$ = newDecVarNode($1, $2); }
	;

lista_nomes 
	: TK_ID { $$ = newListNode($1, NULL); }
	| lista_nomes ',' TK_ID { $$ = newListNode($3, $1); }
	;

tipo 
	: tipo_base { $$ = newTypeNode(TypeBase, $1); }
	| tipo '[' ']' { $$ = newTypeNode(TypeRec, $1);; }
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
	: dec_variavel { $$ = newListNode($1, NULL); }
	| dec_variaveis dec_variavel { $$ = newListNode($2, $1); }
	;

comandos
	: comando { $$ = newListNode($1, NULL); }
	| comandos comando { $$ = newListNode($2, $1); }
	;

comando 
	: TK_IF '(' exp ')' comando %prec TK_THEN { $$ = newCmdNode(CmdIf, $3, $5, NULL); }
	| TK_IF '(' exp ')' comando TK_ELSE comando { $$ = newCmdNode(CmdIf, $3, $5, $7); }
        | TK_WHILE '(' exp ')' comando { $$ = newCmdNode(CmdWhile, $3, $5); }
        | var '=' exp ';' { $$ = newCmdNode(CmdAssig, $1, $3); }
        | TK_RET exp ';' { $$ = newCmdNode(CmdRet, $2); }
        | TK_RET ';' { $$ = newCmdNode(CmdRet, NULL); }
        | chamada ';' { $$ = newCmdNode(CmdCall, $1); }
	| bloco { $$ = newCmdNode(CmdBlock, $1); }
	;

var 
	: TK_ID { $$ = newVarNode(VarId, $1); } 
	| var '[' exp ']' { $$ = newVarNode(VarRec, $1, $3); }
	;

exp 
	: TK_INT_I { $$ = newExpNode(ExpValue, PrimInt, $1); } 
	| TK_FLOAT_I { $$ = newExpNode(ExpValue, PrimFloat, (double)$1); } 
	| TK_CHAR_I { $$ = newExpNode(ExpValue, PrimChar, (int)$1); } 
	| TK_STR { $$ = newExpNode(ExpValue, PrimStr, $1); } 
	| var { $$ = newExpNode(ExpVar, $1); }
	| '(' exp ')' { $$ = $2; }
	| chamada { $$ = newExpNode(ExpCall, $1); }
	| TK_NEW tipo '[' exp ']' { $$ = newExpNode(ExpNew, $2, $4); }
	| TK_MINUS exp %prec UNARY_MINUS { $$ = newExpNode(ExpUn, ExpUnMinus, $2); }
	| exp TK_PLUS exp { $$ = newExpNode(ExpBin, ExpBinPlus, $1, $3); }
	| exp TK_MINUS exp { $$ = newExpNode(ExpBin, ExpBinMinus, $1, $3); }
	| exp TK_AST exp { $$ = newExpNode(ExpBin, ExpBinMult, $1, $3); }
	| exp TK_SLASH exp { $$ = newExpNode(ExpBin, ExpBinDiv, $1, $3); }
	| exp TK_EQUAL exp { $$ = newExpNode(ExpBin, ExpBinEQ, $1, $3); }
	| exp TK_LEQUAL exp { $$ = newExpNode(ExpBin, ExpBinLE, $1, $3); }
	| exp TK_GEQUAL exp { $$ = newExpNode(ExpBin, ExpBinGE, $1, $3); }
	| exp TK_LESS exp { $$ = newExpNode(ExpBin, ExpBinLT, $1, $3); }
	| exp TK_GREATER exp { $$ = newExpNode(ExpBin, ExpBinGT, $1, $3); }
	| TK_NOT exp %prec UNARY_NOT { $$ = newExpNode(ExpUn, ExpUnNot, $2); }
	| exp TK_AND exp { $$ = newExpNode(ExpBin, ExpBinAnd, $1, $3); }
	| exp TK_OR exp { $$ = newExpNode(ExpBin, ExpBinOr, $1, $3); }
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

