#include "runtime/varRead.hpp"
#include "runtime/scope.hpp"
#include "runtime/classInstance.hpp"

#include "compileException.hpp"

namespace gscript
{
	ScriptVarRead::ScriptVarRead(ScriptScope &scope)
		: ScriptCallable(scope)
	{
	}

	ScriptVarRead::ScriptVarRead(ScriptScope& scope, VariableAccessor accessor)
		: ScriptCallable(scope)
		, accessor(accessor)
	{
	}

	ScriptVarRead::ScriptVarRead(ScriptScope& scope, ScriptVariable* variable)
		: ScriptCallable(scope)
		, accessor(VariableAccessor::find(scope, variable->getName()))
	{
	}

	ScriptVarRead::ScriptVarRead(ScriptScope& scope, const std::string& name)
		: ScriptCallable(scope)
		, accessor(VariableAccessor::find(scope, name))
	{
	}

	/*ScriptVarRead::ScriptVarRead(ScriptScope &scope, ScriptVariable *var)
		: ScriptCallable(scope),
		var(std::make_unique<DirectEntityLink<ScriptVariable*>>(var))
	{
	}

	ScriptVarRead::ScriptVarRead(ScriptScope &scope, std::unique_ptr<EntityLink<ScriptVariable*>>&& link)
		: ScriptCallable(scope)
		, var(std::move(link))
	{
	}*/

	ScriptValue *ScriptVarRead::run(const CALLABLE_PARAMS_T &c)
	{
		return this->accessor.get()->getValue();
		//return this->var->get()->getValue();
	}

	ScriptVariable* ScriptVarRead::get()
	{
		return this->accessor.get();
	}

	const ScriptType *ScriptVarRead::getType() const
	{
		return this->accessor.getType();
		//return this->var->orig()->getType();
	}

	void ScriptVarRead::setScope(ScriptClassInstance *instance)
	{	
		this->accessor.setScope(instance);
		//static_cast<MemberEntityLink<ScriptVariable*, ScriptClassInstance::VariableContainer>&>(*this->var).container = &instance->getVariables();
	}

	// Prototype

	ScriptVarReadPrototype::ScriptVarReadPrototype(ScriptScope& scope, const std::string& varname)
		: ScriptCallablePrototype(scope)
		, varname(varname)
	{
	}

	std::unique_ptr<ScriptCallable> ScriptVarReadPrototype::setup()
	{
		return nullptr; // TODO
	}

	// ARRAY VAR READ

	ScriptArrayRead::ScriptArrayRead(ScriptScope &scope, VariableAccessor accessor, std::unique_ptr<ScriptCallable> &&arrayAccessor)
		: ScriptVarRead(scope, accessor)
		, arrayAccessor(std::move(arrayAccessor))
	{
	}

	ScriptArrayRead::ScriptArrayRead(ScriptScope& scope, ScriptVariable* variable, std::unique_ptr<ScriptCallable>&& arrayAccessor)
		: ScriptVarRead(scope, variable)
		, arrayAccessor(std::move(arrayAccessor))
	{
	}

	ScriptValue *ScriptArrayRead::run(const CALLABLE_PARAMS_T &c)
	{
		//ScriptArrayValue *arr = static_cast<ScriptArrayValue*>(this->var->getValue());
		ScriptArrayValue *arr = static_cast<ScriptArrayValue*>(ScriptVarRead::run(c));
		int index = static_cast<ScriptIntValue*>(this->arrayAccessor->run())->getValue();
		return arr->getValue()[index];
	}

	const ScriptType *ScriptArrayRead::getType() const
	{
		return static_cast<const ScriptArrayType*>(this->accessor.getType())->subType;
		//return static_cast<const ScriptArrayType*>(this->var->orig()->getType())->subType;
	}

	//// RESOLVER

	//ScriptVarReadResolver::ScriptVarReadResolver(ScriptScope &scope, const std::string &name)
	//	: ScriptVarRead(scope)
	//	, name(name)
	//{
	//}

	//std::unique_ptr<ScriptVarRead> ScriptVarReadResolver::resolve(ScriptScope &scope, bool member)
	//{
	//	ScriptVariable *var = scope.getVariable(this->name);
	//	//ScriptVarRead *result = new ScriptVarRead(this->scope, scope.getVariable(this->name));

	//	std::unique_ptr<EntityLink<ScriptVariable*>> link = nullptr;
	//	if (member)
	//		link = std::make_unique<MemberEntityLink<ScriptVariable*, ScriptClassInstance::VariableContainer>>(nullptr, var->getInternalPointer(), var);
	//	else
	//		link = std::make_unique<DirectEntityLink<ScriptVariable*>>(var);

	//	return std::make_unique<ScriptVarRead>(this->scope, std::move(link));
	//}

	//// ARRAY READ RESOLV

	//ScriptArrayReadResolver::ScriptArrayReadResolver(ScriptScope &scope, std::unique_ptr<ScriptCallable> &&accessor, const std::string &name)
	//	: ScriptVarReadResolver(scope, name)
	//	, accessor(std::move(accessor))
	//{
	//}

	//std::unique_ptr<ScriptVarRead> ScriptArrayReadResolver::resolve(ScriptScope &scope, bool member)
	//{
	//	return std::make_unique<ScriptArrayRead>(this->scope, scope.getVariable(this->name), std::move(this->accessor));
	//}
}