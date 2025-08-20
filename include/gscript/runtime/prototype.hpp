#ifndef _h_gscript_prototype
#define _h_gscript_prototype

namespace gscript
{
	class ScriptScope;

	// A prototype is any callable script entity which is in semi-complete state
	// meaning that core functionality is in place, but links to concrete memory
	// locations are yet to be established
	// 
	// Every executable part of the script must scan for prototypes in their call
	// queue for prototypes, and upon encountering one - a prototype must be set up
	// before it's good for execution

	template<typename T>
	class Prototype : public T
	{
	public:
		using T::T;

		virtual ~Prototype() = default;
		virtual std::unique_ptr<T> setup(ScriptScope& scope) = 0;
	};

	// A prototype resolver is any script entity which may contain Prototypes
	// A class which inherits PrototypeResolver will have it's setupPrototypes function 
	// called by the compiler after the final compilation stage in order to resolve 
	// dependenties of it's contained prototypes

	class PrototypeResolver
	{
	public:
		virtual ~PrototypeResolver() = default;
		virtual void setupPrototypes(ScriptScope& scope) = 0;
	};
}

#endif