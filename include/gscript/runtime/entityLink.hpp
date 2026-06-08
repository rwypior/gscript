#ifndef _h_gscript_entitylink
#define _h_gscript_entitylink

#include "gscript/compileException.hpp"
#include "gscript/lib.hpp"
#include "gscript/defs.hpp"

#include <cassert>
#include <memory>

namespace gscript
{
	class ScriptScopeBase;
	class ScriptVariable;
	class ScriptType;
	class ScriptFunction;
	class ScriptFieldDeclaration;

	// Variable accessor stores a scope and address of given variable which
	// may be used at later point to access said variable
	// Scope is mutable and may be changed by scoped call, for example by calling
	// variable read by scope operator
	class VariableAccessor
	{
	public:
		SCRIPT_API virtual ~VariableAccessor();
		SCRIPT_API VariableAccessor();
		SCRIPT_API VariableAccessor(ScriptScopeBase* scope, size_t addr);
		SCRIPT_API VariableAccessor(const VariableAccessor& b);

		SCRIPT_API virtual std::unique_ptr<VariableAccessor> clone() const;

		SCRIPT_API static std::unique_ptr<VariableAccessor> find(ScriptScopeBase& scope, const std::string& name, bool searchParents = true);
		// Access variable pointed by this accessor
		// - scope - allows to retarget scope, especially useful for function calls
		SCRIPT_API virtual ScriptVariable* get(ScriptScopeBase* scope = nullptr);
		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const;
		SCRIPT_API void setScope(ScriptScopeBase* scope);
		SCRIPT_API ScriptScopeBase* getScope();
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

		SCRIPT_API virtual std::unique_ptr<VariableAccessor> clone() const override;

		SCRIPT_API static std::unique_ptr<ParameterAccessor> find(ScriptScopeBase& scope, const std::string& name, bool searchParents = true);
		SCRIPT_API ScriptVariable* get(ScriptScopeBase* scope = nullptr);
		SCRIPT_API const std::shared_ptr<ScriptType> getType() const;
	};

	// Field accessor stores a scope and address of given class field which
	// may be used at later point to access said variable
	// Works in a similar way to VariableAccessor.
	// Is only valid for classes
	class FieldAccessor : public VariableAccessor
	{
	public:
		using VariableAccessor::VariableAccessor;

		/*SCRIPT_API virtual ~FieldAccessor();
		SCRIPT_API FieldAccessor();
		SCRIPT_API FieldAccessor(ScriptScopeBase* scope, size_t addr);
		SCRIPT_API FieldAccessor(const FieldAccessor& b);*/

		//SCRIPT_API virtual std::unique_ptr<FieldAccessor> clone() const;
		SCRIPT_API virtual std::unique_ptr<VariableAccessor> clone() const override;

		SCRIPT_API static std::unique_ptr<FieldAccessor> find(ScriptScopeBase& scope, const std::string& name, bool searchParents = true);
		//SCRIPT_API ScriptFieldDeclaration* get(ScriptScopeBase* scope = nullptr);
		SCRIPT_API ScriptVariable* get(ScriptScopeBase* scope = nullptr) override;
		SCRIPT_API const std::shared_ptr<ScriptType> getType() const override;
		SCRIPT_API void setScope(ScriptScopeBase* scope);
		SCRIPT_API operator bool() const;
	};

	// Function accessor stores a scope and address of given function which
	// may be used at later point to access said function
	// Scope is mutable and may be changed by scoped call, for example by calling
	// function call by scope operator
	class FunctionAccessor
	{
	public:
		SCRIPT_API FunctionAccessor();
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