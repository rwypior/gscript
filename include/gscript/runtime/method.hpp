#ifndef _h_gscript_method
#define _h_gscript_method

#include "function.hpp"
#include "defs.hpp"
#include "lib.hpp"

#include <string>

namespace gscript
{
	class ScriptScope;
	//class ScriptExternFunction;

	class ScriptMethod : public ScriptFunction
	{
	public:
		const MODIFIER_T accessModifier = MODIFIER_T::M_NONE;

		//SCRIPT_API ScriptMethod(ScriptFunction &func, const BITFLAG_T access);

		SCRIPT_API ScriptMethod(
			ScriptScopeBase& scope,
			const std::string &name,
			ScriptType *returnType,
			const PARAMS_T &parameters = PARAMS_T(),
			MODIFIER_T modifier = MODIFIER_T::M_NONE
		);
		
		SCRIPT_API bool isStatic() const;
		SCRIPT_API bool isVirtual() const;
		SCRIPT_API bool isAbstract() const;

		SCRIPT_API void setClassInstance(ScriptClassInstance *inst);
		SCRIPT_API void setClassInstance(ScriptClassValue *inst);
		SCRIPT_API ScriptClassInstance *getClassInstance();

		SCRIPT_API virtual ScriptValue *instrun(ScriptClassInstance *instance, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T());

	protected:
		void createThis(ScriptClass &sclass);
	};

	///
	/// METHOD PROTOTYPE
	/// Method prototype holds parsing data used to create full method type when
	/// all names can be resolved
	///
	/*class ScriptMethodPrototype : public ScriptFunctionPrototype
	{
	public:
		SCRIPT_API ScriptMethodPrototype(ScriptMethod &target, const ParserMethod &pfunc);
	};*/
	
	///
	/// EXTERN METHOD
	/// Extern method holds reference to unmanaged methods, referenced by script
	/// by using @extern directive
	///
	//class ScriptExternMethod : public ScriptMethod
	//{
	//public:
	//	SCRIPT_API ScriptExternMethod(
	//		const std::string &externName,
	//		ScriptScope &scope,
	//		const std::string &name,
	//		ScriptType *returnType,
	//		const PARAMS_T &parameters = PARAMS_T(),
	//		BITFLAG_T modifier = static_cast<BITFLAG_T>(MODIFIER_T::M_NONE)
	//	);

	//	SCRIPT_API virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;

	//protected:
	//	ScriptExternFunction &target;
	//};
}

#endif