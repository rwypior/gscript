#ifndef _h_gscript_entitylink
#define _h_gscript_entitylink

#include "classInstance.hpp"
#include "class.hpp"
#include "scope.hpp"
#include "compileException.hpp"

#include <cassert>

namespace gscript
{
	// Variable accessor stores a scope and address of given variable which
	// may be used at later point to access said variable
	// Scope is mutable and may be changed by scoped call, for example by calling
	// variable read by scope operator
	class VariableAccessor
	{
	public:
		virtual ~VariableAccessor() = default;
		VariableAccessor() = default;
		VariableAccessor(ScriptScopeBase* scope, size_t addr)
			: scope(scope)
			, addr(addr)
		{
		}
		VariableAccessor(const VariableAccessor& b)
			: scope(b.scope)
			, addr(b.addr)
		{
		}

		static std::unique_ptr<VariableAccessor> find(ScriptScopeBase& scope, const std::string& name, bool searchParents = true)
		{
			auto addr = scope.findVariableAddr(name, searchParents);

			if (!addr)
				throw CompileException("Variable \"" + name + "\" not found");

			return std::make_unique<VariableAccessor>(addr.scope, addr.addr);
		}

		virtual ScriptVariable* get()
		{
			assert(this->scope && "Scope must not be null");

			if (this->addr > this->scope->getVariables().size())
				return nullptr;
			return this->scope->getVariables().at(this->addr).get();
		}

		virtual const ScriptType* getType() const
		{
			assert(this->scope && "Scope must not be null");

			if (this->addr > this->scope->getVariables().size())
				return nullptr;
			return this->scope->getVariables().at(this->addr)->getType();
		}

		void setScope(ScriptScopeBase* scope)
		{
			this->scope = scope;
		}

		operator bool() const
		{
			return this->scope && this->addr != NullAddr;
		}

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

		static std::unique_ptr<ParameterAccessor> find(ScriptScopeBase& scope, const std::string& name, bool searchParents = true)
		{
			assert(dynamic_cast<ScriptFunction*>(&scope) && "Parameters may only be used in functions");
			ScriptFunction& fnc = static_cast<ScriptFunction&>(scope);

			auto addr = fnc.findParamAddr(name);

			if (!addr)
				throw CompileException("Variable \"" + name + "\" not found");

			return std::make_unique<ParameterAccessor>(addr.scope, addr.addr);
		}

		ScriptVariable* get()
		{
			assert(this->scope && "Scope must not be null");
			assert(dynamic_cast<ScriptFunction*>(this->scope) && "Parameters may only be used in functions");

			ScriptFunction* fnc = static_cast<ScriptFunction*>(this->scope);

			if (this->addr > fnc->getParameters().size())
				return nullptr;
			return &fnc->getParameters().at(this->addr);
		}

		const ScriptType* getType() const
		{
			assert(this->scope && "Scope must not be null");
			assert(dynamic_cast<ScriptFunction*>(this->scope) && "Parameters may only be used in functions");

			ScriptFunction* fnc = static_cast<ScriptFunction*>(this->scope);

			if (this->addr > this->scope->getVariables().size())
				return nullptr;
			return fnc->getParameters().at(this->addr).getType();
		}
	};

	// Function accessor stores a scope and address of given function which
	// may be used at later point to access said function
	// Scope is mutable and may be changed by scoped call, for example by calling
	// function call by scope operator
	class FunctionAccessor
	{
	public:
		FunctionAccessor() = default;
		FunctionAccessor(ScriptScopeBase* scope, size_t addr)
			: scope(scope)
			, addr(addr)
		{
		}
		FunctionAccessor(const FunctionAccessor& b)
			: scope(b.scope)
			, addr(b.addr)
		{
		}

		static FunctionAccessor find(ScriptScopeBase& scope, const std::string& name, const PARAMS_T& params, bool searchParents = true)
		{
			auto addr = scope.findFunctionAddr(name, params, searchParents);

			if (!addr)
				throw CompileException("Function \"" + name + "\" not found");

			return FunctionAccessor(addr.scope, addr.addr);
		}

		ScriptFunction* get()
		{
			assert(this->scope && "Scope must not be null");

			if (this->addr > this->scope->getFunctions().size())
				return nullptr;
			return this->scope->getFunctions().at(this->addr).get();
		}

		const ScriptType* getType() const
		{
			assert(this->scope && "Scope must not be null");

			if (this->addr > this->scope->getFunctions().size())
				return nullptr;
			return this->scope->getFunctions().at(this->addr)->getType();
		}

		void setScope(ScriptScopeBase* scope)
		{
			this->scope = scope;
		}

		operator bool() const
		{
			return this->scope && this->addr != NullAddr;
		}

	private:
		ScriptScopeBase* scope = nullptr;
		size_t addr = NullAddr;
	};
}

#endif