#include "for.hpp"
#include "script.hpp"
#include "pFor.hpp"

namespace gscript
{
	const ScriptType *ScriptFor::returnType = new ScriptType(VALUE_TYPE_T::VT_NULL);

	ScriptFor::ScriptFor(ScriptScope &scope, const ScriptVarDeclaration &vardecl, const ScriptStatement &condition, const ScriptStatement &progress)
		:ScriptCallable(scope),
		ScriptRunnable(&scope),
		vardecl(new ScriptVarDeclaration(vardecl)),
		condition(new ScriptStatement(condition)),
		progress(new ScriptStatement(progress))
	{
	}

	ScriptFor::ScriptFor(ScriptScope &scope, const ParserFor &pfor)
		: ScriptCallable(scope),
		ScriptRunnable(&scope),
		vardecl(pfor.arglist.varDecl ? new ScriptVarDeclaration(*this, *pfor.arglist.varDecl) : nullptr),
		condition(new ScriptStatement(*this, *pfor.arglist.condition)),
		progress(new ScriptStatement(*this, *pfor.arglist.progress))
	{ }

	ScriptValue *ScriptFor::run(const CALLABLE_PARAMS_T &c)
	{
		if (this->vardecl)
			this->vardecl->run();

		while (this->condition->run()->boolean().getValue())
		{
			ScriptRunnable::run();

			if (this->progress)
				this->progress->run();
		}

		return Script::SCR_NULL;
	}

	const ScriptType *ScriptFor::getType() const
	{
		return ScriptFor::returnType;
	}
}