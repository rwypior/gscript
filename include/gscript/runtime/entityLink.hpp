#ifndef _h_gscript_entitylink
#define _h_gscript_entitylink

#include "compileException.hpp"
#include "lib.hpp"
#include "defs.hpp"

#include <cassert>
#include <memory>

namespace gscript
{
	class ScriptScopeBase;
	class ScriptVariable;
	class ScriptType;
	class ScriptFunction;

	// Variable accessor stores a scope and address of given variable which
	// may be used at later point to access said variable
	// Scope is mutable and may be changed by scoped call, for example by calling
	// variable read by scope operator
	class VariableAccessor
	{
	public:
		virtual ~VariableAccessor() = default;
		VariableAccessor() = default;
		SCRIPT_API VariableAccessor(ScriptScopeBase* scope, size_t addr);
		SCRIPT_API VariableAccessor(const VariableAccessor& b);

		SCRIPT_API static std::unique_ptr<VariableAccessor> find(ScriptScopeBase& scope, const std::string& name, bool searchParents = true);
		SCRIPT_API virtual ScriptVariable* get();
		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const;
		SCRIPT_API void setScope(ScriptScopeBase* scope);
		SCRIPT_API operator bool() const;

	protected:
		ScriptScopeBase* scope = nullptr;
		size_t addr = NullAddr;
	};

	// Variable accessor stores a scope and address of given function parameter which
	// may be used at later point to access said variable
	// Works in a similar way to VariableAccessor.
	// Is only valid for functions
	class ParameterAccessor : public VariableAccessor
	{
	public:
		using VariableAccessor::VariableAccessor;

		SCRIPT_API static std::unique_ptr<ParameterAccessor> find(ScriptScopeBase& scope, const std::string& name, bool searchParents = true);
		SCRIPT_API ScriptVariable* get();
		SCRIPT_API const std::shared_ptr<ScriptType> getType() const;
	};

	// Function accessor stores a scope and address of given function which
	// may be used at later point to access said function
	// Scope is mutable and may be changed by scoped call, for example by calling
	// function call by scope operator
	class FunctionAccessor
	{
	public:
		FunctionAccessor() = default;
		SCRIPT_API FunctionAccessor(ScriptScopeBase* scope, size_t addr);
		SCRIPT_API FunctionAccessor(const FunctionAccessor& b);

		SCRIPT_API static FunctionAccessor find(ScriptScopeBase& scope, const std::string& name, const PARAMS_T& params, bool searchParents = true);
		SCRIPT_API ScriptFunction* get();
		SCRIPT_API const std::shared_ptr<ScriptType> getType() const;
		SCRIPT_API void setScope(ScriptScopeBase* scope);
		SCRIPT_API operator bool() const;

	private:
		ScriptScopeBase* scope = nullptr;
		size_t addr = NullAddr;
	};
}

#endif