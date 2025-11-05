#ifndef _h_gscript_funccall
#define _h_gscript_funccall

#include "gscript/EntityPath.hpp"
#include "gscript/defs.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/statement.hpp"
#include "gscript/runtime/funcParam.hpp"
#include "gscript/runtime/entityLink.hpp"

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
		SCRIPT_API ScriptFuncCall(const ScriptFuncCall& b);
		SCRIPT_API ScriptFuncCall(FunctionAccessor func, std::vector<std::unique_ptr<ScriptStatement>>&& params = {});
		SCRIPT_API ScriptFuncCall(ScriptScopeBase& scope, ScriptFunction* func, std::vector<std::unique_ptr<ScriptStatement>>&& params = {});
		SCRIPT_API ScriptFuncCall(ScriptScopeBase& scope, const std::string& name, std::vector<std::unique_ptr<ScriptStatement>>&& params = {});

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const override;
		SCRIPT_API void setInstance(std::unique_ptr<ScriptValue>&& instance);

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
		SCRIPT_API ScriptFuncCallPrototype(ScriptScopeBase* scope, const std::string& funcname, std::vector<std::unique_ptr<ScriptStatement>>&& params = {}, bool staticCall = false);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> build(ScriptScopeBase& scope) override;

		SCRIPT_API const std::string& getName() const;
		SCRIPT_API const std::vector<std::unique_ptr<ScriptStatement>>& getParams() const;
		SCRIPT_API std::vector<std::unique_ptr<ScriptStatement>>& getParams();
		SCRIPT_API const bool isStaticCall() const;

	private:
		ScriptScopeBase* scope;
		std::string funcname;
		std::vector<std::unique_ptr<ScriptStatement>> params;
		bool staticCall = false;
	};
}

#endif