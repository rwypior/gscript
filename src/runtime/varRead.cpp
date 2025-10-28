#include "runtime/varRead.hpp"
#include "runtime/scope.hpp"
#include "runtime/classInstance.hpp"

#include "compileException.hpp"

namespace gscript
{
	// TODO - remove 'scope' argument from constructors - leave only the one with accessor

	ScriptVarRead::ScriptVarRead(const ScriptVarRead& b)
		: accessor(b.accessor->clone())
	{
	}

	ScriptVarRead::ScriptVarRead(ScriptScopeBase& scope, std::unique_ptr<VariableAccessor>&& accessor)
		: accessor(std::move(accessor))
	{
	}

	ScriptVarRead::ScriptVarRead(ScriptScopeBase& scope, ScriptVariable* variable)
		: accessor(VariableAccessor::find(scope, variable->getName()))
	{
	}

	ScriptVarRead::ScriptVarRead(ScriptScopeBase& scope, const std::string& name)
		: accessor(VariableAccessor::find(scope, name))
	{
	}

	std::unique_ptr<ScriptCallable> ScriptVarRead::clone()
	{
		return std::make_unique<ScriptVarRead>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptVarRead::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->accessor->get(&scope)->getValue()->clone();
	}

	ScriptVariable* ScriptVarRead::get()
	{
		return this->accessor->get();
	}

	const std::shared_ptr<ScriptType> ScriptVarRead::getType() const
	{
		return this->accessor->getType();
	}

	void ScriptVarRead::setScope(ScriptClassInstance *instance)
	{	
		this->accessor->setScope(instance);
	}

	// Reference read
	
	std::unique_ptr<ScriptCallable> ScriptVarReferenceRead::clone()
	{
		return std::make_unique<ScriptVarReferenceRead>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptVarReferenceRead::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T& c)
	{
		auto& val = this->accessor->get(&scope)->getValue();
		return std::make_unique<ScriptReferenceValue>(std::make_shared<ScriptReferenceType>(val->getType()), val.get());
	}

	// Prototype

	ScriptVarReadPrototype::ScriptVarReadPrototype(ScriptScopeBase& scope, const std::string& varname)
		: varname(varname)
	{
	}

	std::unique_ptr<ScriptCallable> ScriptVarReadPrototype::build(ScriptScopeBase& scope)
	{
		std::unique_ptr<ScriptVarRead> result;

		try
		{
			if (this->isReference)
				result = std::make_unique<ScriptVarReferenceRead>(scope, VariableAccessor::find(scope, this->varname));
			else
				result = std::make_unique<ScriptVarRead>(scope, VariableAccessor::find(scope, this->varname));
		}
		catch (...)
		{
			try
			{
				if (this->isReference)
					result = std::make_unique<ScriptVarReferenceRead>(scope, ParameterAccessor::find(scope, this->varname));
				else
					result = std::make_unique<ScriptVarRead>(scope, ParameterAccessor::find(scope, this->varname));
			}
			catch (...)
			{
				throw;
			}
		}

		return result;
	}

	const std::string& ScriptVarReadPrototype::getName() const
	{
		return this->varname;
	}

	void ScriptVarReadPrototype::disableReference()
	{
		this->isReference = false;
	}

	// Array var read

	ScriptArrayRead::ScriptArrayRead(const ScriptArrayRead& b)
		: ScriptVarRead(b)
		, arrayAccessor(b.arrayAccessor->clone())
	{
	}

	ScriptArrayRead::ScriptArrayRead(ScriptScopeBase& scope, std::unique_ptr<VariableAccessor>&& accessor, std::unique_ptr<ScriptCallable> &&arrayAccessor)
		: ScriptVarRead(scope, std::move(accessor))
		, arrayAccessor(std::move(arrayAccessor))
	{
	}

	ScriptArrayRead::ScriptArrayRead(ScriptScopeBase& scope, ScriptVariable* variable, std::unique_ptr<ScriptCallable>&& arrayAccessor)
		: ScriptVarRead(scope, variable)
		, arrayAccessor(std::move(arrayAccessor))
	{
	}

	ScriptArrayRead::ScriptArrayRead(ScriptScopeBase& scope, const std::string& name, std::unique_ptr<ScriptCallable>&& arrayAccessor)
		: ScriptVarRead(scope, name)
		, arrayAccessor(std::move(arrayAccessor))
	{
	}

	std::unique_ptr<ScriptCallable> ScriptArrayRead::clone()
	{
		return std::make_unique<ScriptArrayRead>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptArrayRead::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		auto var = ScriptVarRead::run(scope, c);
		ScriptArrayValue* arr = static_cast<ScriptArrayValue*>(var->data());
		int index = static_cast<ScriptIntValue*>(this->arrayAccessor->run(scope).get())->getValue();

		// TODO - check this for performance - same as for VarRead
		return arr->getValue()[index]->clone();
	}

	const std::shared_ptr<ScriptType> ScriptArrayRead::getType() const
	{
		return std::static_pointer_cast<const ScriptArrayType>(this->accessor->getType())->getSubType();
	}

	// Array var read prototype

	ScriptArrayReadPrototype::ScriptArrayReadPrototype(ScriptScopeBase& scope, const std::string& varname, std::unique_ptr<ScriptCallable>&& arrayAccessor)
		: varname(varname)
		, arrayAccessor(std::move(arrayAccessor))
	{
	}

	std::unique_ptr<ScriptCallable> ScriptArrayReadPrototype::build(ScriptScopeBase& scope)
	{
		auto result = std::make_unique<ScriptArrayRead>(scope, VariableAccessor::find(scope, this->varname), std::move(this->arrayAccessor));
		return result;
	}
}