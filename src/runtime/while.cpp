#include "runtime/while.hpp"
#include "runtime/statement.hpp"

namespace gscript
{
	const ScriptType *ScriptWhile::returnType = new ScriptType(VALUE_TYPE_T::VT_NULL);

	ScriptWhile::ScriptWhile(ScriptScope &scope, std::unique_ptr<ScriptStatement>&& condition, std::vector<std::shared_ptr<ScriptCallable>>&& statements)
		: ScriptCallable(scope)
		, ScriptExecutiveBlock(std::move(statements))
		, condition(std::move(condition))
	{
	}

	ScriptValue *ScriptWhile::run(const CALLABLE_PARAMS_T &c)
	{
		while (this->condition->run()->boolean().getValue())
			ScriptExecutiveBlock::execute();

		return SCR_NULL;
	}

	const ScriptType *ScriptWhile::getType() const
	{
		return ScriptWhile::returnType;
	}
}