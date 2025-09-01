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
	// may be user at later point to access said variable
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

		static VariableAccessor find(ScriptScopeBase& scope, const std::string& name)
		{
			auto addr = scope.findVariableAddr(name);

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

	private:
		ScriptScopeBase* scope = nullptr;
		size_t addr = NullAddr;
	};




	template<typename T>
	class EntityLink
	{
	public:
		virtual ~EntityLink() = default;

		virtual T get() = 0;
		virtual T orig()
		{
			return this->get();
		}

		operator T()
		{
			return this->get();
		}
	};

	///
	/// Transparent entity linkage function used to directly obtain both class and non-class members
	///
	template<typename T>
	class DirectEntityLink : public EntityLink<T>
	{
	public:
		explicit DirectEntityLink(T target)
			: target(target)
		{ }

		T get() override
		{
			return this->target;
		}

	protected:
		T target;
	};

	///
	/// Used to obtain class fields based on their membership decided on assigned class instance
	///
	template<typename T, typename CONTAINER_T>
	class MemberEntityLink : public EntityLink<T>
	{
	public:
		CONTAINER_T *container = nullptr;
		size_t offset = 0;
		T original;

		explicit MemberEntityLink(CONTAINER_T *container, size_t offset, T original)
			: container(container)
			, offset(offset)
			, original(original)
		{ }

		T get() override
		{
			return this->container->at(this->offset).get();
		}

		T orig() override
		{
			return this->original;
		}
	};

	///
	/// Used to obtain class fields based on their membership decided on assigned class instance
	///
	template<typename CONTAINER_T>
	class MemberEntityLink<ScriptVariable&, CONTAINER_T> : public EntityLink<ScriptVariable&>
	{
	public:
		CONTAINER_T *container = nullptr;
		size_t offset = 0;
		ScriptVariable& original;

		explicit MemberEntityLink(CONTAINER_T *container, size_t offset, ScriptVariable& original)
			: container(container)
			, offset(offset)
			, original(original)
		{ }

		ScriptVariable &get() override
		{
			return *this->container->at(this->offset);
		}

		ScriptVariable &orig() override
		{
			return this->original;
		}
	};

	///
	/// Used to provide on-the-go resolving of virtual functions based on their
	/// parent class inheritance chain obtained from assigned class instances
	///
	template<typename T>
	class VirtualEntityLink : public EntityLink<T>
	{
	public:
		T original;
		ScriptClassInstance *instance = nullptr;

		explicit VirtualEntityLink(T original)
			: original(original)
		{
		}

		T get() override
		{
			ScriptMethod *m = static_cast<ScriptMethod*>(this->original);
			return this->instance->getClass().getFunction(m->getName(), m->getParameters());
		}

		void setInstance(ScriptClassInstance *instance)
		{
			this->instance = instance;
		}
	};
}

#endif