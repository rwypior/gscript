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

		static VariableAccessor find(ScriptScopeBase& scope, const std::string& name, bool searchParents = true)
		{
			auto addr = scope.findVariableAddr(name, searchParents);

			if (!addr)
				throw CompileException("Variable \"" + name + "\" not found");

			return VariableAccessor(addr.scope, addr.addr);
		}

		ScriptVariable* get()
		{
			assert(this->scope && "Scope must not be null");

			if (this->addr > this->scope->getVariables().size())
				return nullptr;
			return this->scope->getVariables().at(this->addr).get();
		}

		const ScriptType* getType() const
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

	private:
		ScriptScopeBase* scope = nullptr;
		size_t addr = NullAddr;
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