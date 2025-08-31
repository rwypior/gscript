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
		ScriptFuncCall(ScriptScope& scope, EntityLink<ScriptFunction*>* func, std::vector<std::unique_ptr<ScriptStatement>>&& params = {});

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType *getType() const override;
		void setInstance(ScriptClassValue *instance);

		EntityLink<ScriptFunction*>* getFunc()
		{
			return this->func;
		}

	protected:
		//ScriptFunction *func;
		EntityLink<ScriptFunction*> *func = nullptr;

		std::vector<std::unique_ptr<ScriptStatement>> params;
	};

	// METHOD CALL (to be removed?)

	class ScriptMethodCall : public ScriptFuncCall
	{
	public:
		void setInstance(ScriptClassValue *instance);
	};

	// RESOLV

	class ScriptFuncCallResolver : public ScriptFuncCall
	{
	public:
		ScriptFuncCallResolver(ScriptScope &originalScope, std::vector<std::unique_ptr<ScriptStatement>>&& params, const std::string &name, PARAMS_T paramTypes, bool staticCall);

		std::unique_ptr<ScriptFuncCall> resolve(const ScriptScope *scope);

	protected:
		std::string name;
		PARAMS_T paramTypes;
		bool staticCall = false;
	};
}

#endif