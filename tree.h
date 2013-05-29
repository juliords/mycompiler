
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
ListNode *catListNode(ListNode *a, ListNode *b);

ProgramNode* newProgramNode(ListNode *dec);

DeclarationNode* newDeclarationNodeTypeDecVar(ListNode* var);
DeclarationNode* newDeclarationNodeTypeDecFunc(DecFuncNode* func);

ListNode* newDecVarNode(TypeNode *type, ListNode *name);

TypeNode* newTypeNodeTypePrim(BaseType prim);
TypeNode* newTypeNodeTypeArray(TypeNode *array);

DecFuncNode* newDecFuncNode(TypeNode *type, char *id, ListNode *params, BlockNode *block);

ParamNode* newParamNode(TypeNode *type, char *id );

BlockNode* newBlockNode(ListNode *var, ListNode *cmd );

CmdNode* newCmdNodeCmdIf(ExpNode *cond, CmdNode *cmd_if, CmdNode *cmd_else);
CmdNode* newCmdNodeCmdWhile(ExpNode *cond, CmdNode *cmd);
CmdNode* newCmdNodeCmdAssig(VarNode *var, ExpNode *exp);
CmdNode* newCmdNodeCmdRet(ExpNode *exp);
CmdNode* newCmdNodeCmdCall(CallNode *call);
CmdNode* newCmdNodeCmdBlock(BlockNode *block);

VarNode* newVarNodeVarId(char *id);
VarNode* newVarNodeVarArray(VarNode *array, ExpNode* exp);

ExpNode* newExpNodeExpValueInt(int i);
ExpNode* newExpNodeExpValueFloat(float f);
ExpNode* newExpNodeExpValueChar(char c);
ExpNode* newExpNodeExpValueStr(char *s);
ExpNode* newExpNodeExpVar(VarNode *var);
ExpNode* newExpNodeExpBin(ExpBinType type, ExpNode *left, ExpNode *right);
ExpNode* newExpNodeExpUn(ExpUnType type, ExpNode *exp);
ExpNode* newExpNodeExpCall(CallNode *call);
ExpNode* newExpNodeExpNew(TypeNode *type, ExpNode *exp);

CallNode* newCallNode(char *id, ListNode *exp );


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
		ListNode *var;
		DecFuncNode *func;
	} u;
};

/* --------------------------------------------------------------------------------- */

struct DecVarNode
{
	TypeNode *type;
	char *name;
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
	TypePrim,
	TypeArray,
};

struct TypeNode
{
	TypeType type;
	union
	{
		BaseType prim;
		TypeNode *array;
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
	VarArray,
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
			VarNode *array;
		} v;
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
