#include "variable.hpp"
#include "scope.hpp"
#include "type.hpp"
#include "scriptValue.hpp"

#include "compileException.hpp"

namespace gscript
{
	ScriptVariable::ScriptVariable(const std::string &name, const ScriptType *type, ScriptValue *value, size_t internalPointer)
		:name(name),
		value(value),
		type(type),
		internalPointer(internalPointer)
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