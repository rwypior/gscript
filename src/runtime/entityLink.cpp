#include "gscript/runtime/entityLink.hpp"
#include "gscript/runtime/scope.hpp"
#include "gscript/runtime/function.hpp"

namespace gscript
{
	// Variable accessor

	VariableAccessor::VariableAccessor(ScriptScopeBase* scope, size_t addr)
		: scope(scope)
		, addr(addr)
	{
	}

	VariableAccessor::VariableAccessor(const VariableAccessor& b)
		: scope(b.scope)
		, addr(b.addr)
	{
	}

	VariableAccessor::VariableAccessor() = default;

	VariableAccessor::~VariableAccessor() = default;

	std::unique_ptr<VariableAccessor> VariableAccessor::clone() const
	{
		return std::make_unique<VariableAccessor>(*this);
	}

	std::unique_ptr<VariableAccessor> VariableAccessor::find(ScriptScopeBase& scope, const std::string& name, bool searchParents)
	{
		auto addr = scope.findVariableAddr(name, searchParents);

		if (!addr)
			throw CompileException("Variable \"" + name + "\" not found");

		return std::make_unique<VariableAccessor>(addr.scope, addr.addr);
	}

	ScriptVariable* VariableAccessor::get(ScriptScopeBase* scope)
	{
		assert(this->scope && "Scope must not be null");

		auto usedScope = this->scope->isBaseOf(*scope) ? scope : this->scope;
		usedScope = &RemapScope::map(*this->scope);

		if (this->addr > usedScope->getVariables().size())
			return nullptr;
		return usedScope->getVariables().at(this->addr).get();
	}

	const std::shared_ptr<ScriptType> VariableAccessor::getType() const
	{
		assert(this->scope && "Scope must not be null");

		if (this->addr > this->scope->getVariables().size())
			return nullptr;
		return this->scope->getVariables().at(this->addr)->getType();
	}

	void VariableAccessor::setScope(ScriptScopeBase* scope)
	{
		this->scope = scope;
	}

	ScriptScopeBase* VariableAccessor::getScope()
	{
		return this->scope;
	}

	VariableAccessor::operator bool() const
	{
		return this->scope && this->addr != NullAddr;
	}

	// Parameter accessor

	std::unique_ptr<VariableAccessor> ParameterAccessor::clone() const
	{
		return std::make_unique<ParameterAccessor>(*this);
	}

	std::unique_ptr<ParameterAccessor> ParameterAccessor::find(ScriptScopeBase& scope, const std::string& name, bool searchParents)
	{
		//assert(dynamic_cast<ScriptFunction*>(&scope) && "Parameters may only be used in functions");
		ScriptFunction* fnc = scope.getClosestFunction(true);
		//ScriptFunction& fnc = static_cast<ScriptFunction&>(scope);

		auto addr = fnc->findParamAddr(name);

		if (!addr)
			throw CompileException("Variable \"" + name + "\" not found");

		return std::make_unique<ParameterAccessor>(addr.scope, addr.addr);
	}

	ScriptVariable* ParameterAccessor::get(ScriptScopeBase* scope)
	{
		assert(this->scope && "Scope must not be null");
		assert(dynamic_cast<ScriptFunction*>(this->scope) && "Parameters may only be used in functions");

		//scope = this->scope->isBaseOf(*scope) ? scope : this->scope;
		auto usedScope = &RemapScope::map(*this->scope);

		ScriptFunction* fnc = static_cast<ScriptFunction*>(usedScope);

		if (this->addr > fnc->getParameters().size())
			return nullptr;
		return &fnc->getParameters().at(this->addr);
	}

	const std::shared_ptr<ScriptType> ParameterAccessor::getType() const
	{
		assert(this->scope && "Scope must not be null");
		assert(dynamic_cast<ScriptFunction*>(this->scope) && "Parameters may only be used in functions");

		ScriptFunction* fnc = static_cast<ScriptFunction*>(this->scope);

		if (this->addr > this->scope->getVariables().size())
			return nullptr;
		return fnc->getParameters().at(this->addr).getType();
	}

	// Function accessor

	FunctionAccessor::FunctionAccessor() = default;

	FunctionAccessor::FunctionAccessor(ScriptScopeBase* scope, size_t addr)
		: scope(scope)
		, addr(addr)
	{
	}
	FunctionAccessor::FunctionAccessor(const FunctionAccessor& b)
		: scope(b.scope)
		, addr(b.addr)
	{
	}

	FunctionAccessor FunctionAccessor::find(ScriptScopeBase& scope, const std::string& name, const PARAMS_T& params, bool searchParents)
	{
		auto addr = scope.findFunctionAddr(name, params, searchParents);

		if (!addr)
			throw CompileException("Function \"" + name + "\" not found");

		return FunctionAccessor(addr.scope, addr.addr);
	}

	ScriptFunction* FunctionAccessor::get()
	{
		assert(this->scope && "Scope must not be null");

		if (this->addr > this->scope->getFunctions().size())
			return nullptr;
		return this->scope->getFunctions().at(this->addr).get();
	}

	const std::shared_ptr<ScriptType> FunctionAccessor::getType() const
	{
		assert(this->scope && "Scope must not be null");

		if (this->addr > this->scope->getFunctions().size())
			return nullptr;
		return this->scope->getFunctions().at(this->addr)->getType();
	}

	void FunctionAccessor::setScope(ScriptScopeBase* scope)
	{
		this->scope = scope;
	}

	FunctionAccessor::operator bool() const
	{
		return this->scope && this->addr != NullAddr;
	}
}