#include "runtime/namespace.hpp"
#include "runtime/varDeclaration.hpp"
#include "runtime/class.hpp"
#include "extension/scriptExtension.hpp"
#include "compileException.hpp"

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

	const std::string &ScriptNamespace::getName() const
	{
		return this->name;
	}

	//ScriptClass &ScriptNamespace::registerClassPrototype(const ParserClass &pclass)
	//{
	//	const std::string &base = pclass.base;

	//	if (ScriptClass *existing = this->findClass(pclass.name))
	//		return *existing;

	//	ScriptClass *baseClass = NULL;

	//	if (base.length() > 0)
	//	{
	//		baseClass = this->findClass(base);
	//	}

	//	this->classes.push_back(new ScriptClass(*this, pclass.name, baseClass));
	//	ScriptClass *cl = this->classes.back();
	//	cl->parentScope = this;

	//	ScriptClassPrototype *proto = new ScriptClassPrototype(*cl, pclass);
	//	this->classPrototypes.push_back(proto);

	//	return *cl;
	//}

	//ScriptClass &ScriptNamespace::registerClass(const ParserClass &pclass)
	//{
	//	const std::string &base = pclass.base;

	//	if (ScriptClass *existing = this->findClass(pclass.name))
	//		return *existing;

	//	ScriptClass *baseClass = NULL;

	//	if (base.length() > 0)
	//	{
	//		baseClass = this->findClass(base);
	//	}

	//	this->classes.push_back(new ScriptClass(*this, pclass.name, baseClass));
	//	ScriptClass *cl = this->classes.back();
	//	cl->parentScope = this;

	//	for (std::vector<ParserFieldDeclaration>::const_iterator it = pclass.fields.begin(); it != pclass.fields.end(); ++it)
	//	{
	//		ScriptVariable &svar = this->registerVariable(it->name, ScriptType::create(it->type, *this), NULL);
	//		ScriptVarDeclaration *svd = new ScriptVarDeclaration(*this, svar, ScriptStatement(*this, it->value));
	//		cl->addVarDeclaration(svd);

	//		//cl->registerVariable(it->name, ScriptType::create(it->type), NULL);
	//	}

	//	for (std::vector<ParserMethod>::const_iterator it = pclass.methods.begin(); it != pclass.methods.end(); ++it)
	//	{
	//		cl->registerFunction(*it);
	//	}

	//	return *cl;
	//}

	ScriptClass& ScriptNamespace::registerClass(std::unique_ptr<ScriptClass> &&sclass)
	{
		sclass->parentScope = this;
		this->classes.push_back(std::move(sclass));
		//this->classes.at(sclass->getName()) = std::move(sclass);
		//this->classes.push_back(std::move(sclass));
		return *this->classes.back();
	}

	/*ScriptNamespace &ScriptNamespace::registerNamespace(const ParserNamespace &ns)
	{
		this->namespaces.push_back(new ScriptNamespace(this, ns));
		return *this->namespaces.back();
	}*/

	ScriptNamespace &ScriptNamespace::registerNamespace(std::unique_ptr<ScriptNamespace> &&ns)
	{
		this->namespaces.push_back(std::move(ns));
		return *this->namespaces.back();
		/*std::string name = ns->name;
		this->namespaces[name] = std::move(ns);
		return *this->namespaces.at(name);*/
	}

	ScriptClass *ScriptNamespace::findClass(const std::string &name)
	{
		auto it = std::find_if(this->classes.begin(), this->classes.end(), [&name](const std::unique_ptr<ScriptClass>& c) {
			return c->getName() == name;
		});
		if (it == this->classes.end())
			return nullptr;

		return it->get();
	}

	ScriptClass *ScriptNamespace::findClassByInheritance(const std::string &name)
	{
		auto it = std::find_if(this->classes.begin(), this->classes.end(), [&name](const std::unique_ptr<ScriptClass>& cls) {
			return cls->getBase() && cls->getBase()->getName() == name;
		});
		if (it == this->classes.end())
			return nullptr;

		return it->get();
	}

	ScriptNamespace *ScriptNamespace::findNamespace(const std::string &name)
	{
		auto it = std::find_if(this->namespaces.begin(), this->namespaces.end(), [&name](const std::unique_ptr<ScriptNamespace>& ns) {
			return ns->getName() == name;
		});
		if (it == this->namespaces.end())
			return nullptr;

		return it->get();
	}

	void ScriptNamespace::resolveFunctions()
	{
		/*for (ScriptScope::FUNCTION_PROTOTYPE_CONTAINER::iterator it = this->functionPrototypes.begin(); it != this->functionPrototypes.end(); ++it)
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
		}*/
	}

	/*void ScriptNamespace::resolveClasses()
	{
		for (auto& cls : this->classPrototypes)
		{
			cls.second;
			cls->build();
		}

		this->classPrototypes.clear();

		for (auto& pcls : this->classes)
		{
			auto& cls = pcls.second;
			for (ScriptScope::FUNCTION_CONTAINER::iterator it2 = cls->functions.begin(); it2 != cls->functions.end(); ++it2)
			{
				if (ScriptExtension::ExtensionFunction *extf = dynamic_cast<ScriptExtension::ExtensionFunction*>(*it2))
				{
					extf->setup();
				}
			}
		}
	}*/

	void ScriptNamespace::resolveClassMembers()
	{
		/*for (auto& pcls : this->classes)
		{
			auto& cls = pcls.second;

			for (ScriptScope::FUNCTION_PROTOTYPE_CONTAINER::iterator it2 = cls->functionPrototypes.begin(); it2 != cls->functionPrototypes.end(); ++it2)
			{
				(*it2)->build();
			}

			cls->functionPrototypes.clear();
		}*/
	}
}