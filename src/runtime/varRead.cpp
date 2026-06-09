#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/scope.hpp"
#include "gscript/runtime/classInstance.hpp"

#include "gscript/compileException.hpp"

namespace
{
	bool accessorReturnedWarning(std::unique_ptr<gscript::VariableAccessor>& accessor)
	{
		auto invalid = dynamic_cast<gscript::InvalidAccessor*>(accessor.get());
		if (!invalid)
			return false;
		switch (invalid->getCode())
		{
		case gscript::VariableAccessorCode::InvalidFieldContext:
		case gscript::VariableAccessorCode::InvalidParamContext:
			return true;
		}
		return false;
	}
}

namespace gscript
{
	ScriptVarRead::ScriptVarRead(const ScriptVarRead& b)
		: accessor(b.accessor->clone())
	{
	}

	ScriptVarRead::ScriptVarRead(std::unique_ptr<VariableAccessor>&& accessor)
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
		assert(!accessorReturnedWarning(this->accessor) && "Accessor returned warning, meaning invalid context was probably used");
		if (!*accessor)
			throw VariableNotFoundException(name);
	}

	std::unique_ptr<ScriptCallable> ScriptVarRead::clone()
	{
		return std::make_unique<ScriptVarRead>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptVarRead::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		// TODO - this should probably return reference;
		// Maybe, only when required
		// 
		//return std::make_unique<ScriptReferenceValue>(this->accessor->get(&scope)->getValue().get());
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
		// Note: keep in mind this can return a reference to a reference
		auto& val = this->accessor->get(&scope)->getValue();
		return std::make_unique<ScriptReferenceValue>(std::make_shared<ScriptReferenceType>(val->getType()), val->data());
	}

	// Prototype

	ScriptVarReadPrototype::ScriptVarReadPrototype(const std::string& varname)
		: varname(varname)
	{
	}

	std::unique_ptr<ScriptCallable> ScriptVarReadPrototype::build(ScriptScopeBase& scope)
	{
		std::unique_ptr<VariableAccessor> accessor = VariableAccessor::find(scope, this->varname);
		if (!*accessor)
			accessor = ParameterAccessor::find(scope, this->varname);
		if (!*accessor)
			accessor = FieldAccessor::find(scope, this->varname);

		assert(!accessorReturnedWarning(accessor) && "Accessor returned warning, meaning invalid context was probably used");

		if (!*accessor)
			throw VariableNotFoundException(this->varname);

		std::unique_ptr<ScriptVarRead> result;
		if (this->isReference)
			return std::make_unique<ScriptVarReferenceRead>(std::move(accessor));
		return std::make_unique<ScriptVarRead>(std::move(accessor));
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

	ScriptArrayRead::ScriptArrayRead(std::unique_ptr<VariableAccessor>&& accessor, std::unique_ptr<ScriptCallable> &&arrayAccessor)
		: ScriptVarRead(std::move(accessor))
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

	ScriptArrayReadPrototype::ScriptArrayReadPrototype(const std::string& varname, std::unique_ptr<ScriptCallable>&& arrayAccessor)
		: varname(varname)
		, arrayAccessor(std::move(arrayAccessor))
	{
	}

	std::unique_ptr<ScriptCallable> ScriptArrayReadPrototype::build(ScriptScopeBase& scope)
	{
		auto result = std::make_unique<ScriptArrayRead>(VariableAccessor::find(scope, this->varname), std::move(this->arrayAccessor));
		return result;
	}
}