#ifndef _h_gscript_new
#define _h_gscript_new

#include "defs.hpp"
#include "statement.hpp"
#include "scriptValue.hpp"
#include "funcCall.hpp"

#include <string>

namespace gscript
{
	class ScriptNew : public ScriptFuncCall
	{
	public:
		ScriptNew(ScriptClass &sclass, const std::vector<ScriptStatement> &params);

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

	protected:
		ScriptClass &sclass;
	};
}

#endif