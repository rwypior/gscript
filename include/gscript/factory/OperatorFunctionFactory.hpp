#ifndef _h_gscript_operator_function_factory
#define _h_gscript_operator_function_factory

#include "defs.hpp"
#include "util.hpp"

namespace gscript
{
	class ScriptCallable;
	class ScriptScopeBase;

	namespace OperatorFunctionFactory
	{
		typedef std::unique_ptr<ScriptValue>(*OperatorFunction)(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right);

		OperatorFunction getFunction(ValueType left, ValueType right, OperatorType oper);
	};
}

#endif