#ifndef _h_entitylink
#define _h_entitylink

#include "classInstance.hpp"
#include "class.hpp"

namespace gscript
{
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
	protected:
		T target;

	public:
		explicit DirectEntityLink(T target)
			:target(target)
		{ }

		T get() override
		{
			return this->target;
		}
	};

	///
	/// Used to obtain class fields based on their membership decided on assigned class instance
	///
	template<typename T, typename CONTAINER_T>
	class MemberEntityLink : public EntityLink<T>
	{
	public:
		CONTAINER_T * container;
		size_t offset;
		T original;

		explicit MemberEntityLink(CONTAINER_T *container, size_t offset, T original)
			:container(container),
			offset(offset),
			original(original)
		{ }

		T get() override
		{
			return &this->container->at(this->offset);
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
		CONTAINER_T * container;
		size_t offset;
		ScriptVariable& original;

		explicit MemberEntityLink(CONTAINER_T *container, size_t offset, ScriptVariable& original)
			:container(container),
			offset(offset),
			original(original)
		{ }

		ScriptVariable &get() override
		{
			return this->container->at(this->offset);
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
		ScriptClassInstance *instance;

		explicit VirtualEntityLink(T original)
			: original(original)
		{ }

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