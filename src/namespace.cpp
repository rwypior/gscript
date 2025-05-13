#include <iostream>

#include "pNamespace.hpp"
#include "namespace.hpp"
#include "pClass.hpp"
#include "compileException.hpp"
#include "script.hpp"
#include "varDeclaration.hpp"
#include "class.hpp"

#include "scriptExtension.hpp"

namespace gscript
{
	ScriptNamespace::ScriptNamespace(ScriptScope *parentScope)
		: ScriptScope(parentScope)
	{
	}

	ScriptNamespace::ScriptNamespace(ScriptScope *parentScope, const std::string &name)
		: ScriptScope(parentScope),
		name(name)
	{
	}

	ScriptNamespace::ScriptNamespace(ScriptScope *parentScope, const ParserNamespace &ns)
		: ScriptScope(parentScope),
		name(ns.name)
	{
		for (ParserNamespace::CLASS_CONTAINER_T::const_iterator it = ns.classes.begin(); it != ns.classes.end(); ++it)
		{
			this->registerClassPrototype(*it);
		}

		this->resolveClasses();

		for (ParserNamespace::FUNCTION_CONTAINER_T::const_iterator it = ns.functions.begin(); it != ns.functions.end(); ++it)
		{
			this->registerFunctionPrototype(*it);
		}

		this->resolveFunctions();
		this->resolveClassMembers();
	}

	const std::string &ScriptNamespace::getName() const
	{
		return this->name;
	}

	ScriptClass &ScriptNamespace::registerClassPrototype(const ParserClass &pclass)
	{
		const std::string &base = pclass.base;

		if (ScriptClass *existing = this->findClass(pclass.name))
			return *existing;

		ScriptClass *baseClass = NULL;

		if (base.length() > 0)
		{
			baseClass = this->findClass(base);
		}

		this->classes.push_back(new ScriptClass(*this, pclass.name, baseClass));
		ScriptClass *cl = this->classes.back();
		cl->parentScope = this;

		ScriptClassPrototype *proto = new ScriptClassPrototype(*cl, pclass);
		this->classPrototypes.push_back(proto);

		return *cl;
	}

	ScriptClass &ScriptNamespace::registerClass(const ParserClass &pclass)
	{
		const std::string &base = pclass.base;

		if (ScriptClass *existing = this->findClass(pclass.name))
			return *existing;

		ScriptClass *baseClass = NULL;

		if (base.length() > 0)
		{
			baseClass = this->findClass(base);
		}

		this->classes.push_back(new ScriptClass(*this, pclass.name, baseClass));
		ScriptClass *cl = this->classes.back();
		cl->parentScope = this;

		for (std::vector<ParserFieldDeclaration>::const_iterator it = pclass.fields.begin(); it != pclass.fields.end(); ++it)
		{
			ScriptVariable &svar = this->registerVariable(it->name, ScriptType::create(it->type, *this), NULL);
			ScriptVarDeclaration *svd = new ScriptVarDeclaration(*this, svar, ScriptStatement(*this, it->value));
			cl->addVarDeclaration(svd);

			//cl->registerVariable(it->name, ScriptType::create(it->type), NULL);
		}

		for (std::vector<ParserMethod>::const_iterator it = pclass.methods.begin(); it != pclass.methods.end(); ++it)
		{
			cl->registerFunction(*it);
		}

		return *cl;
	}

	void ScriptNamespace::registerClass(ScriptClass *sclass)
	{
		sclass->parentScope = this;
		this->classes.push_back(sclass);
	}

	ScriptNamespace &ScriptNamespace::registerNamespace(const ParserNamespace &ns)
	{
		this->namespaces.push_back(new ScriptNamespace(this, ns));
		return *this->namespaces.back();
	}

	ScriptNamespace &ScriptNamespace::registerNamespace(ScriptNamespace *ns)
	{
		this->namespaces.push_back(ns);
		return *this->namespaces.back();
	}

	ScriptClass *ScriptNamespace::findClass(const std::string &name)
	{
		for (ScriptNamespace::CLASS_CONTAINER_T::iterator it = this->classes.begin(); it != this->classes.end(); ++it)
		{
			ScriptClass *c = *it;
			if (c->getName() == name && !dynamic_cast<ScriptClassPrototype*>(c))
				return c;
		}

		return NULL;
	}

	ScriptClass *ScriptNamespace::findClassByInheritance(const std::string &name)
	{
		for (ScriptNamespace::CLASS_CONTAINER_T::iterator it = this->classes.begin(); it != this->classes.end(); ++it)
		{
			ScriptClass *c = *it;
			if (c->getBase() && c->getBase()->getName() == name)
				return c;
		}

		return NULL;
	}

	ScriptNamespace *ScriptNamespace::findNamespace(const std::string &name)
	{
		for (ScriptNamespace::NAMESPACE_CONTAINER_T::iterator it = this->namespaces.begin(); it != this->namespaces.end(); ++it)
		{
			ScriptNamespace *c = *it;
			if (c->name == name)
				return c;
		}

		return NULL;
	}

	void ScriptNamespace::resolveFunctions()
	{
		for (ScriptScope::FUNCTION_PROTOTYPE_CONTAINER::iterator it = this->functionPrototypes.begin(); it != this->functionPrototypes.end(); ++it)
		{
			(*it)->build();
		}

		this->functionPrototypes.clear();

		for (ScriptScope::FUNCTION_CONTAINER::iterator it = this->functions.begin(); it != this->functions.end(); ++it)
		{
			if (ScriptExtension::ExtensionFunction *extf = dynamic_cast<ScriptExtension::ExtensionFunction*>(*it))
			{
				extf->setup();
			}
		}
	}

	void ScriptNamespace::resolveClasses()
	{
		for (ScriptNamespace::CLASS_PROTOTYPE_CONTAINER_T::iterator it = this->classPrototypes.begin(); it != this->classPrototypes.end(); ++it)
		{
			(*it)->build();
		}

		this->classPrototypes.clear();

		for (ScriptNamespace::CLASS_CONTAINER_T::iterator it = this->classes.begin(); it != this->classes.end(); ++it)
		{
			for (ScriptScope::FUNCTION_CONTAINER::iterator it2 = (*it)->functions.begin(); it2 != (*it)->functions.end(); ++it2)
			{
				if (ScriptExtension::ExtensionFunction *extf = dynamic_cast<ScriptExtension::ExtensionFunction*>(*it2))
				{
					extf->setup();
				}
			}
		}
	}

	void ScriptNamespace::resolveClassMembers()
	{
		for (ScriptNamespace::CLASS_CONTAINER_T::iterator it = this->classes.begin(); it != this->classes.end(); ++it)
		{
			ScriptClass *c = *it;

			for (ScriptScope::FUNCTION_PROTOTYPE_CONTAINER::iterator it2 = c->functionPrototypes.begin(); it2 != c->functionPrototypes.end(); ++it2)
			{
				(*it2)->build();
			}

			c->functionPrototypes.clear();
		}
	}
}