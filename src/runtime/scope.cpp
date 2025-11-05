#include "gscript/runtime/scope.hpp"
#include "gscript/runtime/function.hpp"
#include "gscript/runtime/class.hpp"
#include "gscript/runtime/namespace.hpp"
#include "gscript/runtime/function.hpp"
#include "gscript/type.hpp"
#include "gscript/defs.hpp"
#include "gscript/EntityPath.hpp"
#include "gscript/compileException.hpp"

namespace gscript
{
	// Scope base

	ScriptScopeBase::ScriptScopeBase() = default;
	ScriptScopeBase::~ScriptScopeBase() = default;

	ScriptFunction& ScriptScopeBase::registerFunction(const std::string& name,
		std::shared_ptr<ScriptType> returnType,
		const PARAMS_T& parameters,
		std::vector<std::unique_ptr<ScriptCallable>>&& statements)
	{
		return this->registerFunction(std::make_unique<ScriptFunction>(*this, name, returnType, parameters, std::move(statements)));
	}

	ScriptFunction& ScriptScopeBase::registerFunction(std::unique_ptr<ScriptFunction>&& function)
	{
		this->getFunctions().push_back(std::move(function));
		return *this->getFunctions().back();
	}

	ScriptVariable& ScriptScopeBase::registerVariable(const std::string& name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptValue>&& value)
	{
		return this->registerVariable(std::make_unique<ScriptVariable>(name, type, std::move(value)));
	}

	ScriptVariable& ScriptScopeBase::registerVariable(const std::string& name, const std::shared_ptr<ScriptType> type, const std::unique_ptr<ScriptValue>& value)
	{
		return this->registerVariable(std::make_unique<ScriptVariable>(name, type, value->clone()));
	}

	ScriptVariable& ScriptScopeBase::registerVariable(std::unique_ptr<ScriptVariable>&& variable)
	{
		auto& variables = this->getVariables();
		variables.push_back(std::move(variable));
		return *variables.back();
	}

	// TODO - merge those two functions into one
	// TODO 2 - make this function accept a collection of ValueType enums

	ScriptFunction* ScriptScopeBase::findFunction(const std::string &name, const PARAMS_T params, bool searchParents, bool searchBase) const
	{
		auto& functions = this->getFunctions();

		auto it = std::find_if(functions.begin(), functions.end(), [&name, &params](const std::unique_ptr<ScriptFunction>& fnc) {
			return fnc->matches(name, params);
		});
		if (it != functions.end())
			return it->get();

		if (searchBase)
		{
			if (const ScriptClass* sc = dynamic_cast<const ScriptClass*>(this))
			{
				if (ScriptClass* base = sc->getBase())
				{
					if (ScriptFunction* fnc = base->findFunction(name, params))
						return fnc;
				}
			}
		}

		if (searchParents)
		{
			if (auto parentScope = this->getParentScope())
				return parentScope->findFunction(name, params);
		}

		return nullptr;
	}

	ScopedAddress ScriptScopeBase::findFunctionAddr(const std::string& name, const PARAMS_T params, bool searchParents, bool searchBase)
	{
		auto& functions = this->getFunctions();

		auto it = std::find_if(functions.begin(), functions.end(), [&name, &params](const std::unique_ptr<ScriptFunction>& fnc) {
			return fnc->matches(name, params);
		});
		if (it != functions.end())
			return ScopedAddress(this, it - functions.begin());

		if (searchBase)
		{
			if (const ScriptClass* sc = dynamic_cast<const ScriptClass*>(this))
			{
				if (ScriptClass* base = sc->getBase())
				{
					if (ScopedAddress fnc = base->findFunctionAddr(name, params))
						return fnc;
				}
			}
		}

		if (searchParents)
		{
			if (auto parentScope = this->getParentScope())
				return parentScope->findFunctionAddr(name, params);
		}

		return nullptr;
	}

	ScriptFunction* ScriptScopeBase::getFunction(const std::string &name, const PARAMS_T params, bool searchParents, bool searchBase) const
	{
		if (ScriptFunction *f = this->findFunction(name, params, searchParents, searchBase))
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

	ScriptFunction* ScriptScopeBase::getClosestFunction(bool includeSelf)
	{
		if (ScriptFunction* fnc = dynamic_cast<ScriptFunction*>(this))
		{
			if (includeSelf)
				return fnc;
		}

		if (auto* parentScope = this->getParentScope())
		{
			if (ScriptFunction* fnc = parentScope->getClosestFunction(true))
				return fnc;
		}

		return nullptr;
	}

	bool ScriptScopeBase::isAccessible(const ScriptScopeBase& targetScope, Modifier access) const
	{
		if (access & Modifier::AccessPublic)
			return true;

		if (access & Modifier::AccessPrivate && &targetScope == this)
			return true;

		if (access & Modifier::AccessProtected)
		{
			const ScriptClass *thisClass = dynamic_cast<const ScriptClass*>(this);
			const ScriptClass *targetClass = dynamic_cast<const ScriptClass*>(&targetScope);
			if (thisClass && thisClass->isBaseOf(targetClass))
				return true;
		}

		return false;
	}

	bool ScriptScopeBase::isParentOf(ScriptScopeBase& scope, bool includeGrandparents) const
	{
		if (this == scope.getParentScope())
		{
			return true;
		}

		if (includeGrandparents)
		{
			if (auto parent = scope.getParentScope())
				return this->isParentOf(*parent, includeGrandparents);
		}

		return false;
	}

	// Scope

	ScriptScope::ScriptScope(const ScriptScope& b)
		: parentScope(b.getParentScope())
	{
		for (auto& var : b.getVariables())
		{
			this->variables.push_back(std::make_unique<ScriptVariable>(*var));
		}
	}

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

	// Rescope

	std::stack<std::pair<ScriptScopeBase*, ScriptScopeBase*>> RemapScope::mappingQueue;

	RemapScope::RemapScope(ScriptScopeBase& source, ScriptScopeBase& target)
	{
		mappingQueue.push({&source, &target});
	}

	RemapScope::~RemapScope()
	{
		mappingQueue.pop();
	}

	ScriptScopeBase& RemapScope::map(ScriptScopeBase& source)
	{
		if (mappingQueue.empty() || mappingQueue.top().first != &source)
			return source;

		return *mappingQueue.top().second;
	}
}