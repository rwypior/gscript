#include "runtime/varDeclaration.hpp"
#include "runtime/function.hpp"
#include "runtime/scope.hpp"
#include "runtime/classInstance.hpp"

namespace gscript
{
	// Variable declaration assigns value to variable that was created beforehand
	// Eg. when compiling a namespace, parsing a variable first registers a variable
	// and then creates runtime variable declaration entry which is later run as a
	// statement and a value to it is assigned

	ScriptVarDeclaration::ScriptVarDeclaration(ScriptScope &scope, ScriptVariable &var, std::unique_ptr<ScriptStatement> &&statement)
		: ScriptCallable(scope),
		accessor(VariableAccessor::find(scope, var.getName())),
		statement(std::move(statement))
	{ }

	ScriptVarDeclaration::ScriptVarDeclaration(ScriptScope &scope, std::unique_ptr<VariableAccessor>&& accessor, std::unique_ptr<ScriptStatement> &&statement)
		: ScriptCallable(scope),
		accessor(std::move(accessor)),
		statement(std::move(statement))
	{
	}

	void ScriptVarDeclaration::setInstance(ScriptClassInstance &instance)
	{
		this->accessor->setScope(&instance);
	}

	std::unique_ptr<ScriptValue> ScriptVarDeclaration::run(const CALLABLE_PARAMS_T &params)
	{
		this->accessor->get()->init(this->statement->run());

		return ScriptType::null();
	}

	const std::shared_ptr<ScriptType> ScriptVarDeclaration::getType() const
	{
		return this->statement->getType();
	}

	// Field declaration

	ScriptFieldDeclaration::ScriptFieldDeclaration(ScriptScope& scope, const std::string& name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptStatement>&& statement)
		: ScriptVarDeclaration(scope, {}, std::move(statement))
		, name(name)
		, type(type)
	{
	}

	void ScriptFieldDeclaration::instantiate(ScriptScopeBase& instance)
	{
		auto& var = instance.registerVariable(std::make_unique<ScriptVariable>(this->name, this->type, nullptr));
		this->accessor = VariableAccessor::find(instance, var.getName(), false);
		//this->var = new DirectEntityLink<ScriptVariable&>(var);
	}

	// Prototype

	ScriptVarDeclarationPrototype::ScriptVarDeclarationPrototype(ScriptScope& scope, const std::string& name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptStatement>&& statement)
		: ScriptCallablePrototype(scope)
		, name(name)
		, type(type)
		, statement(std::move(statement))
	{
	}

	std::unique_ptr<ScriptCallable> ScriptVarDeclarationPrototype::build(ScriptScopeBase* scope)
	{
		// TODO
		return nullptr;
	}
}