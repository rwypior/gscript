#include "runtime/varRead.hpp"
#include "runtime/scope.hpp"
#include "runtime/classInstance.hpp"

#include "compileException.hpp"

namespace gscript
{
	ScriptVarRead::ScriptVarRead(ScriptScope &scope)
		:ScriptCallable(scope),
		var(nullptr)
	{ }

	ScriptVarRead::ScriptVarRead(ScriptScope &scope, ScriptVariable *var)
		: ScriptCallable(scope),
		var(std::make_unique<DirectEntityLink<ScriptVariable*>>(var))
	{ }

	ScriptVarRead::ScriptVarRead(ScriptScope &scope, std::unique_ptr<EntityLink<ScriptVariable*>>&& link)
		: ScriptCallable(scope)
		, var(std::move(link))
	{

	}

	ScriptValue *ScriptVarRead::run(const CALLABLE_PARAMS_T &c)
	{
		return this->var->get()->getValue();
	}

	const ScriptType *ScriptVarRead::getType() const
	{
		return this->var->orig()->getType();
	}

	void ScriptVarRead::setScope(ScriptClassInstance *instance)
	{	
		static_cast<MemberEntityLink<ScriptVariable*, ScriptClassInstance::VariableContainer>&>(*this->var).container = &instance->getVariables();
	}

	// ARRAY VAR READ

	ScriptArrayRead::ScriptArrayRead(ScriptScope &scope, ScriptVariable *var, std::unique_ptr<ScriptCallable> &&accessor)
		: ScriptVarRead(scope, var)
		, accessor(std::move(accessor))
	{
	}

	ScriptValue *ScriptArrayRead::run(const CALLABLE_PARAMS_T &c)
	{
		//ScriptArrayValue *arr = static_cast<ScriptArrayValue*>(this->var->getValue());
		ScriptArrayValue *arr = static_cast<ScriptArrayValue*>(ScriptVarRead::run(c));
		int index = static_cast<ScriptIntValue*>(this->accessor->run())->getValue();
		return arr->getValue()[index];
	}

	const ScriptType *ScriptArrayRead::getType() const
	{
		return static_cast<const ScriptArrayType*>(this->var->orig()->getType())->subType;
	}

	// RESOLVER

	ScriptVarReadResolver::ScriptVarReadResolver(ScriptScope &scope, const std::string &name)
		: ScriptVarRead(scope)
		, name(name)
	{
	}

	std::unique_ptr<ScriptVarRead> ScriptVarReadResolver::resolve(ScriptScope &scope, bool member)
	{
		ScriptVariable *var = scope.getVariable(this->name);
		//ScriptVarRead *result = new ScriptVarRead(this->scope, scope.getVariable(this->name));

		std::unique_ptr<EntityLink<ScriptVariable*>> link = nullptr;
		if (member)
			link = std::make_unique<MemberEntityLink<ScriptVariable*, ScriptClassInstance::VariableContainer>>(nullptr, var->getInternalPointer(), var);
		else
			link = std::make_unique<DirectEntityLink<ScriptVariable*>>(var);

		return std::make_unique<ScriptVarRead>(this->scope, std::move(link));
	}

	// ARRAY READ RESOLV

	ScriptArrayReadResolver::ScriptArrayReadResolver(ScriptScope &scope, std::unique_ptr<ScriptCallable> &&accessor, const std::string &name)
		: ScriptVarReadResolver(scope, name)
		, accessor(std::move(accessor))
	{
	}

	std::unique_ptr<ScriptVarRead> ScriptArrayReadResolver::resolve(ScriptScope &scope, bool member)
	{
		return std::make_unique<ScriptArrayRead>(this->scope, scope.getVariable(this->name), std::move(this->accessor));
	}
}