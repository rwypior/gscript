#include "runtime/function.hpp"
#include "runtime/varDeclaration.hpp"
#include "runtime/statement.hpp"
#include "runtime/return.hpp"
#include "runtime/scriptValue.hpp"
#include "runtime/scope.hpp"
#include "runtime/literal.hpp"
#include "framework/sfPrint.hpp"

#include "defs.hpp"
#include "compileException.hpp"
#include "debug.hpp"

#include <iostream>
#include <memory>
#include <exception>

namespace gscript
{
	ScriptFunction::ScriptFunction(const ScriptFunction& fnc)
		: ScriptScope(fnc.baseFunction ? *fnc.baseFunction : fnc)
		, ScriptExecutiveBlock(fnc)
		, name(fnc.name)
		, returnType(fnc.returnType->clone())
		, parameters(fnc.parameters)
		, baseFunction(fnc.baseFunction ? fnc.baseFunction : &fnc)
	{
	}

	ScriptFunction::ScriptFunction(ScriptScopeBase &scope, const std::string &name, std::shared_ptr<ScriptType> returnType, const PARAMS_T &parameters, std::vector<std::unique_ptr<ScriptCallable>>&& statements)
		: ScriptScope(&scope)
		, ScriptExecutiveBlock(std::move(statements))
		, name(name)
		, parameters(parameters)
		, returnType(returnType)
	{
	}

	std::unique_ptr<ScriptValue> ScriptFunction::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		this->validateParams(c); // TODO - move this to compilation stage

		// TODO - create a better implementation of this

		ScriptFunction target(*this);

		RemapScope remapping(*this, target);

		target.registerParameters(c);
		return target.execute(target);
		//return this->execute(target);
	}

	std::unique_ptr<ScriptValue> ScriptFunction::fastrun(ScriptScopeBase& scope, const CALLABLE_PARAMS_T& c)
	{
		this->registerParameters(c);
		return this->execute(scope);
	}

	PARAMS_T &ScriptFunction::getParameters()
	{
		return this->parameters;
	}

	void ScriptFunction::registerParameters(const CALLABLE_PARAMS_T &c)
	{
		for (
			PARAMS_T::iterator it = this->getParameters().begin();
			it != this->getParameters().end();
			++it
			)
		{
			int idx = it - this->getParameters().begin();

			const std::unique_ptr<ScriptValue>& val = c[idx];

			if (it->getType()->getTypeDescriptor() == ValueType::Reference)
				assert(!"NOT IMPLEMENTED"); // TODO - this needs to be fixed
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

			auto typeA = it->getType();
			auto typeB = c[idx]->getType();

			if (*typeA != *typeB)
			{
				this->throwBadParameters("Parameter type mismatch");
				return false;
			}
		}

		return true;
	}

	FunctionParameter* ScriptFunction::findParam(const std::string& name)
	{
		auto& params = this->getParameters();

		auto it = std::find_if(params.begin(), params.end(), [&name](const FunctionParameter& v) {
			return v.getName() == name;
			});
		if (it != params.end())
			return &*it;

		return nullptr;
	}

	ScopedAddress ScriptFunction::findParamAddr(const std::string& name)
	{
		auto& params = this->getParameters();

		auto it = std::find_if(params.begin(), params.end(), [&name](const FunctionParameter& v) {
			return v.getName() == name;
			});
		if (it != params.end())
			return ScopedAddress(this, it - params.begin());

		return {};
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
			if (*it->getType() != *it2->getType())
				return false;
		}

		return true;
	}

	const std::shared_ptr<ScriptType> ScriptFunction::getType() const
	{
		return this->returnType;
	}

	const std::string &ScriptFunction::getName() const
	{
		return this->name;
	}

	// TODO - fix this problem:
	// 1. Function must be "copied" to allow recurrence to work - it needs it's own memory space
	// 2. When the function is copied, only the parameters are copied and the statements
	//	are reused from the original - so is their scope - it points to the original scope (function)
	// 3. This is a problem with nested scopes - they are not retargetted, so the solution is to:
	// a. Either implement isBaseOf for every scope, not just the function, but this implies additional
	//	memory usage and overhead
	// b. Copy the statements when copying function

	bool ScriptFunction::isBaseOf(const ScriptScopeBase& scope) const
	{
		if (const auto fnc = dynamic_cast<const ScriptFunction*>(&scope))
		{
			if (fnc->baseFunction == this || this->baseFunction == fnc->baseFunction)
				return true;
			
			if (auto base = fnc->baseFunction)
			{
				return this->isBaseOf(*base);
			}
		}

		return false;
	}
}