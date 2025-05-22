#ifndef _h_gscript_defs
#define _h_gscript_defs

#include <unordered_map>
#include <list>
#include <string>
#include <cassert>

#define NOARG

namespace gscript
{
	class ScriptValue;
	class FunctionParameter;
	class ParserCallArglist;
	class ScriptType;
	class ScriptArrayType;
	class ScriptArrayValue;

	enum class MODIFIER_T
	{
		M_NONE = 0,
		M_ACCESS_PUBLIC = 1,
		M_ACCESS_PROTECTED = 2,
		M_ACCESS_PRIVATE = 4,
		M_CONST = 8,
		M_STATIC = 16,
		M_VIRTUAL = 32,
		M_ABSTRACT = 64
	};

	enum class CLASS_MODIFIER_T
	{
		CM_NONE = 0,
		CM_ABSTRACT = 1
	};

	enum class VALUE_TYPE_T
	{
		VT_BOOL,
		VT_BYTE,
		VT_CHAR,
		VT_INT,
		VT_UNSIGNED_INT,
		VT_FLOAT,
		VT_DOUBLE,
		VT_STRING,
		VT_CLASS,
		VT_ARRAY,
		VT_REFERENCE,
		VT_NULL,
		VT_VOID
	};

	enum class ENTITY_TYPE_T
	{
		ET_NONE = 0,
		ET_ANY = 1,
		ET_FUNC = 2,
		ET_VAR = 4,
		ET_CLASS = 8,
		ET_NAMESPACE = 16,
		ET_STATEMENT = 32,
		ET_VARDECL = 64
	};

	enum class OPERATOR_TYPE_T
	{
		OT_MEMBER_ACCESSOR = 1,
		OT_ADD = 2,
		OT_ADD_TO = 3,
		OT_SUBTRACT = 4,
		OT_SUBTRACT_FROM = 5,
		OT_MULTIPLY = 6,
		OT_MULTIPLY_BY = 7,
		OT_DIVIDE = 8,
		OT_DIVIDE_BY = 9,
		OT_EQUALS = 10,
		OT_NOT_EQUALS = 11,
		OT_GREATER_THAN = 12,
		OT_GREATER_THAN_OR_EQUAL = 13,
		OT_LESSER_THAN = 14,
		OT_LESSER_THAN_OR_EQUAL = 15,
		OT_ASSIGN = 16,
		OT_NEGATE = 17,
		OT_INCREMENT = 18,
		OT_PRE_INCREMENT = 19,
		OT_POST_INCREMENT = 20,
		OT_DECREMENT = 21,
		OT_PRE_DECREMENT = 22,
		OT_POST_DECREMENT = 23,
		OT_CONDITIONAL_IF = 24,
		OT_CONDITIONAL_ELSE = 25,
		OT_CONDITIONAL_NULL = 26
	};

	enum class OPERATOR_POSITION_T
	{
		OP_IRRELEVANT = 0,
		OP_LEFT = 1,
		OP_RIGHT = 2
	};

	enum class OPERATOR_LINK_T
	{
		OL_SINGLE = 0,
		OL_LEFT = 1,
		OL_RIGHT = 2,
		OL_BOTH = OL_LEFT | OL_RIGHT
	};

	enum class NAMESPACE_TYPE_T
	{
		NT_MAIN = 1,
		NT_NAMED = 2,
		NT_ENCLOSED = 4,
		NT_REGULAR = NT_NAMED | NT_ENCLOSED
	};

	typedef std::vector<FunctionParameter> PARAMS_T;

	typedef unsigned int BITFLAG_T;

	typedef std::vector<ScriptValue*> CALLABLE_PARAMS_T;

	extern const char *KW_NULL;
	extern const char *KW_TRUE;
	extern const char *KW_FALSE;
}

#endif