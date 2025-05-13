#include "sfToString.hpp"
#include "scriptValue.hpp"
#include "defs.hpp"

namespace gscript
{
	const ScriptType ScriptFuncToString::returnType = ScriptType(VALUE_TYPE_T::VT_STRING);

	ScriptFuncToString::ScriptFuncToString(ScriptScope &scope, const std::string &name)
		:ScriptFunction(scope, name, ScriptType::create(VALUE_TYPE_T::VT_STRING, this->scope), PARAMS_T(1, FunctionParameter(ScriptType::create(VALUE_TYPE_T::VT_INT, scope))))
	{

	}

	ScriptValue *ScriptFuncToString::run(const CALLABLE_PARAMS_T &c)
	{
		this->validateParams(c);

		const ScriptValue &val = *c[0];
		const ScriptIntValue &sval = static_cast<const ScriptIntValue&>(val);

		//const ScriptIntValue &sval = *static_cast<const ScriptReferenceValue&>(val).resolve<ScriptIntValue*>();

		//const ScriptIntValue *sval = this->getParam<ScriptIntValue>(c[0]);

		//auto a = sval.getValue();

		return new ScriptStringValue(std::to_string(sval.getValue()));
	}

	const ScriptType &ScriptFuncToString::getType()
	{
		return ScriptFuncToString::returnType;
	}
}