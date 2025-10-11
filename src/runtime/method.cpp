#include "runtime/method.hpp"
#include "runtime/class.hpp"
#include "runtime/classInstance.hpp"

#include "defs.hpp"
#include "compileException.hpp"

#include <cassert>

namespace gscript
{
	ScriptMethod::ScriptMethod(const ScriptMethod& fnc)
		: ScriptFunction(fnc)
	{
	}

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

	std::unique_ptr<ScriptValue> ScriptMethod::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T& c)
	{
		assert(this->getThis().getValue() && "Methods must be run by instrun");

		/*auto inst = static_cast<ScriptClassInstance&>(scope);
		c.front()*/
		
		/*auto instref = std::make_unique<ScriptReferenceValue>(std::make_shared<ScriptReferenceType>(std::make_shared<ScriptClassType>(inst.getClass())), &scope);
		ScriptReferenceValue(std::make_shared<ScriptReferenceType>(std::make_shared<ScriptClassType>(inst.getClass())), &scope);
		this->getThis().setValue(std::move(instref));*/

		this->validateParams(c);

		ScriptMethod target(*this);
		target.registerParameters(c);
		return this->execute(target);
	}

	void ScriptMethod::createThis(ScriptClass &sclass)
	{
		this->registerVariable(std::make_unique<ScriptVariable>(ScriptClass::keywordThis, std::make_unique<ScriptClassType>(sclass), nullptr));
	}

	std::unique_ptr<ScriptValue> ScriptMethod::instrun(std::unique_ptr<ScriptReferenceValue>&& instance, const CALLABLE_PARAMS_T &c)
	{
		auto& inst = this->setClassInstance(std::move(instance));
		return this->run(*inst.getValue(), c);
		//return this->run(*instance->data()->as<ScriptClassValue>().getValue(), c);
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

	ScriptClassValue& ScriptMethod::setClassInstance(std::unique_ptr<ScriptClassInstance>&& inst)
	{
		this->variables.front()->setValue(std::make_unique<ScriptClassValue>(std::move(inst)));
		return this->variables.front()->getValue()->as<ScriptClassValue>();
	}

	ScriptClassValue& ScriptMethod::setClassInstance(std::unique_ptr<ScriptValue>&& inst)
	{
		assert(inst->getType()->getUnderlyingTypeDescriptor() == ValueType::Class);
		this->variables.front()->setValue(std::move(inst));
		return this->variables.front()->getValue()->as<ScriptClassValue>();
	}

	ScriptClassInstance *ScriptMethod::getClassInstance()
	{
		return static_cast<ScriptClassValue*>(this->variables.front()->getValue().get())->getValue().get();
	}

	ScriptVariable& ScriptMethod::getThis() const
	{
		return *this->variables.front();
	}
}