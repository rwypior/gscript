#include "runtime/funcCall.hpp"
#include "runtime/function.hpp"
#include "runtime/method.hpp"
#include "runtime/classInstance.hpp"
#include "defs.hpp"
#include "util.hpp"
#include "compileException.hpp"

namespace gscript
{
	ScriptFuncCall::ScriptFuncCall(ScriptScopeBase& scope, FunctionAccessor func, std::vector<std::unique_ptr<ScriptStatement>>&& params)
		//: ScriptCallable(scope)
		: accessor(func)
		, params(std::move(params))
	{
	}

	ScriptFuncCall::ScriptFuncCall(ScriptScopeBase& scope, ScriptFunction* func, std::vector<std::unique_ptr<ScriptStatement>>&& params)
		//: ScriptCallable(scope)
		: accessor(func ? FunctionAccessor::find(scope, func->getName(), func->getParameters()) : FunctionAccessor())
		, params(std::move(params))
	{
	}

	ScriptFuncCall::ScriptFuncCall(ScriptScopeBase& scope, const std::string& name, std::vector<std::unique_ptr<ScriptStatement>>&& params)
		//: ScriptCallable(scope)
		: accessor(FunctionAccessor::find(scope, name, extractParams(params)))
		, params(std::move(params))
	{
	}

	std::unique_ptr<ScriptValue> ScriptFuncCall::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		if (!this->accessor)
			return nullptr;

		std::vector<std::unique_ptr<ScriptValue>> params;

		for (auto& param : this->params)
		{
			params.push_back(param->run(scope));
		}

		return this->accessor.get()->run(scope, std::move(params));
	}

	const std::shared_ptr<ScriptType> ScriptFuncCall::getType() const
	{
		return this->accessor.getType();
	}

	void ScriptFuncCall::setInstance(std::unique_ptr<ScriptValue>&& instance)
	{
		auto sclassinst = static_cast<ScriptClassValue*>(instance->data())->getValue().get();
		this->accessor.setScope(sclassinst);
		if (this->accessor)
			static_cast<ScriptMethod*>(this->accessor.get())->setClassInstance(std::move(instance));
	}

	// Prototype

	ScriptFuncCallPrototype::ScriptFuncCallPrototype(ScriptScopeBase* scope, const std::string& funcname, std::vector<std::unique_ptr<ScriptStatement>>&& params, bool staticCall)
		//: ScriptCallablePrototype(scope)
		: scope(scope)
		, funcname(funcname)
		, params(std::move(params))
		, staticCall(staticCall)
	{
	}

	std::unique_ptr<ScriptCallable> ScriptFuncCallPrototype::build(ScriptScopeBase& scope)
	{
		ScriptScopeBase* usedScope = this->scope ? this->scope : &scope;

		PARAMS_T params = extractParams(this->params);
		// TODO - use constructor which uses name instead of function here
		auto result = std::make_unique<ScriptFuncCall>(*usedScope, FunctionAccessor::find(*usedScope, this->funcname, params), std::move(this->params));

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

	std::vector<std::unique_ptr<ScriptStatement>>& ScriptFuncCallPrototype::getParams()
	{
		return this->params;
	}

	const bool ScriptFuncCallPrototype::isStaticCall() const
	{
		return this->staticCall;
	}
}