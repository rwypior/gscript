#ifndef _h_gscript_funccall
#define _h_gscript_funccall

#include "callable.hpp"
#include "statement.hpp"
#include "EntityPath.hpp"
#include "funcParam.hpp"
#include "entityLink.hpp"
#include "defs.hpp"

#include <string>
#include <vector>
#include <memory>

namespace gscript
{
	class ScriptScope;
	class ScriptFunction;

	class ScriptMethodCall;

	class ScriptFuncCall : public ScriptCallable
	{
	public:
		//ScriptFuncCall(ScriptScope &scope, ScriptFunction *func, const std::vector<ScriptStatement> &params);
		//ScriptFuncCall(ScriptScope& scope, EntityLink<ScriptFunction*>* func, std::vector<std::unique_ptr<ScriptStatement>>&& params = {});
		ScriptFuncCall(ScriptScopeBase& scope, FunctionAccessor func, std::vector<std::unique_ptr<ScriptStatement>>&& params = {});
		ScriptFuncCall(ScriptScopeBase& scope, ScriptFunction* func, std::vector<std::unique_ptr<ScriptStatement>>&& params = {});
		ScriptFuncCall(ScriptScopeBase& scope, const std::string& name, std::vector<std::unique_ptr<ScriptStatement>>&& params = {});

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const std::shared_ptr<ScriptType> getType() const override;
		void setInstance(std::unique_ptr<ScriptValue>&& instance);

		FunctionAccessor& getFunc()
		{
			return this->accessor;
		}

	protected:
		//ScriptFunction *func;
		//EntityLink<ScriptFunction*> *func = nullptr;
		FunctionAccessor accessor;

		std::vector<std::unique_ptr<ScriptStatement>> params;
	};

	// Prototype

	class ScriptFuncCallPrototype : public ScriptCallablePrototype
	{
	public:
		ScriptFuncCallPrototype(ScriptScopeBase* scope, const std::string& funcname, std::vector<std::unique_ptr<ScriptStatement>>&& params = {}, bool staticCall = false);

		virtual std::unique_ptr<ScriptCallable> build(ScriptScopeBase& scope) override;

		const std::string& getName() const;
		const std::vector<std::unique_ptr<ScriptStatement>>& getParams() const;
		std::vector<std::unique_ptr<ScriptStatement>>& getParams();
		const bool isStaticCall() const;

	private:
		ScriptScopeBase* scope;
		std::string funcname;
		std::vector<std::unique_ptr<ScriptStatement>> params;
		bool staticCall = false;
	};
}

#endif