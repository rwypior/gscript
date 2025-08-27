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
	ScriptScope::ScriptScope(ScriptScope *parentScope)
		:parentScope(parentScope)
	{ }

	/*ScriptFunction &ScriptScope::registerFunction(const ParserFunction &c)
	{
		PARAMS_T params;
		for (
			ParserArglist::ARGLIST_PARAMS_T::const_iterator it = c.arglist.parameters.begin();
			it != c.arglist.parameters.end();
			++it
			)
		{
			params.emplace_back(ScriptType::create(it->type, *this), it->name);
		}

		this->functions.push_back(
			new ScriptFunction(*this, c.name, ScriptType::create(c.returnTypeName, *this), params)
		);
		ScriptFunction &newfunc = *this->functions.back();

		newfunc.parentScope = this;
		newfunc.setup(c);

		return newfunc;
	}

	void ScriptScope::registerFunction(ScriptFunction *c)
	{
		c->parentScope = this;
		this->functions.push_back(c);
	}

	ScriptFunction &ScriptScope::registerFunctionPrototype(const ParserFunction &c)
	{
		PARAMS_T params;
		for (
			ParserArglist::ARGLIST_PARAMS_T::const_iterator it = c.arglist.parameters.begin();
			it != c.arglist.parameters.end();
			++it
			)
		{
			params.emplace_back(ScriptType::create(it->type, *this), it->name);
		}

		if (c.externName.empty())
			this->functions.push_back(
				new ScriptFunction(*this, c.name, ScriptType::create(c.returnTypeName, *this), params)
			);
		else
			this->functions.push_back(
				new ScriptExternFunctionConnector(c.externName, *this, c.name, ScriptType::create(c.returnTypeName, *this), params)
			);

		ScriptFunction &newfunc = *this->functions.back();

		newfunc.parentScope = this;

		ScriptFunctionPrototype *proto = new ScriptFunctionPrototype(newfunc, c);
		this->functionPrototypes.push_back(proto);

		return newfunc;
	}

	ScriptVariable &ScriptScope::registerVariable(const ParserVarDeclaration &pvar, ScriptValue *value)
	{
		return this->registerVariable(pvar.name, ScriptType::create(pvar.type, *this), value);
	}*/


	void ScriptScope::registerFunction(std::unique_ptr<ScriptFunction>&& function)
	{
		this->functions.push_back(std::move(function));
	}

	ScriptVariable& ScriptScope::registerVariable(const std::string &name, const ScriptType *type, ScriptValue *value)
	{
		return this->registerVariable(std::make_unique<ScriptVariable>(name, type, nullptr, this->getVariables().size()));
	}

	ScriptVariable& ScriptScope::registerVariable(std::unique_ptr<ScriptVariable>&& variable)
	{
		this->variables.push_back(std::move(variable));
		return *this->variables.back();
	}

	ScriptFunction *ScriptScope::findFunction(const std::string &name, const PARAMS_T params) const
	{
		auto it = std::find_if(this->functions.begin(), this->functions.end(), [&name, &params](const std::unique_ptr<ScriptFunction>& fnc) {
			return fnc->matches(name, params);
		});
		if (it != this->functions.end())
			return it->get();

		if (const ScriptClass* sc = dynamic_cast<const ScriptClass*>(this))
		{
			if (ScriptClass* base = sc->getBase())
			{
				if (ScriptFunction* fnc = base->findFunction(name, params))
					return fnc;
			}
		}

		if (this->parentScope)
			return this->parentScope->findFunction(name, params);

		return nullptr;
	}

	ScriptFunction *ScriptScope::getFunction(const std::string &name, const PARAMS_T params) const
	{
		if (ScriptFunction *f = this->findFunction(name, params))
			return f;

		throw CompileException("Function \"" + name + "\" could not be found");
	}

	ScriptVariable *ScriptScope::findVariable(const std::string &name)
	{
		auto it = std::find_if(this->variables.begin(), this->variables.end(), [&name](const std::unique_ptr<ScriptVariable>& v) {
			return v->getName() == name;
		});
		if (it != this->variables.end())
			return it->get();

		if (this->parentScope)
			this->parentScope->findVariable(name);

		return nullptr;
	}

	ScriptVariable *ScriptScope::getVariable(const std::string &name)
	{
		if (ScriptVariable *v = this->findVariable(name))
			return v;

		throw CompileException("Variable \"" + name + "\" could not be found");
	}

	std::vector<std::unique_ptr<ScriptVariable>>& ScriptScope::getVariables()
	{
		return this->variables;
	}

	ScriptNamespace *ScriptScope::getClosestNamespace(bool includeSelf)
	{
		if (ScriptNamespace* ns = dynamic_cast<ScriptNamespace*>(this))
		{
			if (includeSelf)
				return ns;
		}

		if (this->parentScope)
		{
			if (ScriptNamespace *ns = this->parentScope->getClosestNamespace(true))
				return ns;
		}

		return nullptr;
	}

	ScriptNamespace* ScriptScope::getGlobalNamespace()
	{
		if (this->parentScope)
		{
			auto parentNamespace = this->parentScope->getGlobalNamespace();
			if (ScriptNamespace* ns = dynamic_cast<ScriptNamespace*>(parentNamespace))
				return ns;
		}

		if (ScriptNamespace* ns = dynamic_cast<ScriptNamespace*>(this))
			return ns;

		return nullptr;
	}

	void ScriptScope::setParentScope(ScriptScope *scope)
	{
		this->parentScope = scope;
	}

	ScriptScope *ScriptScope::getParentScope() const
	{
		return this->parentScope;
	}

	bool ScriptScope::isAccessible(ScriptScope &targetScope, MODIFIER_T access)
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

	/*ScriptClass *ScriptScope::findClass(const EntityPath &path)
	{
		EntityPath::scope_iterator it = path.createScopeIterator();

		for (; it != path.getString().end(); ++it)
		{

		}
	}*/
}