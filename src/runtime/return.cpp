#include "runtime/return.hpp"

namespace gscript
{
	ScriptReturn::ScriptReturn(ScriptScopeBase& scope, std::unique_ptr<ScriptStatement>&& statement)
		: ScriptCallable(scope)
		, statement(std::move(statement))
	{
	}

	std::unique_ptr<ScriptValue> ScriptReturn::run(const CALLABLE_PARAMS_T &c)
	{
		return this->statement->run();
	}

	const std::shared_ptr<ScriptType> ScriptReturn::getType() const
	{
		return this->statement->getType();
	}

	std::unique_ptr<ScriptCallable>& ScriptReturn::getStatement()
	{
		return this->statement;
	}
}