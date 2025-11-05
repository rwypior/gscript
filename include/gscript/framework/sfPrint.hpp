#ifndef _h_gscript_sf_print
#define _h_gscript_sf_print

#include "gscript/lib.hpp"
#include "gscript/runtime/function.hpp"

#include <string>

namespace gscript
{
	class ScriptScope;

	class ScriptFuncPrint : public ScriptFunction
	{
	public:
		SCRIPT_API ScriptFuncPrint(ScriptScopeBase& scope, const std::string &name);
		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const override;
	};
}

#endif