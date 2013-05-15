
#ifndef TREE_H
#define TREE_H

/* --------------------------------------------------------------------------------- */

typedef enum 	DeclarationType	DeclarationType;
typedef enum 	BaseType	BaseType;
typedef enum 	TypeType	TypeType;
typedef enum 	CmdType 	CmdType;
typedef enum 	VarType 	VarType;
typedef enum 	ExpType 	ExpType;
typedef enum 	ExpBinType 	ExpBinType;
typedef enum 	ExpUnType 	ExpUnType;
typedef enum 	PrimitiveType 	PrimitiveType;

typedef struct 	ListNode 	ListNode;
typedef struct 	ProgramNode 	ProgramNode;
typedef struct 	DeclarationNode	DeclarationNode;
typedef struct 	DecVarNode	DecVarNode;
typedef struct 	TypeNode	TypeNode;
typedef struct 	DecFuncNode	DecFuncNode;
typedef struct 	ParamNode	ParamNode;
typedef struct 	BlockNode	BlockNode;
typedef struct 	VarNode 	VarNode;
typedef struct 	ExpNode 	ExpNode;
typedef struct 	CallNode 	CallNode;
typedef struct 	CmdNode 	CmdNode;

/* --------------------------------------------------------------------------------- */

ListNode* newListNode(void *data, ListNode *next);

ProgramNode* newProgramNode(ListNode *dec);

DeclarationNode* newDeclarationNode(DeclarationType type, ... );

DecVarNode* newDecVarNode(TypeNode *type, ListNode *name);

TypeNode* newTypeNode(TypeType type, ... );

DecFuncNode* newDecFuncNode(TypeNode *type, char *id, ListNode *params, BlockNode *block);

ParamNode* newParamNode(TypeNode *type, char *id );

BlockNode* newBlockNode(ListNode *var, ListNode *cmd );

CmdNode* newCmdNode(CmdType type, ... );

VarNode* newVarNode(VarType type, ... );

ExpNode* newExpNode(ExpType type, ... );

CallNode* newCallNode(char *id, ListNode *exp );


/* --------------------------------------------------------------------------------- */

void freeProgramNode(ProgramNode* p);
void freeDeclarationNode(DeclarationNode* p);
void freeDecVarNode(DecVarNode* p);
void freeTypeNode(TypeNode* p);
void freeDecFuncNode(DecFuncNode* p);
void freeParamNode(ParamNode* p);
void freeBlockNode(BlockNode* p);
void freeTypeNode(TypeNode* p);
void freeCmdNode(CmdNode* p);
void freeVarNode(VarNode* p);
void freeExpNode(ExpNode* p);
void freeCallNode(CallNode* p);


/* --------------------------------------------------------------------------------- */

struct ListNode
{
	void *data;
	ListNode *next;
	ListNode *last;
};

/* --------------------------------------------------------------------------------- */

struct ProgramNode
{
	ListNode *dec;
};

/* --------------------------------------------------------------------------------- */

enum DeclarationType
{
	DecVar,
	DecFunc,
};

struct DeclarationNode
{
	DeclarationType type;
	union
	{
		DecVarNode *var;
		DecFuncNode *func;
	} u;
};

/* --------------------------------------------------------------------------------- */

/**
 * Otimizacao:
 * - Ao inves de criar uma lista de nomes, pode-se criar logo uma lista
 *   de variaveis. De modo que estas as representem, de fato.
 */

struct DecVarNode
{
	TypeNode *type;
	ListNode *name;
};

/* --------------------------------------------------------------------------------- */

enum BaseType
{
	TypeInt,
	TypeChar,
	TypeFloat,
	TypeVoid,
};

enum TypeType
{
	TypeBase,
	TypeRec,
};

struct TypeNode
{
	TypeType type;
	union
	{
		BaseType base;
		TypeNode *next;
	} u;
};

/* --------------------------------------------------------------------------------- */

struct DecFuncNode
{
	TypeNode *type;
	char *id;
	ListNode *params;
	BlockNode *block;
};

/* --------------------------------------------------------------------------------- */

struct ParamNode
{
	TypeNode *type;
	char *id;
};

/* --------------------------------------------------------------------------------- */

struct BlockNode
{
	ListNode *var;
	ListNode *cmd;
};

/* --------------------------------------------------------------------------------- */

enum CmdType
{
	CmdIf,
	CmdWhile,
	CmdAssig,
	CmdRet,
	CmdCall,
	CmdBlock,
};

struct CmdNode
{
	CmdType type;
	union
	{
		struct
		{
			ExpNode *cond;
			CmdNode *cmd_if;
			CmdNode *cmd_else;
		} i;

		struct
		{
			ExpNode *cond;
			CmdNode *cmd;
		} w;

		struct 
		{
			VarNode *var;
			ExpNode *exp;
		} a;

		struct
		{
			ExpNode *exp;
		} r;

		struct
		{
			BlockNode *block;
		} b;

		struct
		{
			CallNode *call;
		} c;
	} u;
};

/* --------------------------------------------------------------------------------- */

enum VarType
{
	VarId,
	VarRec,
};

struct VarNode
{
	VarType type;
	union
	{
		char *id;
		
		struct
		{
			ExpNode *exp;
			VarNode *next;
		} rec;
	} u;
};

/* --------------------------------------------------------------------------------- */

enum ExpType
{
	ExpVar,
	ExpValue,
	ExpBin,
	ExpUn,
	ExpCall,
	ExpNew,
};

enum PrimitiveType
{
	PrimFloat,
	PrimInt,
	PrimChar,
	PrimStr,
};

enum ExpBinType
{
	ExpBinPlus,
	ExpBinMinus,
	ExpBinMult,
	ExpBinDiv,
	ExpBinEQ,
	ExpBinLE,
	ExpBinGE,
	ExpBinLT,
	ExpBinGT,
	ExpBinAnd,
	ExpBinOr,
};

enum ExpUnType
{
	ExpUnMinus,
	ExpUnNot,
};

struct ExpNode
{
	ExpType type;
	union
	{
		struct
		{
			VarNode *var;
		} var;

		struct 
		{
			PrimitiveType type;
			union
			{
				int i;
				float f;
				char c;
				char *s;
			} u;
		} prim;

		struct 
		{
			ExpBinType type;
			ExpNode *left;
			ExpNode *right;
		} bin;

		struct 
		{
			ExpUnType type;
			ExpNode *exp;
		} un;

		struct
		{
			CallNode *call;
		} call;

		struct
		{
			TypeNode *type;
			ExpNode *exp;
		} enew;
	} u;
};

/* --------------------------------------------------------------------------------- */

struct CallNode
{
	char *id;
	ListNode *exp;
};

/* --------------------------------------------------------------------------------- */

#endif
