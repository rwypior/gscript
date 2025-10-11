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
		//ScriptVarRead(ScriptScope& scope);
		ScriptVarRead(ScriptScopeBase& scope, std::unique_ptr<VariableAccessor>&& accessor);
		ScriptVarRead(ScriptScopeBase& scope, ScriptVariable *variable);
		ScriptVarRead(ScriptScopeBase& scope, const std::string& name);

		virtual const std::shared_ptr<ScriptType> getType() const override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual ScriptVariable* get();

		virtual void setScope(ScriptClassInstance *instance);

	protected:
		std::unique_ptr<VariableAccessor> accessor;
	};

	class ScriptVarReferenceRead : public ScriptVarRead
	{
	using ScriptVarRead::ScriptVarRead;
	public:
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T& c = CALLABLE_PARAMS_T()) override;
	};

	class ScriptVarReadPrototype : public ScriptCallablePrototype
	{
	public:
		ScriptVarReadPrototype(ScriptScopeBase& scope, const std::string& varname);

		virtual std::unique_ptr<ScriptCallable> build(ScriptScopeBase& scope) override;

		const std::string& getName() const;

		void disableReference();

	private:
		std::string varname;
		bool isReference = true;
	};

	// Array read

	class ScriptArrayRead : public ScriptVarRead
	{
	public:
		ScriptArrayRead(ScriptScopeBase &scope, std::unique_ptr<VariableAccessor>&& accessor, std::unique_ptr<ScriptCallable> &&arrayAccessor);
		ScriptArrayRead(ScriptScopeBase &scope, ScriptVariable *variable, std::unique_ptr<ScriptCallable> &&arrayAccessor);
		ScriptArrayRead(ScriptScopeBase &scope, const std::string& name, std::unique_ptr<ScriptCallable> &&arrayAccessor);
		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		virtual const std::shared_ptr<ScriptType> getType() const override;

	protected:
		std::unique_ptr<ScriptCallable> arrayAccessor = nullptr;
	};

	class ScriptArrayReadPrototype : public ScriptCallablePrototype
	{
	public:
		ScriptArrayReadPrototype(ScriptScopeBase& scope, const std::string& varname, std::unique_ptr<ScriptCallable>&& arrayAccessor);

		virtual std::unique_ptr<ScriptCallable> build(ScriptScopeBase& scope) override;

	private:
		std::string varname;
		std::unique_ptr<ScriptCallable> arrayAccessor;
	};
}

#endif