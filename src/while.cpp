#include "while.hpp"
#include "script.hpp"
#include "pWhile.hpp"
#include "statement.hpp"

namespace gscript
{
	const ScriptType *ScriptWhile::returnType = new ScriptType(VALUE_TYPE_T::VT_NULL);

	ScriptWhile::ScriptWhile(ScriptScope &scope, const ScriptStatement &condition)
		:ScriptCallable(scope),
		ScriptRunnable(&scope),
		condition(new ScriptStatement(condition))
	{
	}

	ScriptWhile::ScriptWhile(ScriptScope &scope, const ParserWhile &pwhile)
		: ScriptCallable(scope),
		ScriptRunnable(&scope),
		condition(new ScriptStatement(scope, *pwhile.arglist.parameters.front()))
	{
	}

	ScriptValue *ScriptWhile::run(const CALLABLE_PARAMS_T &c)
	{
		while (this->condition->run()->boolean().getValue())
			ScriptRunnable::run();

		return Script::SCR_NULL;
	}

	const ScriptType *ScriptWhile::getType() const
	{
		return ScriptWhile::returnType;
	}
}