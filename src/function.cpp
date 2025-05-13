#include <iostream>
#include <memory>
#include <exception>

#include "function.hpp"
#include "pFunction.hpp"
#include "pStatement.hpp"
#include "pVarDeclaration.hpp"
#include "varDeclaration.hpp"
#include "statement.hpp"
#include "pReturn.hpp"
#include "return.hpp"
#include "script.hpp"
#include "defs.hpp"
#include "scriptValue.hpp"
#include "scope.hpp"

#include "literal.hpp"
#include "sfPrint.hpp"

#include "compileException.hpp"

#include "debug.hpp"

namespace gscript
{
	ScriptFunction::ScriptFunction(ScriptScope &scope, const std::string &name, ScriptType *returnType, const PARAMS_T &parameters)
		: ScriptCallable(scope),
		ScriptRunnable(&scope),
		name(name),
		parameters(parameters),
		returnType(returnType)
	{
	}

	ScriptFunction::~ScriptFunction()
	{
	}

	void ScriptFunction::setup(const ParserFunction &pfunc)
	{
		ScriptRunnable::setup(pfunc.body);
	}

	ScriptValue *ScriptFunction::run(const CALLABLE_PARAMS_T &c)
	{
		this->validateParams(c);
		this->registerParameters(c);

		return ScriptRunnable::run();
	}

	PARAMS_T &ScriptFunction::getParameters()
	{
		return this->parameters;
	}

	//
	// WHEN USING REFERENCE TYPE - SETVALUE SHOULD SET THE VALUE THAT THE REFERENCE POINTS TO.
	// NEW METHOD SHOULD BE ADDED TO RETARGET THE REFERENCE - THIS SHOULD BE USED HERE.
	//

	void ScriptFunction::registerParameters(const CALLABLE_PARAMS_T &c)
	{
		for (
			PARAMS_T::iterator it = this->getParameters().begin();
			it != this->getParameters().end();
			++it
			)
		{
			int idx = it - this->getParameters().begin();

			ScriptValue *val = c[idx];
			//it->setValue(val);
			//it->setValue(val->clone());

			if (it->getType()->getAbsoluteTypeDescriptor() == VALUE_TYPE_T::VT_REFERENCE)
				//static_cast<ScriptReferenceValue&>(*it->getValue()).setValue(val);
				static_cast<ScriptReferenceValue&>(*it->getValue()).retarget(val);
			else
				it->setValue(val->clone());
		}
	}

	bool ScriptFunction::validateParams(const CALLABLE_PARAMS_T &c, bool throwException)
	{
		if (c.size() != this->getParameters().size())
		{
			this->throwBadParameters(std::string("Number of parameters does not match, given ") + std::to_string(c.size()) +
				", expected " + std::to_string(this->getParameters().size()));
			return false;
		}

		for (
			PARAMS_T::const_iterator it = this->getParameters().begin();
			it != this->getParameters().end();
			++it
			)
		{
			int idx = it - this->getParameters().begin();

			const ScriptType &typeA = *it->getType();
			const ScriptType &typeB = *c[idx]->getType();

			if (typeA != typeB)
			{
				this->throwBadParameters("Parameter type mismatch");
				return false;
			}
		}

		return true;
	}

	void ScriptFunction::throwBadParameters(const std::string &text)
	{
		DBG(text);
		throw new std::runtime_error(text);
	}

	ScriptVariable *ScriptFunction::findVariable(const std::string &name)
	{
		for (
			PARAMS_T::iterator it = this->parameters.begin();
			it != this->parameters.end();
			++it
			)
		{
			if (it->getName() == name)
				return &(*it);
		}

		return ScriptScope::findVariable(name);
	}

	bool ScriptFunction::matches(const std::string &name, const PARAMS_T &parameters)
	{
		if (name != this->name)
			return false;

		if (parameters.size() != this->parameters.size())
			return false;

		PARAMS_T::const_iterator it = parameters.begin();
		PARAMS_T::const_iterator it2 = this->parameters.begin();

		for (
			;
			it != parameters.end();
			++it, ++it2)
		{
			auto a = it->getType();
			auto b = it2->getType();

			auto x = a->operator!=(*b);
			auto y = a->operator==(*b);
			auto z = b->operator!=(*a);

			if (*it->getType() != *it2->getType())
				return false;
		}

		return true;
	}

	const ScriptType *ScriptFunction::getType() const
	{
		return this->returnType;
	}

	const std::string &ScriptFunction::getName() const
	{
		return this->name;
	}

	// PROTOTYPE

	ScriptFunctionPrototype::ScriptFunctionPrototype(ScriptFunction &target, const ParserFunction &pfunc)
		:target(target),
		pfunc(pfunc)
	{
	}

	void ScriptFunctionPrototype::build()
	{
		this->target.setup(this->pfunc);
	}

	// EXTERN

	ScriptExternFunctionConnector::ScriptExternFunctionConnector(const std::string &externName, ScriptScope &scope, const std::string &name, ScriptType *returnType, const PARAMS_T &parameters)
		:ScriptFunction(scope, name, returnType, parameters),
		target(scope.getExternFunction(externName))
	{
	}

	ScriptValue *ScriptExternFunctionConnector::run(const CALLABLE_PARAMS_T &c)
	{
		return this->target.run(c);
	}
}