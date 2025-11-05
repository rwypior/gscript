#include "gscript/runtime/class.hpp"
#include "gscript/runtime/method.hpp"
#include "gscript/runtime/function.hpp"
#include "gscript/runtime/namespace.hpp"
#include "gscript/runtime/varDeclaration.hpp"
#include "gscript/runtime/entityLink.hpp"
#include "gscript/runtime/classInstance.hpp"

#include "gscript/defs.hpp"
#include "gscript/compileException.hpp"

namespace gscript
{
	ScriptClass::ScriptClass(ScriptScopeBase& scope, const std::string &name, ScriptClass *base)
		: ScriptNamespace(&scope)
		, name(name)
		, base(base)
	{
	}

	void ScriptClass::setup()
	{
		this->inheritAbstracts();
	}

	void ScriptClass::createThis()
	{
		this->registerVariable(std::make_unique<ScriptVariable>(ScriptClass::keywordThis, std::make_shared<ScriptClassType>(*this), nullptr));
	}

	void ScriptClass::makeAbstract()
	{
		this->modifier |= ClassModifier::Abstract;
	}

	bool ScriptClass::isAbstract()
	{
		return this->modifier & ClassModifier::Abstract;
	}

	std::unique_ptr<ScriptClassInstance> ScriptClass::instantiate(const CALLABLE_PARAMS_T &c)
	{
		auto inst = std::make_unique<ScriptClassInstance>(*this);

		for (auto& field : this->fieldDeclarations)
		{
			field->instantiate(*inst);
		}

		this->initialize(*inst);

		auto instval = std::make_unique<ScriptClassValue>(std::move(inst));

		if (ScriptMethod* constructor = this->getConstructor())
			constructor->instrun(ScriptReferenceValue::create(instval.get()), c);
				
		return std::move(instval->getValue());
	}

	ScriptMethod *ScriptClass::findMethod(const std::string &name, const PARAMS_T &params, bool searchParents, bool searchBase)
	{
		return static_cast<ScriptMethod*>(this->getFunction(name, params, searchParents, searchBase));
	}

	std::vector<ScriptMethod*> ScriptClass::getAbstractMethods()
	{
		std::vector<ScriptMethod*> res;

		for (auto& fnc : this->functions)
		{
			if (ScriptMethod *method = dynamic_cast<ScriptMethod*>(fnc.get()))
			{
				if (method->isAbstract())
					res.push_back(method);
			}
		}

		return res;
	}

	ScriptClass *ScriptClass::getBase() const
	{
		return this->base;
	}

	void ScriptClass::assignConstructor(ScriptFunction &f)
	{
		if (f.getName() == this->name)
			this->constructor = static_cast<ScriptMethod*>(&f);
	}

	ScriptFunction& ScriptClass::registerFunction(std::unique_ptr<ScriptFunction>&& f)
	{
		if (ScriptMethod *method = dynamic_cast<ScriptMethod*>(f.get()))
		{
			f->setParentScope(this);
			
			this->functions.push_back(std::move(f));

			this->assignConstructor(*this->functions.back());
			return *this->functions.back();
		}

		throw CompileException("Only methods may be registered in class");
	}

	ScriptMethod* ScriptClass::findMethod(const std::string& name, const PARAMS_T params, bool searchParents, bool searchBase) const
	{
		return dynamic_cast<ScriptMethod*>(this->findFunction(name, params, searchParents, searchBase));
	}

	void ScriptClass::initialize(ScriptClassInstance &instance)
	{
		if (this->base)
			this->base->initialize(instance);

		for (auto& field : this->fieldDeclarations)
		{
			field->setInstance(instance);
			field->run(instance);
		}
	}

	void ScriptClass::inheritAbstracts()
	{
		if (this->base)
		{
			std::vector<ScriptMethod*> &abstracts = this->base->getAbstractMethods();

			for (std::vector<ScriptMethod*>::const_iterator it = abstracts.begin(); it != abstracts.end(); ++it)
			{
				ScriptMethod *abstractm = *it;
				ScriptMethod *method = this->findMethod(abstractm->getName(), abstractm->getParameters());

				if (!method || method->isAbstract())
				{
					this->makeAbstract();
					return;
				}
			}
		}
	}

	void ScriptClass::addFieldDeclaration(std::unique_ptr<ScriptFieldDeclaration>&& svd)
	{
		this->fieldDeclarations.push_back(std::move(svd));
	}

	const std::string &ScriptClass::getName() const
	{
		return this->name;
	}

	bool ScriptClass::isBaseOf(const ScriptClass *base) const
	{
		if (!this->base)
			return false;

		if (this->base == base)
			return true;

		return this->base->isBaseOf(base);
	}

	bool ScriptClass::operator==(const ScriptClass &b) const
	{
		bool baseCmp = true;
		if (this->base)
			baseCmp = *this->base == *b.base;

		return baseCmp && this->name == b.name;
	}
}