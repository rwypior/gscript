#include "runtime/varDeclaration.hpp"
#include "runtime/function.hpp"
#include "runtime/scope.hpp"

namespace gscript
{
	// Variable declaration assigns value to variable that was created beforehand
	// Eg. when compiling a namespace, parsing a variable first registers a variable
	// and then creates runtime variable declaration entry which is later run as a
	// statement and a value to it is assigned

	ScriptVarDeclaration::ScriptVarDeclaration(ScriptScope &scope, ScriptVariable &var, std::unique_ptr<ScriptStatement> &&statement)
		: ScriptCallable(scope),
		//var(var),
		var(new DirectEntityLink<ScriptVariable&>(var)),
		statement(std::move(statement))
	{ }

	ScriptVarDeclaration::ScriptVarDeclaration(ScriptScope &scope, EntityLink<ScriptVariable&> *link, std::unique_ptr<ScriptStatement> &&statement)
		: ScriptCallable(scope),
		//var(var),
		var(link),
		statement(std::move(statement))
	{
	}

	/*ScriptVarDeclaration::ScriptVarDeclaration(ScriptScope &scope, ParserVarDeclaration &pvardecl)
		: ScriptCallable(scope),
		var(new DirectEntityLink<ScriptVariable&>(scope.registerVariable(pvardecl, nullptr))),
		statement(scope, pvardecl.value)
	{ }*/

	//void ScriptVarDeclaration::setup(ScriptScope& scope)
	//{
	//	//scope.registerVariable()
	//}

	void ScriptVarDeclaration::setInstance(ScriptClassInstance &instance)
	{
		static_cast<MemberEntityLink<ScriptVariable&, ScriptClassInstance::VariableContainer>*>(this->var)->container = &instance.getVariables();
	}

	ScriptValue *ScriptVarDeclaration::run(const CALLABLE_PARAMS_T &params)
	{
		//this->var.setValue(this->statement.run()->clone());
		
		//this->scope.registerVariable(this->name, this->type.get(), this->statement->run());
		this->var->get().init(this->statement->run());

		//std::cout << "VarDeclaration(" << this->var->get().getName() << ", " << this->var->get().getValue() << ", " << this->var->get().getInternalPointer() << ")" << std::endl;

		return nullptr;
	}

	const ScriptType *ScriptVarDeclaration::getType() const
	{
		return this->statement->getType();
	}
}