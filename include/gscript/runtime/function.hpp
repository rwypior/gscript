#ifndef _h_gscript_function
#define _h_gscript_function

#include "gscript/type.hpp"
#include "gscript/defs.hpp"
#include "gscript/lib.hpp"
#include "gscript/runtime/executiveBlock.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/funcParam.hpp"
#include "gscript/runtime/scope.hpp"

#include <string>
#include <memory>
#include <cassert>

namespace gscript
{
	class ScriptVariable;
	class ScriptValue;
	class ScriptScope;

	class ScriptFunction : public ScriptScope, public ScriptExecutiveBlock, public ScriptCallable
	{
	public:
		SCRIPT_API ScriptFunction(
			ScriptScopeBase& scope, 
			const std::string& name, 
			std::shared_ptr<ScriptType> returnType, 
			const PARAMS_T & parameters = PARAMS_T(),
			std::vector<std::unique_ptr<ScriptCallable>>&& statements = {}
		);

		SCRIPT_API ~ScriptFunction();

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override
		{
			assert(!"This probably should not be implemented");
			return nullptr;
		}

		/// Run the function with dedicated call stack
		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

		/// Run the function the "fast" way - without creating the stack for the call,
		/// This way of running the functions is only suitable for the first run of the
		/// function (eg. non-recursive)
		SCRIPT_API virtual std::unique_ptr<ScriptValue> fastrun(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());
		SCRIPT_API virtual PARAMS_T &getParameters();

		/// Test validity of parameters which are to be passed to the `run` or `fastrun` functions
		SCRIPT_API virtual bool validateParams(const CALLABLE_PARAMS_T &c, bool throwException = true);
		SCRIPT_API FunctionParameter* findParam(const std::string& name);
		SCRIPT_API ScopedAddress findParamAddr(const std::string& name);
		SCRIPT_API ScriptVariable *findVariable(const std::string &name) override;

		/// Check if this function matches given function signature (name and parameters)
		SCRIPT_API bool matches(const std::string &name, const PARAMS_T &parameters);

		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const override;
		SCRIPT_API const std::string &getName() const;

		SCRIPT_API virtual bool isBaseOf(const ScriptScopeBase& scope) const override;

	protected:
		SCRIPT_API ScriptFunction(const ScriptFunction& fnc);

		SCRIPT_API void throwBadParameters(const std::string& text);

		template<typename T>
		T* getParam(ScriptValue* p)
		{
			if (p->getType()->getAbsoluteTypeDescriptor() == VALUE_TYPE_T::VT_REFERENCE)
				return static_cast<ScriptReferenceValue*>(p)->resolve<T*>();

			return static_cast<T*>(p);
		}

		SCRIPT_API void registerParameters(const CALLABLE_PARAMS_T& c);

		const std::string name;
		std::shared_ptr<ScriptType> returnType;
		PARAMS_T parameters;
		const ScriptFunction* baseFunction = nullptr;
	};
}

#endif