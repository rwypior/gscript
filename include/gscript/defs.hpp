#ifndef _h_gscript_defs
#define _h_gscript_defs

#include "bitfield.hpp"

#include <vector>
#include <limits>
#include <memory>

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

	enum class Modifier
	{
		None = 0x00,
		AccessPublic = 0x01,
		AccessProtected = 0x02,
		AccessPrivate = 0x04,
		Const = 0x08,
		Static = 0x10,
		Virtual = 0x20,
		Abstract = 0x40
	};

	enum class ClassModifier
	{
		None = 0x00,
		Abstract = 0x01
	};

	enum class ValueType
	{
		Bool,
		Byte,
		Char,
		Int,
		UnsignedInt,
		Float,
		Double,
		String,
		Class,
		Array,
		Reference,
		Null,
		Void
	};

	enum class OperatorType
	{
		Invalid = 0,
		MemberAccessor = 1,
		Add = 2,
		AddTo = 3,
		Subtract = 4,
		SubtractFrom = 5,
		Multiply = 6,
		MultiplyBy = 7,
		Divide = 8,
		DivideBy = 9,
		Equals = 10,
		NotEquals = 11,
		GreaterThan = 12,
		GreaterThanOrEqual = 13,
		LesserThan = 14,
		LesserThanOrEqual = 15,
		Assign = 16,
		Negate = 17,
		Increment = 18,
		PreIncrement = 19,
		PostIncrement = 20,
		Decrement = 21,
		PreDecrement = 22,
		PostDecrement = 23,
		ConditionalIf = 24,
		ConditionalElse = 25,
		ConditionalNull = 26
	};

	enum class OperatorPosition
	{
		Irrelevant = 0,
		Left = 1,
		Right = 2
	};

	enum class OperatorLinkage
	{
		Single = 0x00,
		Left = 0x01,
		Right = 0x02,
		Both = Left | Right
	};

	enum class NamespaceType
	{
		Main = 0x01,
		Named = 0x02,
		Enclosed = 0x04,
		Regular = Named | Enclosed
	};

	typedef std::vector<FunctionParameter> PARAMS_T;
	typedef std::vector<std::unique_ptr<ScriptValue>> CALLABLE_PARAMS_T;

	constexpr char keywordNull[] = "null";
	constexpr char keywordTrue[] = "true";
	constexpr char keywordFalse[] = "false";

	const std::shared_ptr<ScriptNullValue> getNull();
	const std::shared_ptr<ScriptBoolValue> getTrue();
	const std::shared_ptr<ScriptBoolValue> getFalse();
}

DEFINE_BITFIELD(gscript::Modifier);
DEFINE_BITFIELD(gscript::ClassModifier);
DEFINE_BITFIELD(gscript::OperatorPosition);
DEFINE_BITFIELD(gscript::OperatorLinkage);
DEFINE_BITFIELD(gscript::NamespaceType);

#endif