#include "gscript/runtime/varDeclaration.hpp"
#include "gscript/runtime/function.hpp"
#include "gscript/runtime/scope.hpp"
#include "gscript/runtime/classInstance.hpp"

namespace gscript
{
	// Variable declaration assigns value to variable that was created beforehand
	// Eg. when compiling a namespace, parsing a variable first registers a variable
	// and then creates runtime variable declaration entry which is later run as a
	// statement and a value to it is assigned

	ScriptVarDeclaration::ScriptVarDeclaration(const ScriptVarDeclaration& b)
		: accessor(b.accessor->clone())
		, statement(b.statement->clone())
	{
	}

	ScriptVarDeclaration::ScriptVarDeclaration(ScriptScope &scope, ScriptVariable &var, std::unique_ptr<ScriptStatement> &&statement)
		: accessor(VariableAccessor::find(scope, var.getName()))
		, statement(std::move(statement))
	{
	}

	ScriptVarDeclaration::ScriptVarDeclaration(std::unique_ptr<VariableAccessor>&& accessor, std::unique_ptr<ScriptStatement> &&statement)
		: accessor(std::move(accessor))
		, statement(std::move(statement))
	{
	}

	std::unique_ptr<ScriptCallable> ScriptVarDeclaration::clone()
	{
		return std::make_unique<ScriptVarDeclaration>(*this);
	}

	void ScriptVarDeclaration::setInstance(ScriptClassInstance &instance)
	{
		this->accessor->setScope(&instance);
	}

	std::unique_ptr<ScriptValue> ScriptVarDeclaration::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &params)
	{
		this->accessor->get(&scope)->init(this->statement->run(scope));
		return ScriptType::null();
	}

	const std::shared_ptr<ScriptType> ScriptVarDeclaration::getType() const
	{
		return this->statement->getType();
	}

	const std::unique_ptr<ScriptCallable>& ScriptVarDeclaration::getStatement() const
	{
		return this->statement;
	}

	std::unique_ptr<ScriptCallable>& ScriptVarDeclaration::getStatement()
	{
		return this->statement;
	}

	// Field declaration

	ScriptFieldDeclaration::ScriptFieldDeclaration(const ScriptFieldDeclaration& b)
		: ScriptVarDeclaration(b)
		, name(b.name)
		, type(b.type->clone())
	{
	}

	ScriptFieldDeclaration::ScriptFieldDeclaration(const std::string& name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptStatement>&& statement)
		: ScriptVarDeclaration({}, std::move(statement))
		, name(name)
		, type(type)
	{
	}

	std::unique_ptr<ScriptCallable> ScriptFieldDeclaration::clone()
	{
		return std::make_unique<ScriptFieldDeclaration>(*this);
	}

	void ScriptFieldDeclaration::instantiate(ScriptScopeBase& instance)
	{
		auto& var = instance.registerVariable(std::make_unique<ScriptVariable>(this->name, this->type, nullptr));
		this->accessor = VariableAccessor::find(instance, var.getName(), false);
	}
}