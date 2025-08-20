#ifndef _h_gscript_callable
#define _h_gscript_callable

#include "scriptValue.hpp"
#include "lib.hpp"

#include <memory>
#include <vector>

namespace gscript
{
	class ScriptScope;

	class ScriptCallable
	{
	public:
		SCRIPT_API ScriptCallable(ScriptScope &scope);

		SCRIPT_API virtual const ScriptType *getType() const = 0;

		SCRIPT_API virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) = 0;

		SCRIPT_API ScriptScope &getScope();

	protected:
		ScriptScope& scope;
	};
}

#endif