#ifndef _h_gscript_defs
#define _h_gscript_defs

#include "bitfield.hpp"

#include <vector>
#include <limits>

#define NOARG

namespace gscript
{
	class ScriptValue;
	class FunctionParameter;
	class ParserCallArglist;
	class ScriptType;
	class ScriptArrayType;
	class ScriptArrayValue;
	class ScriptNullValue;
	class ScriptBoolValue;
	class ScriptScopeBase;

	static constexpr size_t NullAddr = std::numeric_limits<size_t>::max();

	struct ScopedAddress
	{
		ScopedAddress(ScriptScopeBase* scope = nullptr, size_t addr = NullAddr)
			: scope(scope)
			, addr(addr)
		{
		}

		ScriptScopeBase* scope;
		size_t addr;

		operator bool() const
		{
			return scope && addr != NullAddr;
		}
	};

	enum class MODIFIER_T
	{
		M_NONE = 0x00,
		M_ACCESS_PUBLIC = 0x01,
		M_ACCESS_PROTECTED = 0x02,
		M_ACCESS_PRIVATE = 0x04,
		M_CONST = 0x08,
		M_STATIC = 0x10,
		M_VIRTUAL = 0x20,
		M_ABSTRACT = 0x40
	};

	enum class CLASS_MODIFIER_T
	{
		CM_NONE = 0x00,
		CM_ABSTRACT = 0x01
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
		ET_NONE = 0x00,
		ET_ANY = 0x01,
		ET_FUNC = 0x02,
		ET_VAR = 0x04,
		ET_CLASS = 0x08,
		ET_NAMESPACE = 0x10,
		ET_STATEMENT = 0x20,
		ET_VARDECL = 0x40
	};

	enum class OPERATOR_TYPE_T
	{
		OT_INVALID = 0,
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
		OL_SINGLE = 0x00,
		OL_LEFT = 0x01,
		OL_RIGHT = 0x02,
		OL_BOTH = OL_LEFT | OL_RIGHT
	};

	enum class NAMESPACE_TYPE_T
	{
		NT_MAIN = 0x01,
		NT_NAMED = 0x02,
		NT_ENCLOSED = 0x04,
		NT_REGULAR = NT_NAMED | NT_ENCLOSED
	};

	typedef std::vector<FunctionParameter> PARAMS_T;
	typedef std::vector<ScriptValue*> CALLABLE_PARAMS_T;

	extern const char *KW_NULL;
	extern const char *KW_TRUE;
	extern const char *KW_FALSE;

	extern ScriptNullValue* SCR_NULL;
	extern ScriptBoolValue* SCR_TRUE;
	extern ScriptBoolValue* SCR_FALSE;
}

DEFINE_BITFIELD(gscript::MODIFIER_T);
DEFINE_BITFIELD(gscript::CLASS_MODIFIER_T);
DEFINE_BITFIELD(gscript::ENTITY_TYPE_T);
DEFINE_BITFIELD(gscript::OPERATOR_POSITION_T);
DEFINE_BITFIELD(gscript::OPERATOR_LINK_T);
DEFINE_BITFIELD(gscript::NAMESPACE_TYPE_T);

#endif