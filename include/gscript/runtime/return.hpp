#ifndef _h_gscript_return
#define _h_gscript_return

#include "callable.hpp"
#include "statement.hpp"

namespace gscript
{
	class ScriptReturn : public ScriptCallable
	{
	public:

		ScriptReturn(ScriptScopeBase& scope, std::unique_ptr<ScriptStatement>&& statement);
		virtual std::unique_ptr<ScriptValue> run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());

		virtual const std::shared_ptr<ScriptType> getType() const override;

		std::unique_ptr<ScriptCallable>& getStatement();

	private:
		static const ScriptType *returnType;

		std::unique_ptr<ScriptCallable> statement;
	};
}

#endif