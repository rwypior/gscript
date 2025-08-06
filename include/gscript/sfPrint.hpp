#ifndef _h_gscript_sf_print
#define _h_gscript_sf_print

#include "function.hpp"

#include <string>

namespace gscript
{
	class ScriptScope;

	class ScriptFuncPrint : public ScriptFunction
	{
	public:
		ScriptFuncPrint(ScriptScope &scope, const std::string &name);
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType &getType();

	private:
		static const ScriptType returnType;
	};
}

#endif