#include "runtime/method.hpp"
#include "runtime/class.hpp"

#include "defs.hpp"
#include "compileException.hpp"

#include <cassert>

namespace gscript
{
	ScriptMethod::ScriptMethod
	(
		ScriptScopeBase& scope,
		const std::string &name,
		std::shared_ptr<ScriptType> returnType,
		const PARAMS_T &parameters,
		Modifier modifier
	)
		: ScriptFunction(scope, name, returnType, parameters)
		, accessModifier(modifier)
	{
		this->createThis(static_cast<ScriptClass&>(scope));
	}

	void ScriptMethod::createThis(ScriptClass &sclass)
	{
		this->registerVariable(std::make_unique<ScriptVariable>(ScriptClass::keywordThis, std::make_unique<ScriptClassType>(sclass), nullptr));
	}

	std::unique_ptr<ScriptValue> ScriptMethod::instrun(std::unique_ptr<ScriptReferenceValue>&& instance, const CALLABLE_PARAMS_T &c)
	{
		this->setClassInstance(std::move(instance));
		return this->run(c);
	}

	bool ScriptMethod::isStatic() const
	{
		return this->accessModifier & Modifier::Static;
	}

	bool ScriptMethod::isVirtual() const
	{
		return this->accessModifier & Modifier::Virtual;
	}

	bool ScriptMethod::isAbstract() const
	{
		return this->accessModifier & Modifier::Abstract;
	}

	void ScriptMethod::setClassInstance(std::unique_ptr<ScriptClassInstance>&& inst)
	{
		this->variables.front()->setValue(std::make_unique<ScriptClassValue>(std::move(inst)));
	}

	void ScriptMethod::setClassInstance(std::unique_ptr<ScriptValue>&& inst)
	{
		assert(inst->getType()->getUnderlyingTypeDescriptor() == ValueType::Class);
		this->variables.front()->setValue(std::move(inst));
	}

	ScriptClassInstance *ScriptMethod::getClassInstance()
	{
		return static_cast<ScriptClassValue*>(this->variables.front()->getValue().get())->getValue().get();
	}
}