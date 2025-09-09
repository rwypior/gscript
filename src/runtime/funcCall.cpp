#include "runtime/funcCall.hpp"
#include "runtime/function.hpp"
#include "runtime/method.hpp"
#include "defs.hpp"
#include "util.hpp"
#include "compileException.hpp"

namespace gscript
{
	ScriptFuncCall::ScriptFuncCall(ScriptScope& scope, FunctionAccessor func, std::vector<std::unique_ptr<ScriptStatement>>&& params)
		: ScriptCallable(scope)
		, accessor(func)
		, params(std::move(params))
	{
	}

	ScriptFuncCall::ScriptFuncCall(ScriptScope& scope, ScriptFunction* func, std::vector<std::unique_ptr<ScriptStatement>>&& params)
		: ScriptCallable(scope)
		, accessor(func ? FunctionAccessor::find(scope, func->getName(), func->getParameters()) : FunctionAccessor())
		, params(std::move(params))
	{
	}

	ScriptFuncCall::ScriptFuncCall(ScriptScope& scope, const std::string& name, std::vector<std::unique_ptr<ScriptStatement>>&& params)
		: ScriptCallable(scope)
		, accessor(FunctionAccessor::find(scope, name, extractParams(params)))
		, params(std::move(params))
	{
	}

	ScriptValue *ScriptFuncCall::run(const CALLABLE_PARAMS_T &c)
	{
		if (!this->accessor)
			return nullptr;

		CALLABLE_PARAMS_T params;

		for (auto& param : this->params)
		{
			params.push_back(param->run());
			//params.push_back(it->run()->clone());
		}

		return this->accessor.get()->run(params);
	}

	const ScriptType *ScriptFuncCall::getType() const
	{
		return this->accessor.getType();
	}

	void ScriptFuncCall::setInstance(ScriptClassValue *instance)
	{
		this->accessor.setScope(instance->getValue());
		if (this->accessor)
			static_cast<ScriptMethod*>(this->accessor.get())->setClassInstance(instance);

		/*static_cast<VirtualEntityLink<ScriptFunction*>*>(this->func)->setInstance(instance->getValue());
		static_cast<ScriptMethod*>(this->func->get())->setClassInstance(instance);*/
	}

	// Prototype

	ScriptFuncCallPrototype::ScriptFuncCallPrototype(ScriptScope& scope, const std::string& funcname, std::vector<std::unique_ptr<ScriptStatement>>&& params, bool staticCall)
		: ScriptCallablePrototype(scope)
		, funcname(funcname)
		, params(std::move(params))
		, staticCall(staticCall)
	{
	}

	std::unique_ptr<ScriptCallable> ScriptFuncCallPrototype::build(ScriptScopeBase* scope)
	{
		ScriptScope* usedScope = static_cast<ScriptScope*>(scope ? scope : &this->scope);

		PARAMS_T params = extractParams(this->params);
		auto result = std::make_unique<ScriptFuncCall>(*usedScope, FunctionAccessor::find(*usedScope, this->funcname, params));

		return result;
	}

	const std::string& ScriptFuncCallPrototype::getName() const
	{
		return this->funcname;
	}

	const std::vector<std::unique_ptr<ScriptStatement>>& ScriptFuncCallPrototype::getParams() const
	{
		return this->params;
	}

	const bool ScriptFuncCallPrototype::isStaticCall() const
	{
		return this->staticCall;
	}

	// RESOLVER

	//ScriptFuncCallResolver::ScriptFuncCallResolver(ScriptScope &originalScope, std::vector<std::unique_ptr<ScriptStatement>>&& params, const std::string &name, PARAMS_T paramTypes, bool staticCall)
	//	: ScriptFuncCall(originalScope, nullptr, std::move(params))
	//	, name(name)
	//	, paramTypes(paramTypes)
	//	, staticCall(staticCall)
	//{
	//}

	//std::unique_ptr<ScriptFuncCall> ScriptFuncCallResolver::resolve(const ScriptScope *scope)
	//{
	//	if (this->staticCall)
	//		scope = &this->scope;

	//	ScriptFunction *fnc = scope->getFunction(this->name, this->paramTypes);

	//	if (this->staticCall)
	//	{
	//		if (ScriptMethod *m = dynamic_cast<ScriptMethod*>(fnc))
	//			if (!m->isStatic())
	//				throw CompileException("Function \"" + this->name + "\" was not called through object and is not static");
	//	}

	//	//ScriptFuncCall *result = new ScriptFuncCall(this->scope, fnc, this->params);

	//	bool isVirtual = false;

	//	if (ScriptMethod *method = dynamic_cast<ScriptMethod*>(fnc))
	//	{
	//		if (method->isVirtual())
	//			isVirtual = true;
	//	}

	//	ScriptFuncCall *result;
	//	if (isVirtual)
	//		return std::make_unique<ScriptFuncCall>(this->scope, new VirtualEntityLink<ScriptFunction*>(fnc), std::move(this->params));

	//	return std::make_unique<ScriptFuncCall>(this->scope, new DirectEntityLink<ScriptFunction*>(fnc), std::move(this->params));
	//}
}