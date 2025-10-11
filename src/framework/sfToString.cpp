#include "framework/sfToString.hpp"
#include "runtime/scriptValue.hpp"
#include "defs.hpp"

namespace gscript
{
	const ScriptType ScriptFuncToString::returnType = ScriptType(ValueType::String);

	ScriptFuncToString::ScriptFuncToString(ScriptScope &scope, const std::string &name)
		: ScriptFunction(scope, name, ScriptType::create(ValueType::String, scope), PARAMS_T(1, FunctionParameter(ScriptType::create(ValueType::Int, scope))))
	{
		this->setup(scope);
	}

	std::unique_ptr<ScriptValue> ScriptFuncToString::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		this->validateParams(c);

		const ScriptValue &val = *c[0];
		auto resolved = val.data();

		std::string res;
		if (auto cv = dynamic_cast<const ScriptClassValue*>(resolved))
		{
			// TODO - implement this
		}
		else if (auto cv = dynamic_cast<const ScriptCharValue*>(resolved))
			res = std::string(1, cv->getValue());
		else if (auto cv = dynamic_cast<const ScriptByteValue*>(resolved))
			res = std::to_string(cv->getValue());
		else if (auto cv = dynamic_cast<const ScriptIntValue*>(resolved))
			res = std::to_string(cv->getValue());
		else if (auto cv = dynamic_cast<const ScriptUnsignedIntValue*>(resolved))
			res = std::to_string(cv->getValue());
		else if (auto cv = dynamic_cast<const ScriptFloatValue*>(resolved))
			res = std::to_string(cv->getValue());
		else if (auto cv = dynamic_cast<const ScriptDoubleValue*>(resolved))
			res = std::to_string(cv->getValue());

		return std::make_unique<ScriptStringValue>(res);
	}

	const ScriptType &ScriptFuncToString::getType()
	{
		return ScriptFuncToString::returnType;
	}
}