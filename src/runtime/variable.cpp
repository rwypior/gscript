#include "runtime/variable.hpp"
#include "runtime/scope.hpp"
#include "runtime/scriptValue.hpp"

#include "type.hpp"
#include "compileException.hpp"

namespace gscript
{
	ScriptVariable::ScriptVariable(const std::string &name, const ScriptType *type, ScriptValue *value/*, size_t internalPointer*/)
		: name(name)
		, value(value)
		, type(type)
		//, internalPointer(internalPointer)
	{
	}

	ScriptVariable::ScriptVariable(const ScriptVariable& b)
		: name(b.name)
		, value(b.value->clone())
		, type(new ScriptType(*b.type))
		//, internalPointer(b.internalPointer)
	{
	}

	void ScriptVariable::init(ScriptValue *value)
	{
		if (this->getType()->getAbsoluteTypeDescriptor() == VALUE_TYPE_T::VT_REFERENCE)
			this->setValue(value);
		else
		{
			this->setValue(value->getType() ? value->clone() : ScriptType::createEmptyValue(this->type->getTypeDescriptor(), this->type));
		}
	}
}