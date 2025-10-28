#include "runtime/return.hpp"

namespace gscript
{
	ScriptReturn::ScriptReturn(const ScriptReturn& b)
		: statement(b.statement->clone())
	{
	}

	ScriptReturn::ScriptReturn(std::unique_ptr<ScriptStatement>&& statement)
		: statement(std::move(statement))
	{
	}

	std::unique_ptr<ScriptCallable> ScriptReturn::clone()
	{
		return std::make_unique<ScriptReturn>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptReturn::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return std::make_unique<ScriptReturnValue>(this->statement->run(scope));
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