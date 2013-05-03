
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
typedef struct 	TypeNode 	TypeNode;
typedef struct 	VarNode 	VarNode;
typedef struct 	ExpNode 	ExpNode;
typedef struct 	CallNode 	CallNode;
typedef struct 	CmdNode 	CmdNode;

struct ListNode
{
	void *data;
	ListNode *next;
};

/* --------------------------------------------------------------------------------- */

struct ProgramNode
{
	ListNode *programs;	
};

/* --------------------------------------------------------------------------------- */

enum DeclarationType
{
	DeclVar,
	DeclFunc,
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

struct DevVarNode
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
	TypeType *type;
	char *id;
	ListNode *params;
	BlockNode *block;
};

/* --------------------------------------------------------------------------------- */

struct ParamNode
{
	TypeType *type;
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
			ListNode *var;
			ListNode *cmd;
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
			VarNode *next;
			ExpNode *exp;
		} rec;
	} u;
};

/* --------------------------------------------------------------------------------- */

enum PrimitiveType
{
	PrimInt,
	PrimFloat,
	PrimChar,
	PrimStr,
};

enum ExpType
{
	ExpVar,
	ExpValue,
	ExpBin,
	ExpUn,
	ExpCall,
	ExpNew,
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

