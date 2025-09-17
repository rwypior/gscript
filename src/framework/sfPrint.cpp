#include "framework/sfPrint.hpp"
#include "runtime/scriptValue.hpp"
#include "runtime/callable.hpp"
#include "script.hpp"
#include "defs.hpp"
#include "debug.hpp"

#include <iostream>

namespace gscript
{
	ScriptFuncPrint::ScriptFuncPrint(ScriptScopeBase& scope, const std::string &name)
		: ScriptFunction(scope, name, ScriptType::create(VALUE_TYPE_T::VT_NULL, this->scope), PARAMS_T(1, FunctionParameter(ScriptType::create(VALUE_TYPE_T::VT_STRING, scope))))
	{

	}

	std::unique_ptr<ScriptValue> ScriptFuncPrint::run(const CALLABLE_PARAMS_T &c)
	{
		this->validateParams(c);

		const ScriptValue &val = *c[0];
		const ScriptStringValue &sval = static_cast<const ScriptStringValue&>(val);

		std::cout << sval.getValue() << std::flush;

		return ScriptType::null();
	}

	const std::shared_ptr<ScriptType> ScriptFuncPrint::getType() const
	{
		return ScriptType::nulltype();
	}
}