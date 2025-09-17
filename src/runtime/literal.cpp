#include "runtime/literal.hpp"
#include "runtime/scriptValue.hpp"
#include "defs.hpp"
#include "runtimeException.hpp"
#include "debug.hpp"

namespace gscript
{
	ScriptLiteral::ScriptLiteral(ScriptScopeBase& scope, std::unique_ptr<ScriptValue> &&val)
		: ScriptCallable(scope)
		, val(std::move(val))
	{ }

	//ScriptLiteral::ScriptLiteral(ScriptScope &scope, ParserLiteral pliteral)
	//	: ScriptCallable(scope)
	//{
	//	switch (pliteral.type)
	//	{
	//	case VALUE_TYPE_T::VT_INT:
	//		this->val = new ScriptIntValue(std::stoi(pliteral.value));
	//		break;
	//	case VALUE_TYPE_T::VT_STRING:
	//		this->val = new ScriptStringValue(pliteral.value);
	//		break;
	//	case VALUE_TYPE_T::VT_FLOAT:
	//		this->val = new ScriptFloatValue(std::stof(pliteral.value));
	//		break;
	//	case VALUE_TYPE_T::VT_DOUBLE:
	//		this->val = new ScriptDoubleValue(std::stod(pliteral.value));
	//		break;
	//	case VALUE_TYPE_T::VT_CHAR:
	//		this->val = new ScriptCharValue(pliteral.value[0]);
	//		break;
	//	default:
	//		throw RuntimeException(std::string("Unknown type "));
	//	}
	//}

	std::unique_ptr<ScriptValue> ScriptLiteral::run(const CALLABLE_PARAMS_T &c)
	{
		return this->val->clone();
	}

	const std::shared_ptr<ScriptType> ScriptLiteral::getType() const
	{
		return this->val->getType();
	}
}