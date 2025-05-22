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

namespace gscript
{
	class ScriptScope;
	class ScriptFunction;

	class ScriptMethodCall;

	class ScriptFuncCall : public ScriptCallable
	{
	public:
		//ScriptFunction *func;
		EntityLink<ScriptFunction*> *func;

		std::vector<ScriptStatement> params;

		//ScriptFuncCall(ScriptScope &scope, ScriptFunction *func, const std::vector<ScriptStatement> &params);
		ScriptFuncCall(ScriptScope &scope, EntityLink<ScriptFunction*> *func, const std::vector<ScriptStatement> &params);

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType *getType() const override;
		void setInstance(ScriptClassValue *instance);
	};

	// METHOD CALL (to be removed?)

	class ScriptMethodCall : public ScriptFuncCall
	{
	public:
		void setInstance(ScriptClassValue *instance);
	};

	// RESOLV

	class ScriptFuncCallResolv : public ScriptFuncCall
	{
	protected:
		std::string name;
		PARAMS_T paramTypes;
		bool staticCall;

	public:
		ScriptFuncCallResolv(ScriptScope &originalScope, const std::vector<ScriptStatement> &params, const std::string &name, PARAMS_T paramTypes, bool staticCall);

		ScriptFuncCall *resolve(const ScriptScope *scope);
	};
}

#endif