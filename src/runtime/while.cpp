#include "runtime/while.hpp"
#include "runtime/statement.hpp"

namespace gscript
{
	ScriptWhile::ScriptWhile(ScriptScope &scope, std::unique_ptr<ScriptStatement>&& condition, std::vector<std::shared_ptr<ScriptCallable>>&& statements)
		: ScriptCallable(scope)
		, ScriptExecutiveBlock(std::move(statements))
		, condition(std::move(condition))
	{
	}

	std::unique_ptr<ScriptValue> ScriptWhile::run(const CALLABLE_PARAMS_T &c)
	{
		while (this->condition->run()->boolean().getValue())
			ScriptExecutiveBlock::execute();

		return ScriptType::null();
	}

	const std::shared_ptr<ScriptType> ScriptWhile::getType() const
	{
		return ScriptType::nulltype();
	}
}