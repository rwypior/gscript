#ifndef _h_gscript_varread
#define _h_gscript_varread

#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/entityLink.hpp"
#include "gscript/runtime/scopedCall.hpp"

#include <string>
#include <memory>

namespace gscript
{
	class ScriptVariable;

	// Var Read

	class ScriptVarRead : public ScriptCallable, public ScriptScopedCall
	{
	public:
		SCRIPT_API ScriptVarRead(const ScriptVarRead& b);
		SCRIPT_API explicit ScriptVarRead(std::unique_ptr<VariableAccessor>&& accessor);
		SCRIPT_API ScriptVarRead(ScriptScopeBase& scope, ScriptVariable *variable);
		SCRIPT_API ScriptVarRead(ScriptScopeBase& scope, const std::string& name);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual ScriptVariable* get();

		SCRIPT_API virtual void setScope(ScriptClassInstance *instance);

	protected:
		std::unique_ptr<VariableAccessor> accessor;
	};

	class ScriptVarReferenceRead : public ScriptVarRead
	{
	using ScriptVarRead::ScriptVarRead;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T& c = CALLABLE_PARAMS_T()) override;
	};

	class ScriptVarReadPrototype : public ScriptCallablePrototype
	{
	public:
		SCRIPT_API ScriptVarReadPrototype(const std::string& varname);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> build(ScriptScopeBase& scope) override;

		SCRIPT_API const std::string& getName() const;

		// Disable returning reference variable - useful in eg. return statements
		// TODO - should probably revert this - references should be disabled by default
		// and enableReferences function should be created instead
		SCRIPT_API void disableReference();

	private:
		std::string varname;
		bool isReference = true;
	};

	// Array read

	class ScriptArrayRead : public ScriptVarRead
	{
	public:
		SCRIPT_API ScriptArrayRead(const ScriptArrayRead& b);
		SCRIPT_API ScriptArrayRead(std::unique_ptr<VariableAccessor>&& accessor, std::unique_ptr<ScriptCallable> &&arrayAccessor);
		SCRIPT_API ScriptArrayRead(ScriptScopeBase &scope, ScriptVariable *variable, std::unique_ptr<ScriptCallable> &&arrayAccessor);
		SCRIPT_API ScriptArrayRead(ScriptScopeBase &scope, const std::string& name, std::unique_ptr<ScriptCallable> &&arrayAccessor);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const override;

	protected:
		std::unique_ptr<ScriptCallable> arrayAccessor = nullptr;
	};

	class ScriptArrayReadPrototype : public ScriptCallablePrototype
	{
	public:
		SCRIPT_API ScriptArrayReadPrototype(const std::string& varname, std::unique_ptr<ScriptCallable>&& arrayAccessor);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> build(ScriptScopeBase& scope) override;

	private:
		std::string varname;
		std::unique_ptr<ScriptCallable> arrayAccessor;
	};
}

#endif