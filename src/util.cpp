#include "util.hpp"
#include "runtime/scriptValue.hpp"
#include "runtime/statement.hpp"
#include "runtime/funcParam.hpp"

namespace gscript
{
	make_sarray::make_sarray(ScriptType *subType, size_t count)
		: type(new ScriptArrayType(subType))
	{
		this->v.reserve(count);
	}

	make_sarray::make_sarray(ScriptType *subType, size_t count, char **argv)
		: type(new ScriptArrayType(subType))
	{
		this->v.reserve(count);
		for (size_t i = 0; i < count; ++i)
		{
			*this << new ScriptStringValue(argv[i]);
		}
	}

	make_sarray &make_sarray::operator<<(ScriptValue* val)
	{
		this->v.push_back(val);
		return *this;
	}

	make_sarray::operator ScriptArrayValue *() const
	{
		return new ScriptArrayValue(this->type, this->v);
	}

	PARAMS_T extractParams(std::vector<std::unique_ptr<ScriptStatement>>& statements)
	{
		PARAMS_T params;
		for (auto& stmt : statements)
		{
			params.push_back(ScriptType(stmt->getType()->getTypeDescriptor()));
		}
		return params;
	}
	//PARAMS_T
}