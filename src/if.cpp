#include "if.hpp"
#include "script.hpp"
#include "pIf.hpp"
#include "pElse.hpp"

namespace gscript
{
	const ScriptType *ScriptIf::returnType = new ScriptType(VALUE_TYPE_T::VT_NULL);

	ScriptIf::ScriptIf(ScriptScope &scope, const ScriptStatement &condition)
		:ScriptCallable(scope),
		ScriptRunnable(&scope),
		condition(new ScriptStatement(condition))
	{
	}

	ScriptIf::ScriptIf(ScriptScope &scope, const ParserIf &pif, ScriptScope &parentScope)
		: ScriptCallable(scope),
		ScriptRunnable(&scope),
		condition(new ScriptStatement(scope, *pif.arglist.parameters.front())),
		selse(pif.pelse.body.body.statements.empty() ? NULL : new ScriptIf(scope, pif.pelse, parentScope))
	{
		this->setParentScope(&parentScope);

		if (this->selse)
			this->selse->setup(pif.pelse.body.body);
	}

	ScriptIf::ScriptIf(ScriptScope &scope, const ParserElse &pelse, ScriptScope &parentScope)
		: ScriptCallable(scope),
		ScriptRunnable(&scope),
		condition(pelse.pif ? new ScriptStatement(this->scope, pelse.pif->arglist.parameters.front()) : NULL)
	{
		this->setParentScope(&parentScope);
	}

	ScriptValue *ScriptIf::run(const CALLABLE_PARAMS_T &c)
	{
		if (!this->condition || this->condition->run()->boolean().getValue())
			return ScriptRunnable::run();
		else if (this->selse)
			return this->selse->run();

		return Script::SCR_NULL;
	}

	const ScriptType *ScriptIf::getType() const
	{
		return ScriptIf::returnType;
	}
}