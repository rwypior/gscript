#include "gscript/runtime/varDeclaration.hpp"
#include "gscript/runtime/function.hpp"
#include "gscript/runtime/scope.hpp"
#include "gscript/runtime/classInstance.hpp"
#include "gscript/runtime/literal.hpp"

namespace
{
	std::unique_ptr<gscript::ScriptStatement> literalStatement(std::unique_ptr<gscript::ScriptValue>&& value)
	{
		return std::make_unique<gscript::ScriptStatement>(std::make_unique<gscript::ScriptLiteral>(std::move(value)));
	}
}

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

	ScriptVarDeclaration::ScriptVarDeclaration(std::unique_ptr<VariableAccessor>&& accessor, std::unique_ptr<ScriptCallable>&& callable)
		: accessor(std::move(accessor))
		, statement(std::move(callable))
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

	ScriptFieldDeclaration::ScriptFieldDeclaration(ScriptScope& scope, const std::string& name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptStatement>&& statement)
		: ScriptVarDeclaration(FieldAccessor::find(scope, name), std::move(statement))
		, name(name)
		, type(type)
	{
	}

	ScriptFieldDeclaration::ScriptFieldDeclaration(ScriptScope& scope, const std::string& name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptValue>&& value)
		: ScriptVarDeclaration(FieldAccessor::find(scope, name), literalStatement(std::move(value)))
		, name(name)
		, type(type)
	{
	}

	ScriptFieldDeclaration::ScriptFieldDeclaration(ScriptScope& scope, const std::string& name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptCallable>&& callable)
		: ScriptVarDeclaration(FieldAccessor::find(scope, name), std::move(callable))
		, name(name)
		, type(type)
	{
	}

	ScriptFieldDeclaration::ScriptFieldDeclaration(std::unique_ptr<VariableAccessor>&& accessor, const std::string& name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptCallable>&& callable)
		: ScriptVarDeclaration(std::move(accessor), std::move(callable))
		, name(name)
		, type(type)
	{
	}

	ScriptFieldDeclaration::ScriptFieldDeclaration(std::unique_ptr<VariableAccessor>&& accessor, const std::string& name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptValue>&& value)
		: ScriptVarDeclaration(std::move(accessor), literalStatement(std::move(value)))
		, name(name)
		, type(type)
	{
	}

	const std::string& ScriptFieldDeclaration::getName() const
	{
		return this->name;
	}

	std::unique_ptr<ScriptCallable> ScriptFieldDeclaration::clone()
	{
		return std::make_unique<ScriptFieldDeclaration>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptFieldDeclaration::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T& params)
	{
		this->instantiate(scope);
		return ScriptVarDeclaration::run(scope, params);
	}

	void ScriptFieldDeclaration::instantiate(ScriptScopeBase& instance)
	{
		auto& var = instance.registerVariable(std::make_unique<ScriptVariable>(this->name, this->type, nullptr));
		this->accessor = VariableAccessor::find(instance, var.getName(), false);
	}

	// Field declaration prototype

	ScriptFieldDeclarationPrototype::ScriptFieldDeclarationPrototype(ScriptScope& scope, const std::string& name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptStatement>&& statement)
		: ScriptFieldDeclaration({}, name, type, std::move(statement))
	{
	}

	ScriptFieldDeclarationPrototype::ScriptFieldDeclarationPrototype(ScriptScope& scope, const std::string& name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptValue>&& value)
		: ScriptFieldDeclaration({}, name, type, std::move(value))
	{
	}

	std::unique_ptr<ScriptCallable> ScriptFieldDeclarationPrototype::build(ScriptScopeBase& scope)
	{
		assert(dynamic_cast<ScriptScope*>(&scope) && "Field declaration may only be used in class scope");
		return std::make_unique<ScriptFieldDeclaration>(static_cast<ScriptScope&>(scope), this->name, this->type, std::move(this->statement));
	}
}