#include "varDeclaration.hpp"
#include "function.hpp"
#include "scope.hpp"
#include "script.hpp"

#include <iostream>

namespace gscript
{
	ScriptVarDeclaration::ScriptVarDeclaration(ScriptScope &scope, ScriptVariable &var, const ScriptStatement &statement)
		: ScriptCallable(scope),
		//var(var),
		var(new DirectEntityLink<ScriptVariable&>(var)),
		statement(statement)
	{ }

	ScriptVarDeclaration::ScriptVarDeclaration(ScriptScope &scope, EntityLink<ScriptVariable&> *link, const ScriptStatement &statement)
		: ScriptCallable(scope),
		//var(var),
		var(link),
		statement(statement)
	{
	}

	ScriptVarDeclaration::ScriptVarDeclaration(ScriptScope &scope, ParserVarDeclaration &pvardecl)
		: ScriptCallable(scope),
		var(new DirectEntityLink<ScriptVariable&>(scope.registerVariable(pvardecl, nullptr))),
		statement(scope, pvardecl.value)
	{ }

	void ScriptVarDeclaration::setInstance(ScriptClassInstance &instance)
	{
		static_cast<MemberEntityLink< ScriptVariable&, ScriptClassInstance::INSTANCE_VARIABLES_CONTAINER_T >*>(this->var)->container = &instance.getVariables();
	}

	ScriptValue *ScriptVarDeclaration::run(const CALLABLE_PARAMS_T &params)
	{
		//this->var.setValue(this->statement.run()->clone());
		this->var->get().init(this->statement.run());

		std::cout << "VarDeclaration(" << this->var->get().getName() << ", " << this->var->get().getValue() << ", " << this->var->get().getInternalPointer() << ")" << std::endl;

		return nullptr;
	}

	const ScriptType *ScriptVarDeclaration::getType() const
	{
		return this->statement.getType();
	}
}