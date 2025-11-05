#ifndef _h_gscript_new
#define _h_gscript_new

#include "gscript/defs.hpp"
#include "gscript/runtime/statement.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/funcCall.hpp"

namespace gscript
{
	class ScriptNew : public ScriptFuncCall
	{
	public:
		SCRIPT_API ScriptNew(const ScriptNew& b);
		SCRIPT_API ScriptNew(ScriptClass& sclass, std::vector<std::unique_ptr<ScriptStatement>>&& params = {});

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

	protected:
		ScriptClass &sclass;
	};
}

#endif