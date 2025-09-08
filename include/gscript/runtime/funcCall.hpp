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
		ScriptFuncCall(ScriptScope& scope, FunctionAccessor func, std::vector<std::unique_ptr<ScriptStatement>>&& params = {});
		ScriptFuncCall(ScriptScope& scope, ScriptFunction* func, std::vector<std::unique_ptr<ScriptStatement>>&& params = {});
		ScriptFuncCall(ScriptScope& scope, const std::string& name, std::vector<std::unique_ptr<ScriptStatement>>&& params = {});

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType *getType() const override;
		void setInstance(ScriptClassValue *instance);

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
		ScriptFuncCallPrototype(ScriptScope& scope, const std::string& funcname, std::vector<std::unique_ptr<ScriptStatement>>&& params = {}, bool staticCall = false);

		virtual std::unique_ptr<ScriptCallable> build() override;

		const std::string& getName() const;
		const std::vector<std::unique_ptr<ScriptStatement>>& getParams() const;
		const bool isStaticCall() const;

	private:
		std::string funcname;
		std::vector<std::unique_ptr<ScriptStatement>> params;
		bool staticCall = false;
	};

	//// RESOLVER

	//class ScriptFuncCallResolver : public ScriptFuncCall
	//{
	//public:
	//	ScriptFuncCallResolver(ScriptScope &originalScope, std::vector<std::unique_ptr<ScriptStatement>>&& params, const std::string &name, PARAMS_T paramTypes, bool staticCall);

	//	std::unique_ptr<ScriptFuncCall> resolve(const ScriptScope *scope);

	//protected:
	//	std::string name;
	//	PARAMS_T paramTypes;
	//	bool staticCall = false;
	//};
}

#endif