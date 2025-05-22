#ifndef _h_gscript_func_tostring
#define _h_gscript_func_tostring

#include "function.hpp"

namespace gscript
{
	class ScriptFuncToString : public ScriptFunction
	{
	private:
		static const ScriptType returnType;

	public:
		ScriptFuncToString(ScriptScope &scope, const std::string &name);
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType &getType();
	};
}

#endif