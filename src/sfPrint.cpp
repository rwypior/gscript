#include "sfPrint.hpp"
#include "scriptValue.hpp"
#include "callable.hpp"
#include "script.hpp"
#include "defs.hpp"

#include "debug.hpp"

#include <iostream>
#include <memory>
#include <vector>

namespace gscript
{
	const ScriptType ScriptFuncPrint::returnType = ScriptType(VALUE_TYPE_T::VT_NULL);

	ScriptFuncPrint::ScriptFuncPrint(ScriptScope &scope, const std::string &name)
		:ScriptFunction(scope, name, ScriptType::create(VALUE_TYPE_T::VT_NULL, this->scope), PARAMS_T(1, FunctionParameter(ScriptType::create(VALUE_TYPE_T::VT_STRING, scope))))
	{

	}

	ScriptValue *ScriptFuncPrint::run(const CALLABLE_PARAMS_T &c)
	{
		this->validateParams(c);

		const ScriptValue &val = *c[0];
		const ScriptStringValue &sval = static_cast<const ScriptStringValue&>(val);

		std::cout << sval.getValue() << std::flush;

		return Script::SCR_NULL;
	}

	const ScriptType &ScriptFuncPrint::getType()
	{
		return ScriptFuncPrint::returnType;
	}
}