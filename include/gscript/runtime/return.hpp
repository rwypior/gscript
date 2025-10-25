#ifndef _h_gscript_return
#define _h_gscript_return

#include "callable.hpp"
#include "statement.hpp"

namespace gscript
{
	class ScriptReturn : public ScriptCallable
	{
	public:
		SCRIPT_API ScriptReturn(const ScriptReturn& b);
		SCRIPT_API ScriptReturn(ScriptScopeBase& scope, std::unique_ptr<ScriptStatement>&& statement);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());

		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const override;

		SCRIPT_API std::unique_ptr<ScriptCallable>& getStatement();

	private:
		std::unique_ptr<ScriptCallable> statement;
	};
}

#endif