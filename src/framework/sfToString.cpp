#include "framework/sfToString.hpp"
#include "runtime/scriptValue.hpp"
#include "defs.hpp"

namespace gscript
{
	const ScriptType ScriptFuncToString::returnType = ScriptType(ValueType::String);

	ScriptFuncToString::ScriptFuncToString(ScriptScope &scope, const std::string &name)
		: ScriptFunction(scope, name, ScriptType::create(ValueType::String, this->scope), PARAMS_T(1, FunctionParameter(ScriptType::create(ValueType::Int, scope))))
	{
		PARAMS_T;
		FunctionParameter;
	}

	std::unique_ptr<ScriptValue> ScriptFuncToString::run(const CALLABLE_PARAMS_T &c)
	{
		this->validateParams(c);

		const ScriptValue &val = *c[0];
		const ScriptIntValue &sval = static_cast<const ScriptIntValue&>(val);

		return std::make_unique<ScriptStringValue>(std::to_string(sval.getValue()));
	}

	const ScriptType &ScriptFuncToString::getType()
	{
		return ScriptFuncToString::returnType;
	}
}