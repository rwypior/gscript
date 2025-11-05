#ifndef _h_gscript_method
#define _h_gscript_method

#include "gscript/defs.hpp"
#include "gscript/lib.hpp"
#include "gscript/runtime/function.hpp"

#include <string>

namespace gscript
{
	class ScriptScope;

	class ScriptMethod : public ScriptFunction
	{
	public:
		const Modifier accessModifier = Modifier::None;

		SCRIPT_API ScriptMethod(
			ScriptScopeBase& scope,
			const std::string &name,
			std::shared_ptr<ScriptType> returnType,
			const PARAMS_T &parameters = PARAMS_T(),
			Modifier modifier = Modifier::None
		);
		
		SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T& c = CALLABLE_PARAMS_T()) override;

		SCRIPT_API bool isStatic() const;
		SCRIPT_API bool isVirtual() const;
		SCRIPT_API bool isAbstract() const;

		SCRIPT_API ScriptClassValue& setClassInstance(std::unique_ptr<ScriptClassInstance>&& inst);
		SCRIPT_API ScriptClassValue& setClassInstance(std::unique_ptr<ScriptValue>&& inst);
		SCRIPT_API ScriptClassInstance *getClassInstance();

		SCRIPT_API ScriptVariable& getThis() const;

		// Run this method from the class instance
		SCRIPT_API virtual std::unique_ptr<ScriptValue> instrun(std::unique_ptr<ScriptReferenceValue>&& instance, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());

	protected:
		SCRIPT_API ScriptMethod(const ScriptMethod& fnc);

		SCRIPT_API void createThis(ScriptClass &sclass);
	};
}

#endif