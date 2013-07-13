
#ifndef TREE_H
#define TREE_H

/* --------------------------------------------------------------------------------- */

typedef enum 	DeclarationType	DeclarationType;
typedef enum 	DecVarType	DecVarType;
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

ListNode* newListNodeFirst(void *data, ListNode *next);
ListNode* newListNodeLast(void *data, ListNode *next);
ListNode *catListNode(ListNode *a, ListNode *b);

ProgramNode* newProgramNode(ListNode *dec);

DeclarationNode* newDeclarationNodeTypeDecVar(ListNode* var);
DeclarationNode* newDeclarationNodeTypeDecFunc(DecFuncNode* func);

ListNode* newDecVarNodeList(TypeNode *type, ListNode *name);
DecVarNode* newDecVarNode(TypeNode *type, char *name);

TypeNode* newTypeNodeTypePrim(BaseType prim);
TypeNode* newTypeNodeTypeArray(TypeNode *array);

DecFuncNode* newDecFuncNode(TypeNode *type, char *id, ListNode *params, BlockNode *block);

BlockNode* newBlockNode(ListNode *var, ListNode *cmd );

CmdNode* newCmdNodeCmdIf(ExpNode *cond, CmdNode *cmd_if, CmdNode *cmd_else);
CmdNode* newCmdNodeCmdWhile(ExpNode *cond, CmdNode *cmd);
CmdNode* newCmdNodeCmdAssig(VarNode *var, ExpNode *exp);
CmdNode* newCmdNodeCmdRet(ExpNode *exp);
CmdNode* newCmdNodeCmdCall(CallNode *call);
CmdNode* newCmdNodeCmdBlock(BlockNode *block);

VarNode* newVarNodeVarId(char *id);
VarNode* newVarNodeVarArray(VarNode *array, ExpNode* exp);

ExpNode* newExpNodeExpConstInt(int i);
ExpNode* newExpNodeExpConstFloat(float f);
ExpNode* newExpNodeExpConstChar(char c);
ExpNode* newExpNodeExpConstStr(char *s);
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

enum DecVarType
{
	DecVarGlobal,
	DecVarParam,
	DecVarLocal,
};

struct DecVarNode
{
	DecVarType context;

	TypeNode *type;
	char *name;

	int nref;
	int stack_offset;
};

/* --------------------------------------------------------------------------------- */

enum BaseType
{
	TypeVoid,
	TypeChar,
	TypeInt,
	TypeFloat,
};

struct TypeNode
{
	BaseType prim;
	int dims;
};

/* --------------------------------------------------------------------------------- */

struct DecFuncNode
{
	TypeNode *type;
	char *id;
	ListNode *params;
	BlockNode *block;

	int nref;
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
	TypeNode *atype;

	union
	{
		struct
		{
			char *id;
			DecVarNode* dec;
		} b;
		
		struct
		{
			ExpNode *exp;
			VarNode *var;
		} d;
	} u;
};

/* --------------------------------------------------------------------------------- */

enum ExpType
{
	ExpVar,
	ExpConst,
	ExpBin,
	ExpUn,
	ExpCall,
	ExpNew,
	ExpCast,
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
	TypeNode *atype;

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

		struct
		{
			ExpNode *exp;
		} cast;
	} u;
};

/* --------------------------------------------------------------------------------- */

struct CallNode
{
	char *id;
	ListNode *exp;

	DecFuncNode *dec;
};

/* --------------------------------------------------------------------------------- */

#endif
