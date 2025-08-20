#include "runtime/funcCall.hpp"
#include "runtime/function.hpp"
#include "runtime/method.hpp"
#include "defs.hpp"
#include "compileException.hpp"

namespace gscript
{
	/*ScriptFuncCall::ScriptFuncCall(ScriptScope &scope, ScriptFunction *func, const std::vector<ScriptStatement> &params)
		:ScriptCallable(scope),
		func(func),
		params(params)
	{
	}*/

	ScriptFuncCall::ScriptFuncCall(ScriptScope &scope, EntityLink<ScriptFunction*> *func, std::vector<std::unique_ptr<ScriptStatement>>&& params)
		: ScriptCallable(scope)
		, func(func)
		, params(std::move(params))
	{
	}

	ScriptValue *ScriptFuncCall::run(const CALLABLE_PARAMS_T &c)
	{
		CALLABLE_PARAMS_T params;

		for (auto& param : this->params)
		{
			params.push_back(param->run());
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
		this->func->get()->getVariables().front()->setValue(instance);
	}

	// RESOLVER

	ScriptFuncCallResolver::ScriptFuncCallResolver(ScriptScope &originalScope, std::vector<std::unique_ptr<ScriptStatement>>&& params, const std::string &name, PARAMS_T paramTypes, bool staticCall)
		: ScriptFuncCall(originalScope, nullptr, std::move(params))
		, name(name)
		, paramTypes(paramTypes)
		, staticCall(staticCall)
	{
	}

	std::unique_ptr<ScriptFuncCall> ScriptFuncCallResolver::resolve(const ScriptScope *scope)
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
			return std::make_unique<ScriptFuncCall>(this->scope, new VirtualEntityLink<ScriptFunction*>(fnc), std::move(this->params));

		return std::make_unique<ScriptFuncCall>(this->scope, new DirectEntityLink<ScriptFunction*>(fnc), std::move(this->params));
	}
}