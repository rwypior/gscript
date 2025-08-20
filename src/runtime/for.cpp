#include "runtime/for.hpp"
#include "defs.hpp"

namespace gscript
{
	const ScriptType *ScriptFor::returnType = new ScriptType(VALUE_TYPE_T::VT_NULL);

	ScriptFor::ScriptFor(
			ScriptScope &scope, 
			std::unique_ptr<ScriptVarDeclaration>&& vardecl, 
			std::unique_ptr<ScriptStatement>&& condition, 
			std::unique_ptr<ScriptStatement>&& progress,
			std::vector<std::shared_ptr<ScriptCallable>>&& statements
		)
		: ScriptCallable(scope)
		, ScriptExecutiveBlock(std::move(statements))
		, vardecl(std::move(vardecl))
		, condition(std::move(condition))
		, progress(std::move(progress))
	{
	}

	ScriptValue *ScriptFor::run(const CALLABLE_PARAMS_T &c)
	{
		if (this->vardecl)
			this->vardecl->run();

		while (this->condition->run()->boolean().getValue())
		{
			ScriptExecutiveBlock::execute();

			if (this->progress)
				this->progress->run();
		}

		return SCR_NULL;
	}

	const ScriptType *ScriptFor::getType() const
	{
		return ScriptFor::returnType;
	}
}