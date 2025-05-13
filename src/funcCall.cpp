#include "funcCall.hpp"
#include "function.hpp"
#include "script.hpp"
#include "defs.hpp"
#include "method.hpp"

#include "compileException.hpp"

#include <iostream>

namespace gscript
{
	/*ScriptFuncCall::ScriptFuncCall(ScriptScope &scope, ScriptFunction *func, const std::vector<ScriptStatement> &params)
		:ScriptCallable(scope),
		func(func),
		params(params)
	{
	}*/

	ScriptFuncCall::ScriptFuncCall(ScriptScope &scope, EntityLink<ScriptFunction*> *func, const std::vector<ScriptStatement> &params)
		:ScriptCallable(scope),
		func(func),
		params(params)
	{
	}

	ScriptValue *ScriptFuncCall::run(const CALLABLE_PARAMS_T &c)
	{
		CALLABLE_PARAMS_T params;

		for (std::vector<ScriptStatement>::iterator it = this->params.begin(); it != this->params.end(); ++it)
		{
			params.push_back(it->run());
			//params.push_back(it->run()->clone());
		}

		return this->func->get()->run(params);
	}

	const ScriptType *ScriptFuncCall::getType() const
	{
		return this->func->get()->getType();
	}

	void ScriptFuncCall::setInstance(ScriptClassValue *instance)
	{
		static_cast<VirtualEntityLink<ScriptFunction*>*>(this->func)->setInstance(instance->getValue());
		static_cast<ScriptMethod*>(this->func->get())->setClassInstance(instance);
		//this->func->get()->variables.front().setValue(instance);
	}

	// METHOD CALL

	void ScriptMethodCall::setInstance(ScriptClassValue *instance)
	{
		this->func->get()->getVariables().front().setValue(instance);
	}

	// RESOLV

	ScriptFuncCallResolv::ScriptFuncCallResolv(ScriptScope &originalScope, const std::vector<ScriptStatement> &params, const std::string &name, PARAMS_T paramTypes, bool staticCall)
		:ScriptFuncCall(originalScope, NULL, params),
		name(name),
		paramTypes(paramTypes),
		staticCall(staticCall)
	{
	}

	ScriptFuncCall *ScriptFuncCallResolv::resolve(const ScriptScope *scope)
	{
		if (this->staticCall)
			scope = &this->scope;

		ScriptFunction *fnc = scope->getFunction(this->name, this->paramTypes);

		if (this->staticCall)
		{
			if (ScriptMethod *m = dynamic_cast<ScriptMethod*>(fnc))
				if (!m->isStatic())
					throw CompileException("Function \"" + this->name + "\" was not called through object and is not static");
		}

		//ScriptFuncCall *result = new ScriptFuncCall(this->scope, fnc, this->params);

		bool isVirtual = false;

		if (ScriptMethod *method = dynamic_cast<ScriptMethod*>(fnc))
		{
			if (method->isVirtual())
				isVirtual = true;
		}

		ScriptFuncCall *result;
		if (isVirtual)
			result = new ScriptFuncCall(this->scope, new VirtualEntityLink<ScriptFunction*>(fnc), this->params);
		else
			result = new ScriptFuncCall(this->scope, new DirectEntityLink<ScriptFunction*>(fnc), this->params);

		delete this;

		return result;
	}
}