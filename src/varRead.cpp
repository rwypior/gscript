#include "varRead.hpp"
#include "scope.hpp"
#include "script.hpp"
#include "classInstance.hpp"

#include "compileException.hpp"

#include <iostream>

namespace gscript
{
	ScriptVarRead::ScriptVarRead(ScriptScope &scope)
		:ScriptCallable(scope),
		var(nullptr)
	{ }

	ScriptVarRead::ScriptVarRead(ScriptScope &scope, ScriptVariable *var)
		: ScriptCallable(scope),
		var(new DirectEntityLink<ScriptVariable*>(var))
	{ }

	ScriptVarRead::ScriptVarRead(ScriptScope &scope, EntityLink<ScriptVariable*> *link)
		: ScriptCallable(scope),
		var(link)
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
		static_cast<MemberEntityLink< ScriptVariable*, ScriptClassInstance::INSTANCE_VARIABLES_CONTAINER_T >*>(this->var)->container = &instance->getVariables();
	}

	// ARRAY VAR READ

	ScriptArrayRead::ScriptArrayRead(ScriptScope &scope, ScriptVariable *var, ScriptCallable *accessor)
		:ScriptVarRead(scope, var),
		accessor(accessor)
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

	// RESOLV

	ScriptVarReadResolv::ScriptVarReadResolv(ScriptScope &scope, const std::string &name)
		:ScriptVarRead(scope),
		name(name)
	{ }

	ScriptVarRead *ScriptVarReadResolv::resolve(ScriptScope &scope, bool member)
	{
		ScriptVariable *var = scope.getVariable(this->name);
		//ScriptVarRead *result = new ScriptVarRead(this->scope, scope.getVariable(this->name));

		EntityLink<ScriptVariable*> *link = nullptr;
		if (member)
			link = new MemberEntityLink<ScriptVariable*, ScriptClassInstance::INSTANCE_VARIABLES_CONTAINER_T>(nullptr, var->getInternalPointer(), var);
		else
			link = new DirectEntityLink<ScriptVariable*>(var);

		ScriptVarRead *result = new ScriptVarRead(this->scope, link);

		delete this;

		return result;
	}

	// ARRAY READ RESOLV

	ScriptArrayReadResolv::ScriptArrayReadResolv(ScriptScope &scope, ScriptCallable *accessor, const std::string &name)
		:ScriptVarReadResolv(scope, name),
		accessor(accessor)
	{ }

	ScriptVarRead *ScriptArrayReadResolv::resolve(ScriptScope &scope, bool member)
	{
		ScriptVarRead *result = new ScriptArrayRead(this->scope, scope.getVariable(this->name), this->accessor);

		delete this;

		return result;
	}
}