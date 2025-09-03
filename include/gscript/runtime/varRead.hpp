#ifndef _h_gscript_varread
#define _h_gscript_varread

#include "callable.hpp"
#include "entityLink.hpp"
#include "scopedCall.hpp"

#include <string>
#include <memory>

namespace gscript
{
	class ScriptVariable;

	// Var Read

	class ScriptVarRead : public ScriptCallable, public ScriptScopedCall
	{
	public:
		//ScriptVariable *var;
		//std::unique_ptr<EntityLink<ScriptVariable*>> var = nullptr;

		ScriptVarRead(ScriptScope& scope);
		ScriptVarRead(ScriptScope& scope, VariableAccessor accessor);
		ScriptVarRead(ScriptScope& scope, ScriptVariable *variable);
		ScriptVarRead(ScriptScope& scope, const std::string& name);
		//ScriptVarRead(ScriptScope &scope, ScriptVariable *var);
		//ScriptVarRead(ScriptScope &scope, std::unique_ptr<EntityLink<ScriptVariable*>>&& link);

		virtual const ScriptType *getType() const override;

		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual ScriptVariable* get();

		virtual void setScope(ScriptClassInstance *instance);

	protected:
		VariableAccessor accessor;
	};

	class ScriptVarReadPrototype : public ScriptCallablePrototype
	{
	public:
		ScriptVarReadPrototype(ScriptScope& scope, const std::string& varname);

		virtual std::unique_ptr<ScriptCallable> build() override;

	private:
		std::string varname;
	};

	// Array read

	class ScriptArrayRead : public ScriptVarRead
	{
	public:
		ScriptArrayRead(ScriptScope &scope, VariableAccessor accessor, std::unique_ptr<ScriptCallable> &&arrayAccessor);
		ScriptArrayRead(ScriptScope &scope, ScriptVariable *variable, std::unique_ptr<ScriptCallable> &&arrayAccessor);
		ScriptArrayRead(ScriptScope &scope, const std::string& name, std::unique_ptr<ScriptCallable> &&arrayAccessor);
		virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const ScriptType *getType() const override;

	protected:
		std::unique_ptr<ScriptCallable> arrayAccessor = nullptr;
	};

	//// RESOLVER

	//class ScriptVarReadResolver : public ScriptVarRead
	//{
	//public:
	//	std::string name;
	//	ScriptVarReadResolver(ScriptScope &scope, const std::string &name);

	//	std::unique_ptr<ScriptVarRead> resolve(ScriptScope &scope, bool member);
	//};

	//class ScriptArrayReadResolver : public ScriptVarReadResolver
	//{
	//public:
	//	ScriptArrayReadResolver(ScriptScope &scope, std::unique_ptr<ScriptCallable> &&accessor, const std::string &name);

	//	std::unique_ptr<ScriptVarRead> resolve(ScriptScope &scope, bool member);

	//protected:
	//	std::unique_ptr<ScriptCallable> accessor = nullptr;
	//};
}

#endif