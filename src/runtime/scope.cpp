#include "runtime/scope.hpp"
#include "runtime/function.hpp"
#include "runtime/class.hpp"
#include "runtime/namespace.hpp"
#include "runtime/function.hpp"
#include "type.hpp"
#include "defs.hpp"
#include "EntityPath.hpp"
#include "compileException.hpp"

namespace gscript
{
	// Scope base

	ScriptFunction& ScriptScopeBase::registerFunction(const std::string& name,
		ScriptType* returnType,
		const PARAMS_T& parameters,
		std::vector<std::shared_ptr<ScriptCallable>>&& statements)
	{
		return this->registerFunction(std::make_unique<ScriptFunction>(*this, name, returnType, parameters, std::move(statements)));
	}

	ScriptFunction& ScriptScopeBase::registerFunction(std::unique_ptr<ScriptFunction>&& function)
	{
		this->getFunctions().push_back(std::move(function));
		return *this->getFunctions().back();
	}

	ScriptVariable& ScriptScopeBase::registerVariable(const std::string &name, const ScriptType *type, ScriptValue *value)
	{
		return this->registerVariable(std::make_unique<ScriptVariable>(name, type, value));
	}

	ScriptVariable& ScriptScopeBase::registerVariable(std::unique_ptr<ScriptVariable>&& variable)
	{
		auto& variables = this->getVariables();
		variables.push_back(std::move(variable));
		return *variables.back();
	}

	ScriptFunction * ScriptScopeBase::findFunction(const std::string &name, const PARAMS_T params) const
	{
		auto& functions = this->getFunctions();

		auto it = std::find_if(functions.begin(), functions.end(), [&name, &params](const std::unique_ptr<ScriptFunction>& fnc) {
			return fnc->matches(name, params);
		});
		if (it != functions.end())
			return it->get();

		if (const ScriptClass* sc = dynamic_cast<const ScriptClass*>(this))
		{
			if (ScriptClass* base = sc->getBase())
			{
				if (ScriptFunction* fnc = base->findFunction(name, params))
					return fnc;
			}
		}

		if (auto parentScope = this->getParentScope())
			return parentScope->findFunction(name, params);

		return nullptr;
	}

	ScopedAddress ScriptScopeBase::findFunctionAddr(const std::string& name, const PARAMS_T params, bool searchParents)
	{
		auto& functions = this->getFunctions();

		auto it = std::find_if(functions.begin(), functions.end(), [&name, &params](const std::unique_ptr<ScriptFunction>& fnc) {
			return fnc->matches(name, params);
		});
		if (it != functions.end())
			return ScopedAddress(this, it - functions.begin());

		if (const ScriptClass* sc = dynamic_cast<const ScriptClass*>(this))
		{
			if (ScriptClass* base = sc->getBase())
			{
				if (ScopedAddress fnc = base->findFunctionAddr(name, params))
					return fnc;
			}
		}

		if (searchParents)
		{
			if (auto parentScope = this->getParentScope())
				return parentScope->findFunction(name, params);
		}

		return nullptr;
	}

	ScriptFunction* ScriptScopeBase::getFunction(const std::string &name, const PARAMS_T params) const
	{
		if (ScriptFunction *f = this->findFunction(name, params))
			return f;

		return nullptr;
	}

	ScriptVariable* ScriptScopeBase::findVariable(const std::string &name)
	{
		auto& variables = this->getVariables();

		auto it = std::find_if(variables.begin(), variables.end(), [&name](const std::unique_ptr<ScriptVariable>& v) {
			return v->getName() == name;
		});
		if (it != variables.end())
			return it->get();

		if (auto* parentScope = this->getParentScope())
			return parentScope->findVariable(name);

		return nullptr;
	}

	ScopedAddress ScriptScopeBase::findVariableAddr(const std::string& name, bool searchParents)
	{
		auto& variables = this->getVariables();

		auto it = std::find_if(variables.begin(), variables.end(), [&name](const std::unique_ptr<ScriptVariable>& v) {
			return v->getName() == name;
			});
		if (it != variables.end())
			return ScopedAddress(this, it - variables.begin());

		if (searchParents)
		{
			if (auto* parentScope = this->getParentScope())
				return parentScope->findVariableAddr(name);
		}

		return {};
	}

	ScriptVariable* ScriptScopeBase::getVariable(const std::string &name)
	{
		if (ScriptVariable *v = this->findVariable(name))
			return v;

		return nullptr;
	}

	ScriptNamespace* ScriptScopeBase::getClosestNamespace(bool includeSelf)
	{
		if (ScriptNamespace* ns = dynamic_cast<ScriptNamespace*>(this))
		{
			if (includeSelf)
				return ns;
		}

		if (auto* parentScope = this->getParentScope())
		{
			if (ScriptNamespace *ns = parentScope->getClosestNamespace(true))
				return ns;
		}

		return nullptr;
	}

	ScriptNamespace* ScriptScopeBase::getGlobalNamespace()
	{
		if (auto* parentScope = this->getParentScope())
		{
			auto parentNamespace = parentScope->getGlobalNamespace();
			if (ScriptNamespace* ns = dynamic_cast<ScriptNamespace*>(parentNamespace))
				return ns;
		}

		if (ScriptNamespace* ns = dynamic_cast<ScriptNamespace*>(this))
			return ns;

		return nullptr;
	}

	bool ScriptScopeBase::isAccessible(ScriptScopeBase& targetScope, MODIFIER_T access)
	{
		if (access & MODIFIER_T::M_ACCESS_PUBLIC)
			return true;

		if (access & MODIFIER_T::M_ACCESS_PRIVATE && &targetScope == this)
			return true;

		if (access & MODIFIER_T::M_ACCESS_PROTECTED)
		{
			ScriptClass *thisClass = dynamic_cast<ScriptClass*>(this);
			ScriptClass *targetClass = dynamic_cast<ScriptClass*>(&targetScope);
			if (thisClass && thisClass->isBaseOf(targetClass))
				return true;
		}

		return false;
	}

	// Scope

	ScriptScope::ScriptScope(ScriptScopeBase* parentScope)
		: parentScope(parentScope)
	{
	}

	std::vector<std::unique_ptr<ScriptVariable>>& ScriptScope::getVariables()
	{
		return this->variables;
	}

	const std::vector<std::unique_ptr<ScriptVariable>>& ScriptScope::getVariables() const
	{
		return this->variables;
	}

	std::vector<std::unique_ptr<ScriptFunction>>& ScriptScope::getFunctions()
	{
		return this->functions;
	}

	const std::vector<std::unique_ptr<ScriptFunction>>& ScriptScope::getFunctions() const
	{
		return this->functions;
	}

	void ScriptScope::setParentScope(ScriptScopeBase* scope)
	{
		this->parentScope = scope;
	}

	ScriptScopeBase* ScriptScope::getParentScope() const
	{
		return this->parentScope;
	}
}