#include "runtime/varRead.hpp"
#include "runtime/scope.hpp"
#include "runtime/classInstance.hpp"

#include "compileException.hpp"

namespace gscript
{
	ScriptVarRead::ScriptVarRead(ScriptScope& scope)
		: ScriptCallable(scope)
	{
	}

	ScriptVarRead::ScriptVarRead(ScriptScope& scope, std::unique_ptr<VariableAccessor>&& accessor)
		: ScriptCallable(scope)
		, accessor(std::move(accessor))
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

	std::unique_ptr<ScriptValue> ScriptVarRead::run(const CALLABLE_PARAMS_T &c)
	{
		// TODO - check this for performance
		// 1. Test if reference reads work correctly
		auto& val = this->accessor->get()->getValue();
		return std::make_unique<ScriptReferenceValue>(std::make_shared<ScriptReferenceType>(val->getType()), val.get());
		//return this->accessor->get()->getValue()->clone();
	}

	ScriptVariable* ScriptVarRead::get()
	{
		return this->accessor->get();
	}

	const std::shared_ptr<ScriptType> ScriptVarRead::getType() const
	{
		return this->accessor->getType();
		//return this->var->orig()->getType();
	}

	void ScriptVarRead::setScope(ScriptClassInstance *instance)
	{	
		this->accessor->setScope(instance);
		//static_cast<MemberEntityLink<ScriptVariable*, ScriptClassInstance::VariableContainer>&>(*this->var).container = &instance->getVariables();
	}

	// Prototype

	ScriptVarReadPrototype::ScriptVarReadPrototype(ScriptScope& scope, const std::string& varname)
		: ScriptCallablePrototype(scope)
		, varname(varname)
	{
	}

	std::unique_ptr<ScriptCallable> ScriptVarReadPrototype::build(ScriptScopeBase* scope)
	{
		ScriptScope* usedScope = static_cast<ScriptScope*>(scope ? scope : &this->scope);

		std::unique_ptr<ScriptVarRead> result;

		try
		{
			result = std::make_unique<ScriptVarRead>(*usedScope, VariableAccessor::find(*usedScope, this->varname));
		}
		catch (...)
		{
			try
			{
				result = std::make_unique<ScriptVarRead>(*usedScope, ParameterAccessor::find(*usedScope, this->varname));
			}
			catch (...)
			{
				throw;
			}
		}
		//auto result = std::make_unique<ScriptVarRead>(*usedScope, VariableAccessor::find(*usedScope, this->varname));

		return result;
	}

	// Array var read

	ScriptArrayRead::ScriptArrayRead(ScriptScope& scope, std::unique_ptr<VariableAccessor>&& accessor, std::unique_ptr<ScriptCallable> &&arrayAccessor)
		: ScriptVarRead(scope, std::move(accessor))
		, arrayAccessor(std::move(arrayAccessor))
	{
	}

	ScriptArrayRead::ScriptArrayRead(ScriptScope& scope, ScriptVariable* variable, std::unique_ptr<ScriptCallable>&& arrayAccessor)
		: ScriptVarRead(scope, variable)
		, arrayAccessor(std::move(arrayAccessor))
	{
	}

	ScriptArrayRead::ScriptArrayRead(ScriptScope& scope, const std::string& name, std::unique_ptr<ScriptCallable>&& arrayAccessor)
		: ScriptVarRead(scope, name)
		, arrayAccessor(std::move(arrayAccessor))
	{
	}

	std::unique_ptr<ScriptValue> ScriptArrayRead::run(const CALLABLE_PARAMS_T &c)
	{
		ScriptArrayValue *arr = static_cast<ScriptArrayValue*>(ScriptVarRead::run(c).get());
		int index = static_cast<ScriptIntValue*>(this->arrayAccessor->run().get())->getValue();

		// TODO - check this for performance - same as for VarRead
		return arr->getValue()[index]->clone();
	}

	const std::shared_ptr<ScriptType> ScriptArrayRead::getType() const
	{
		return std::static_pointer_cast<const ScriptArrayType>(this->accessor->getType())->getSubType();
	}

	// Array var read prototype

	ScriptArrayReadPrototype::ScriptArrayReadPrototype(ScriptScope& scope, const std::string& varname, std::unique_ptr<ScriptCallable>&& arrayAccessor)
		: ScriptCallablePrototype(scope)
		, varname(varname)
		, arrayAccessor(std::move(arrayAccessor))
	{
	}

	std::unique_ptr<ScriptCallable> ScriptArrayReadPrototype::build(ScriptScopeBase* scope)
	{
		ScriptScope* usedScope = static_cast<ScriptScope*>(scope ? scope : &this->scope);

		auto result = std::make_unique<ScriptArrayRead>(*usedScope, VariableAccessor::find(*usedScope, this->varname), std::move(this->arrayAccessor));

		return result;
	}
}