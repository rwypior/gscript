#include "runtime/for.hpp"
#include "defs.hpp"
#include "util.hpp"

namespace gscript
{
	ScriptFor::ScriptFor(const ScriptFor& b)
		: ScriptExecutiveBlock(b)
		, vardecl(static_unique_pointer_cast<ScriptVarDeclaration>( b.vardecl->clone() ))
		, condition(static_unique_pointer_cast<ScriptStatement>( b.condition->clone() ))
		, progress(static_unique_pointer_cast<ScriptStatement>( b.progress->clone() ))
	{
	}

	ScriptFor::ScriptFor(
			std::unique_ptr<ScriptVarDeclaration>&& vardecl, 
			std::unique_ptr<ScriptStatement>&& condition, 
			std::unique_ptr<ScriptStatement>&& progress,
			std::vector<std::unique_ptr<ScriptCallable>>&& statements
		)
		: ScriptExecutiveBlock(std::move(statements))
		, vardecl(std::move(vardecl))
		, condition(std::move(condition))
		, progress(std::move(progress))
	{
	}

	std::unique_ptr<ScriptCallable> ScriptFor::clone()
	{
		return std::make_unique<ScriptFor>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptFor::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		if (this->vardecl)
			this->vardecl->run(scope);

		while (this->condition->run(scope)->boolean().getValue())
		{
			ScriptExecutiveBlock::execute(scope);

			if (this->progress)
				this->progress->run(scope);
		}

		return ScriptType::null();
	}

	const std::shared_ptr<ScriptType> ScriptFor::getType() const
	{
		return ScriptType::nulltype();
	}
}