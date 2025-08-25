#ifndef _h_gscript_function
#define _h_gscript_function

#include "executiveBlock.hpp"
#include "callable.hpp"
#include "funcParam.hpp"
#include "type.hpp"
#include "defs.hpp"
#include "lib.hpp"
#include "scope.hpp"

#include <string>

namespace gscript
{
	class ScriptVariable;
	class ScriptValue;
	class ScriptScope;

	class ScriptFunction : public ScriptScope, public ScriptExecutiveBlock, public ScriptCallable
	{
	public:
		//SCRIPT_API ScriptFunction(const ScriptFunction& fnc);
		SCRIPT_API ScriptFunction(
			ScriptScope& scope, 
			const std::string& name, 
			ScriptType* returnType, 
			const PARAMS_T & parameters = PARAMS_T(),
			std::vector<std::shared_ptr<ScriptCallable>>&& statements = {}
		);
		SCRIPT_API ~ScriptFunction();

		SCRIPT_API virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual PARAMS_T &getParameters();
		SCRIPT_API virtual bool validateParams(const CALLABLE_PARAMS_T &c, bool throwException = true);
		SCRIPT_API ScriptVariable *findVariable(const std::string &name) override;

		SCRIPT_API bool matches(const std::string &name, const PARAMS_T &parameters);

		SCRIPT_API virtual const ScriptType *getType() const override;
		SCRIPT_API const std::string &getName() const;

		SCRIPT_API size_t getInternalPointer() const
		{
			return this->internalPointer;
		}

	protected:
		void throwBadParameters(const std::string& text);

		size_t internalPointer = 0;

		template<typename T>
		T* getParam(ScriptValue* p)
		{
			if (p->getType()->getAbsoluteTypeDescriptor() == VALUE_TYPE_T::VT_REFERENCE)
				return static_cast<ScriptReferenceValue*>(p)->resolve<T*>();

			return static_cast<T*>(p);
		}

		void registerParameters(const CALLABLE_PARAMS_T& c);

		const std::string name;
		ScriptType* returnType = nullptr;
		PARAMS_T parameters;
	};

	///
	/// Used to hold place for function until all classes are registered
	/// This is neccessary so all function and class names are registered
	/// and visible.
	///
	
	// TODO
	/*class ScriptFunctionPrototype
	{
	public:
		SCRIPT_API ScriptFunctionPrototype(ScriptFunction &target, const ParserFunction &pfunc);

		SCRIPT_API void build();

	protected:
		ScriptFunction &target;
		const ParserFunction &pfunc;
	};*/

	///
	/// Used to represent functions created on host-side. Overrides setup
	/// function to act like prototype, but without usage of parser entities.
	///
	//class ScriptExternFunction : public ScriptFunction
	//{
	//public:
	//	using ScriptFunction::ScriptFunction;

	//	SCRIPT_API virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override = 0;
	//	virtual void setup() {};
	//};

	///
	/// Used to map internal function representation to existing registered
	/// external function. Works the same as ScriptExternMethod.
	///
	//class ScriptExternFunctionConnector : public ScriptFunction
	//{
	//public:
	//	SCRIPT_API ScriptExternFunctionConnector(const std::string &externName, ScriptScope &scope, const std::string &name, ScriptType *returnType, const PARAMS_T &parameters = PARAMS_T());

	//	SCRIPT_API virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

	//protected:
	//	ScriptExternFunction &target;
	//};
}

#endif