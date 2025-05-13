#include <iostream>

#include "scope.hpp"
#include "pFunction.hpp"
#include "function.hpp"
#include "class.hpp"
#include "pVarDeclaration.hpp"
#include "namespace.hpp"
#include "globalNamespace.hpp"
#include "EntityPath.hpp"
#include "pFuncCall.hpp"
#include "function.hpp"
#include "type.hpp"
#include "pVar.hpp"
#include "defs.hpp"

#include "compileException.hpp"

namespace gscript
{
	ScriptScope::ScriptScope(ScriptScope *parentScope)
		:parentScope(parentScope)
	{ }

	ScriptFunction &ScriptScope::registerFunction(const ParserFunction &c)
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
	}

	ScriptVariable &ScriptScope::registerVariable(const std::string &name, const ScriptType *type, ScriptValue *value)
	{
		this->variables.emplace_back(name, type, value, this->variables.size());
		ScriptVariable &svar = this->variables.back();
		return svar;
	}

	ScriptFunction *ScriptScope::findFunction(const std::string &name, const PARAMS_T params) const
	{
		for (
			ScriptScope::FUNCTION_CONTAINER::const_iterator it = this->functions.begin();
			it != this->functions.end();
			++it
			)
		{
			if ((*it)->matches(name, params))
				return &(*(*it));
		}

		if (const ScriptClass *sc = dynamic_cast<const ScriptClass*>(this))
		{
			if (ScriptClass *base = sc->getBase())
				if (ScriptFunction *f = base->findFunction(name, params))
					return f;
		}

		if (this->parentScope)
			return this->parentScope->findFunction(name, params);

		return NULL;
	}

	ScriptFunction *ScriptScope::getFunction(const std::string &name, const PARAMS_T params) const
	{
		if (ScriptFunction *f = this->findFunction(name, params))
			return f;

		throw CompileException("Function \"" + name + "\" could not be found");
	}

	ScriptVariable *ScriptScope::findVariable(const std::string &name)
	{
		for (
			ScriptScope::VARIABLE_CONTAINER::iterator it = this->variables.begin();
			it != this->variables.end();
			++it
			)
		{
			if (it->getName() == name)
				return &(*it);
		}

		if (this->parentScope)
			return this->parentScope->findVariable(name);

		return NULL;
	}

	ScriptVariable *ScriptScope::getVariable(const std::string &name)
	{
		if (ScriptVariable *v = this->findVariable(name))
			return v;

		throw CompileException("Variable \"" + name + "\" could not be found");
	}

	ScriptScope::VARIABLE_CONTAINER &ScriptScope::getVariables()
	{
		return this->variables;
	}

	ScriptNamespace *ScriptScope::getClosestNamespace(bool includeSelf)
	{
		if (ScriptNamespace *ns = dynamic_cast<ScriptNamespace*>(this))
			if (includeSelf)
				return ns;

		if (this->parentScope)
			if (ScriptNamespace *ns = this->parentScope->getClosestNamespace(true))
				return ns;

		return NULL;
	}

	ScriptGlobalNamespace *ScriptScope::getGlobalNamespace()
	{
		if (ScriptGlobalNamespace *ns = dynamic_cast<ScriptGlobalNamespace*>(this))
			return ns;

		if (this->parentScope)
			return this->parentScope->getGlobalNamespace();

		return NULL;
	}

	void ScriptScope::setParentScope(ScriptScope *scope)
	{
		this->parentScope = scope;
	}

	ScriptScope *ScriptScope::getParentScope() const
	{
		return this->parentScope;
	}

	bool ScriptScope::isAccessible(ScriptScope &targetScope, BITFLAG_T access)
	{
		if (access & static_cast<int>(MODIFIER_T::M_ACCESS_PUBLIC))
			return true;

		if (access & static_cast<int>(MODIFIER_T::M_ACCESS_PRIVATE) && &targetScope == this)
			return true;

		if (access & static_cast<int>(MODIFIER_T::M_ACCESS_PROTECTED))
		{
			ScriptClass *thisClass = dynamic_cast<ScriptClass*>(this);
			ScriptClass *targetClass = dynamic_cast<ScriptClass*>(&targetScope);
			if (thisClass && thisClass->isBaseOf(targetClass))
				return true;
		}

		return false;
	}

	ScriptExternFunction &ScriptScope::getExternFunction(const std::string &name)
	{
		if (ScriptGlobalNamespace *global = this->getGlobalNamespace())
			if (ScriptExternFunction *ext = global->findExternFunction(name))
				return *ext;

		throw CompileException(std::string("Extern function \"" + name + "\" not found"));
	}

	/*ScriptClass *ScriptScope::findClass(const EntityPath &path)
	{
		EntityPath::scope_iterator it = path.createScopeIterator();

		for (; it != path.getString().end(); ++it)
		{

		}
	}*/
}