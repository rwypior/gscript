#include "runtime/namespace.hpp"
#include "runtime/varDeclaration.hpp"
#include "runtime/class.hpp"
#include "extension/scriptExtension.hpp"
#include "compileException.hpp"

namespace gscript
{
	ScriptNamespace::ScriptNamespace(ScriptScopeBase* parentScope)
		: ScriptScope(parentScope)
	{
	}

	ScriptNamespace::ScriptNamespace(ScriptScopeBase* parentScope, const std::string &name)
		: ScriptScope(parentScope),
		name(name)
	{
	}

	const std::string &ScriptNamespace::getName() const
	{
		return this->name;
	}

	ScriptClass& ScriptNamespace::registerClass(std::unique_ptr<ScriptClass> &&sclass)
	{
		sclass->parentScope = this;
		this->classes.push_back(std::move(sclass));
		return *this->classes.back();
	}

	ScriptNamespace &ScriptNamespace::registerNamespace(std::unique_ptr<ScriptNamespace> &&ns)
	{
		this->namespaces.push_back(std::move(ns));
		return *this->namespaces.back();
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

	std::vector<std::unique_ptr<ScriptClass>>& ScriptNamespace::getClasses()
	{
		return this->classes;
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

	std::vector<std::unique_ptr<ScriptNamespace>>& ScriptNamespace::getNamespaces()
	{
		return this->namespaces;
	}
}