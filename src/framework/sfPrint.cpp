#include "gscript/framework/sfPrint.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/script.hpp"
#include "gscript/defs.hpp"
#include "gscript/debug.hpp"

#include <iostream>

namespace gscript
{
	ScriptFuncPrint::ScriptFuncPrint(ScriptScopeBase& scope, const std::string &name)
		: ScriptFunction(scope, name, ScriptType::create(ValueType::Null, scope), PARAMS_T(1, FunctionParameter(ScriptType::create(ValueType::String, scope))))
	{
	}

	std::unique_ptr<ScriptValue> ScriptFuncPrint::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
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