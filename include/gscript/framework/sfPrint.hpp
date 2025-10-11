#ifndef _h_gscript_sf_print
#define _h_gscript_sf_print

#include "runtime/function.hpp"

#include <string>

namespace gscript
{
	class ScriptScope;

	class ScriptFuncPrint : public ScriptFunction
	{
	public:
		ScriptFuncPrint(ScriptScopeBase& scope, const std::string &name);
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const std::shared_ptr<ScriptType> getType() const override;
	};
}

#endif