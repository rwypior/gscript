#ifndef _h_gscript_varread
#define _h_gscript_varread

#include "callable.hpp"
#include "entityLink.hpp"
#include "scopedCall.hpp"

#include <string>

namespace gscript
{
	class ScriptVariable;

	class ScriptVarRead : public ScriptCallable, public ScriptScopedCall
	{
	public:
		//ScriptVariable *var;
		EntityLink<ScriptVariable*> *var = nullptr;

		ScriptVarRead(ScriptScope &scope);
		ScriptVarRead(ScriptScope &scope, ScriptVariable *var);
		ScriptVarRead(ScriptScope &scope, EntityLink<ScriptVariable*> *link);

		virtual const ScriptType *getType() const override;

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual void setScope(ScriptClassInstance *instance);
	};

	class ScriptArrayRead : public ScriptVarRead
	{
	public:
		ScriptArrayRead(ScriptScope &scope, ScriptVariable *var, ScriptCallable *accessor);
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType *getType() const override;

	protected:
		ScriptCallable *accessor = nullptr;
	};

	// RESOLV

	class ScriptVarReadResolv : public ScriptVarRead
	{
	public:
		std::string name;
		ScriptVarReadResolv(ScriptScope &scope, const std::string &name);

		ScriptVarRead *resolve(ScriptScope &scope, bool member);
	};

	class ScriptArrayReadResolv : public ScriptVarReadResolv
	{
	public:
		ScriptArrayReadResolv(ScriptScope &scope, ScriptCallable *accessor, const std::string &name);

		ScriptVarRead *resolve(ScriptScope &scope, bool member);

	protected:
		ScriptCallable *accessor = nullptr;
	};
}

#endif