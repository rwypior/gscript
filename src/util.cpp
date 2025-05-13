#include "util.hpp"
#include "scriptValue.hpp"

namespace gscript
{
	make_sarray::make_sarray(ScriptType *subType, size_t count)
		:v(new ScriptValue*[count]),
		type(new ScriptArrayType(subType))
	{ }

	make_sarray::make_sarray(ScriptType *subType, size_t count, char **argv)
		: v(new ScriptValue*[count]),
		type(new ScriptArrayType(subType))
	{
		for (size_t i = 0; i < count; ++i)
		{
			*this << new ScriptStringValue(argv[i]);
		}
	}

	make_sarray &make_sarray::operator<<(ScriptValue* val)
	{
		v[this->i++] = val;
		return *this;
	}

	make_sarray::operator ScriptArrayValue *() const
	{
		return new ScriptArrayValue(this->type, this->v);
	}
}