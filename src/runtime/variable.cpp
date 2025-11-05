#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/scriptValue.hpp"

#include "gscript/type.hpp"
#include "gscript/compileException.hpp"

namespace gscript
{
	ScriptVariable::ScriptVariable(const std::string &name, const std::shared_ptr<ScriptType> type, std::unique_ptr<ScriptValue>&& value)
		: name(name)
		, value(std::move(value))
		, type(type)
	{
	}

	ScriptVariable::ScriptVariable(const ScriptVariable& b)
		: name(b.name)
		, value(b.value ? b.value->clone() : nullptr)
		, type(new ScriptType(*b.type))
	{
	}

	ScriptVariable::~ScriptVariable() = default;

	void ScriptVariable::init(std::unique_ptr<ScriptValue>&& value)
	{
		if (this->getType()->getTypeDescriptor() == ValueType::Reference)
			this->setValue(value);
		else
			this->setValue(value->getType() ? value->clone() : ScriptType::createEmptyValue(this->type->getTypeDescriptor(), this->type));
	}

	const std::unique_ptr<ScriptValue>& ScriptVariable::getValue() const
	{
		return this->value;
	}

	void ScriptVariable::setValue(std::unique_ptr<ScriptValue>&& value)
	{
		this->value = std::move(value);
	}

	void ScriptVariable::setValue(const std::unique_ptr<ScriptValue>& value)
	{
		this->value = value->clone();
	}

	const std::string& ScriptVariable::getName() const
	{
		return this->name;
	}

	const std::shared_ptr<ScriptType> ScriptVariable::getType() const
	{
		return this->type;
	}
}