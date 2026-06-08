#ifndef _h_gscript_operator_function_factory
#define _h_gscript_operator_function_factory

#include "gscript/defs.hpp"
#include "gscript/util.hpp"
#include "gscript/lib.hpp"

#include <functional>

namespace gscript
{
	class ScriptCallable;
	class ScriptScopeBase;

	namespace OperatorFunctionFactory
	{
		//typedef std::unique_ptr<ScriptValue>(*OperatorFunction)(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right);
		using OperatorFunction = std::function<std::unique_ptr<ScriptValue>(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)>;

		SCRIPT_API OperatorFunction getFunction(std::shared_ptr<ScriptCallable> left, std::shared_ptr<ScriptCallable> right, OperatorType oper);
	};
}

#endif