#include "runtime/method.hpp"
#include "runtime/class.hpp"

#include "defs.hpp"
#include "compileException.hpp"

namespace gscript
{
	/*ScriptMethod::ScriptMethod(ScriptFunction &func, BITFLAG_T modifier)
		: ScriptFunction(func)
		, accessModifier(modifier)
	{
		this->createThis(static_cast<ScriptClass&>(func.getScope()));
	}*/

	ScriptMethod::ScriptMethod
	(
		ScriptScopeBase& scope,
		const std::string &name,
		ScriptType *returnType,
		const PARAMS_T &parameters,
		MODIFIER_T modifier
	)
		: ScriptFunction(scope, name, returnType, parameters)
		, accessModifier(modifier)
	{
		this->createThis(static_cast<ScriptClass&>(scope));
	}

	void ScriptMethod::createThis(ScriptClass &sclass)
	{
		//this->registerVariable(ScriptClass::KW_THIS, new ScriptClassType(static_cast<ScriptClass&>(func.getScope())), NULL);
		//this->registerVariable(ScriptClass::KW_THIS, new ScriptClassType(sclass), NULL);
		this->registerVariable(std::make_unique<ScriptVariable>(ScriptClass::KW_THIS, new ScriptClassType(sclass), nullptr));
	}

	ScriptValue *ScriptMethod::instrun(ScriptClassInstance *instance, const CALLABLE_PARAMS_T &c)
	{
		this->setClassInstance(instance);
		return this->run(c);
	}

	bool ScriptMethod::isStatic() const
	{
		return this->accessModifier & MODIFIER_T::M_STATIC;
	}

	bool ScriptMethod::isVirtual() const
	{
		return this->accessModifier & MODIFIER_T::M_VIRTUAL;
	}

	bool ScriptMethod::isAbstract() const
	{
		return this->accessModifier & MODIFIER_T::M_ABSTRACT;
	}

	void ScriptMethod::setClassInstance(ScriptClassInstance *inst)
	{
		this->variables.front()->setValue(new ScriptClassValue(inst));
	}

	void ScriptMethod::setClassInstance(ScriptClassValue *inst)
	{
		this->variables.front()->setValue(inst);
	}

	ScriptClassInstance *ScriptMethod::getClassInstance()
	{
		return static_cast<ScriptClassValue*>(this->variables.front()->getValue())->getValue();
	}

	// PROTOTYPE

	/*ScriptMethodPrototype::ScriptMethodPrototype(ScriptMethod &target, const ParserMethod &pmethod)
		: ScriptFunctionPrototype(target, pmethod)
	{
	}*/

	// EXTERN

	//ScriptExternMethod::ScriptExternMethod
	//(
	//	const std::string &externName,
	//	ScriptScope &scope,
	//	const std::string &name,
	//	ScriptType *returnType,
	//	const PARAMS_T &parameters,
	//	BITFLAG_T modifier
	//)
	//	: ScriptMethod(scope, name, returnType, parameters, modifier)
	//	, target(scope.getExternFunction(externName))
	//{
	//}

	//ScriptValue *ScriptExternMethod::run(const CALLABLE_PARAMS_T &c)
	//{
	//	return this->target.run(c);
	//}
}