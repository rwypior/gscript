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

		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		SCRIPT_API virtual std::unique_ptr<ScriptValue> fastrun(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());
		SCRIPT_API virtual PARAMS_T &getParameters();
		SCRIPT_API virtual bool validateParams(const CALLABLE_PARAMS_T &c, bool throwException = true);
		SCRIPT_API FunctionParameter* findParam(const std::string& name);
		SCRIPT_API ScopedAddress findParamAddr(const std::string& name);
		SCRIPT_API ScriptVariable *findVariable(const std::string &name) override;

		SCRIPT_API bool matches(const std::string &name, const PARAMS_T &parameters);

		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() const override;
		SCRIPT_API const std::string &getName() const;

		SCRIPT_API virtual bool isBaseOf(const ScriptScopeBase& scope) const override;

	protected:
		SCRIPT_API ScriptFunction(const ScriptFunction& fnc);

		void throwBadParameters(const std::string& text);

		template<typename T>
		T* getParam(ScriptValue* p)
		{
			if (p->getType()->getAbsoluteTypeDescriptor() == VALUE_TYPE_T::VT_REFERENCE)
				return static_cast<ScriptReferenceValue*>(p)->resolve<T*>();

			return static_cast<T*>(p);
		}

		void registerParameters(const CALLABLE_PARAMS_T& c);

		const std::string name;
		std::shared_ptr<ScriptType> returnType;
		PARAMS_T parameters;
		const ScriptFunction* baseFunction = nullptr;
	};
}

#endif