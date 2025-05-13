#include <iostream>

#include "class.hpp"
#include "method.hpp"
#include "pMethod.hpp"
#include "function.hpp"
#include "namespace.hpp"
#include "varDeclaration.hpp"
#include "defs.hpp"
#include "pClass.hpp"
#include "varDeclaration.hpp"

#include "entityLink.hpp"

#include "compileException.hpp"

namespace gscript
{
	const char *ScriptClass::KW_THIS = "this";

	ScriptClass::ScriptClass(ScriptScope &scope, const std::string &name, ScriptClass *base)
		: ScriptNamespace(&scope),
		name(name),
		base(base)
	{
	}

	void ScriptClass::setup()
	{
		this->inheritAbstracts();
	}

	void ScriptClass::createThis()
	{
		this->registerVariable(ScriptClass::KW_THIS, new ScriptClassType(*this), nullptr);
	}

	void ScriptClass::makeAbstract()
	{
		this->modifier |= static_cast<int>(CLASS_MODIFIER_T::CM_ABSTRACT);
	}

	bool ScriptClass::isAbstract()
	{
		return this->modifier & static_cast<int>(CLASS_MODIFIER_T::CM_ABSTRACT);
	}

	ScriptClassInstance *ScriptClass::instantiate(const CALLABLE_PARAMS_T &c)
	{
		ScriptClassInstance *inst = new ScriptClassInstance(*this);
		this->initialize(*inst);

		if (ScriptMethod *constructor = this->getConstructor())
			constructor->instrun(inst, c);

		return inst;
	}

	ScriptMethod *ScriptClass::findMethod(const std::string &name, const PARAMS_T &params)
	{
		return static_cast<ScriptMethod*>(this->getFunction(name, params));
	}

	std::vector<ScriptMethod*> ScriptClass::getAbstractMethods()
	{
		std::vector<ScriptMethod*> res;

		for (FUNCTION_CONTAINER::const_iterator it = this->functions.begin(); it != this->functions.end(); ++it)
		{
			if (ScriptMethod *method = dynamic_cast<ScriptMethod*>(*it))
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

	ScriptFunction &ScriptClass::registerFunction(const ParserFunction &c)
	{
		if (const ParserMethod *pmethod = dynamic_cast<const ParserMethod*>(&c))
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
				new ScriptMethod(*this, c.name, ScriptType::create(c.returnTypeName, *this->parentScope), params, pmethod->accessSpecifier.getModifier())
			);
			ScriptMethod &newfunc = static_cast<ScriptMethod&>(*this->functions.back());
			this->assignConstructor(newfunc);

			newfunc.setParentScope(this);
			newfunc.setup(c);

			return newfunc;
		}

		throw CompileException("Only methods can be registered in class");
	}

	void ScriptClass::registerFunction(ScriptFunction *f)
	{
		if (ScriptMethod *method = dynamic_cast<ScriptMethod*>(f))
		{
			f->setParentScope(this);
			this->functions.push_back(f);

			this->assignConstructor(*f);
		}
		else
			throw CompileException("Only methods might get registered in class");
	}

	void ScriptClass::registerMethodPrototype(const ParserMethod &m)
	{
		PARAMS_T params;
		for (
			ParserArglist::ARGLIST_PARAMS_T::const_iterator it = m.arglist.parameters.begin();
			it != m.arglist.parameters.end();
			++it
			)
		{
			params.emplace_back(ScriptType::create(it->type, *this), it->name);
		}

		if (m.externName.empty())
			this->functions.push_back(
				new ScriptMethod(*this, m.name, ScriptType::create(m.returnTypeName, *this->parentScope), params, m.accessSpecifier.getModifier())
			);
		else
			this->functions.push_back(
				new ScriptExternMethod(m.externName, *this, m.name, ScriptType::create(m.returnTypeName, *this->parentScope), params, m.accessSpecifier.getModifier())
			);

		ScriptMethod &newfunc = static_cast<ScriptMethod&>(*this->functions.back());
		this->assignConstructor(newfunc);

		if (newfunc.isAbstract())
			this->makeAbstract();

		newfunc.setParentScope(this);

		ScriptMethodPrototype *proto = new ScriptMethodPrototype(newfunc, m);
		this->functionPrototypes.push_back(proto);
	}

	void ScriptClass::initialize(ScriptClassInstance &instance)
	{
		if (this->base)
			this->base->initialize(instance);

		for (
			ScriptClass::VAR_DECLARATION_CONTAINER::const_iterator it = this->varDeclarations.begin();
			it != this->varDeclarations.end();
			++it
			)
		{
			(*it)->setInstance(instance);
			(*it)->run();
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

	void ScriptClass::addVarDeclaration(ScriptVarDeclaration *svd)
	{
		this->varDeclarations.push_back(svd);
	}

	const std::string &ScriptClass::getName() const
	{
		return this->name;
	}

	bool ScriptClass::isBaseOf(ScriptClass *base)
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

	// CLASS RESOLV

	ScriptClassResolv::ScriptClassResolv(ScriptNamespace *snamespace, ParserClass *pClass)
		:ScriptClass(*snamespace->getParentScope(), pClass->name, NULL),
		pClass(pClass)
	{
	}

	ScriptClass *ScriptClassResolv::resolve()
	{
		ScriptClass *cls = this->snamespace->findClass(this->name);
		delete this;
		return cls;
	}

	// PROTOTYPE

	ScriptClassPrototype::ScriptClassPrototype(ScriptClass &target, const ParserClass &pClass)
		:pClass(pClass),
		target(target)
	{
	}

	void ScriptClassPrototype::build()
	{
		const std::string &base = this->pClass.base;

		ScriptClass *baseClass = NULL;

		if (base.length() > 0)
		{
			baseClass = this->target.getClosestNamespace()->findClass(base);

			if (!baseClass)
				throw CompileException(std::string("Base class ") + base + " not found");
		}

		for (std::vector<ParserFieldDeclaration>::const_iterator it = this->pClass.fields.begin(); it != this->pClass.fields.end(); ++it)
		{
			ScriptVariable &svar = this->target.registerVariable(it->name, ScriptType::create(it->type, *this->target.getParentScope()), NULL);
			//ScriptVarDeclaration *svd = new ScriptVarDeclaration(this->target, svar, ScriptStatement(this->target, it->value));
			EntityLink<ScriptVariable&> *link = new MemberEntityLink<ScriptVariable&, ScriptClassInstance::INSTANCE_VARIABLES_CONTAINER_T>(nullptr, svar.getInternalPointer(), svar);
			ScriptVarDeclaration *svd = new ScriptVarDeclaration(this->target, link, ScriptStatement(this->target, it->value));
			this->target.addVarDeclaration(svd);
		}

		for (std::vector<ParserMethod>::const_iterator it = this->pClass.methods.begin(); it != this->pClass.methods.end(); ++it)
		{
			this->target.registerMethodPrototype(*it);
		}

		this->target.setup();
	}
}