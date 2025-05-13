#ifndef _h_script_func_tostring
#define _h_script_func_tostring

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