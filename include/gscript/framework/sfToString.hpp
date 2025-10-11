#ifndef _h_gscript_func_tostring
#define _h_gscript_func_tostring

#include "runtime/function.hpp"

namespace gscript
{
	class ScriptFuncToString : public ScriptFunction
	{
	public:
		ScriptFuncToString(ScriptScope &scope, const std::string &name);
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType &getType();

	private:
		static const ScriptType returnType;
	};
}

#endif