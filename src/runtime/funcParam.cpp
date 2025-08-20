#include "runtime/funcParam.hpp"
#include "runtime/statement.hpp"
#include "type.hpp"

namespace gscript
{
	FunctionParameter::FunctionParameter(const ScriptType &type, const std::string &name, ScriptValue *val)
		: ScriptVariable(name, new ScriptType(type), val, 0)
	{
	}

	FunctionParameter::FunctionParameter(const ScriptType &type, const std::string &name)
		: ScriptVariable(name, new ScriptType(type), ScriptType::createEmptyValue(type.getTypeDescriptor()), 0)
	{
	}

	FunctionParameter::FunctionParameter(const ScriptType *type, const std::string &name)
		//: ScriptVariable(name, type, ScriptType::createEmptyValue(type->getTypeDescriptor()), 0)
		: ScriptVariable(name, type, ScriptType::createEmptyValue(type->getAbsoluteTypeDescriptor(), type), 0)
	{
	}

	// CONTAINER

	FunctionParameterContainer::FunctionParameterContainer(std::vector<std::unique_ptr<ScriptStatement>>&& params)
		: params(std::move(params))
	{
	}

	PARAMS_T FunctionParameterContainer::getParamTypes() const
	{
		PARAMS_T result;

		for (auto& param : this->params)
		{
			result.emplace_back(param->getType());
		}

		return result;
	}

	std::vector<std::unique_ptr<ScriptStatement>>& FunctionParameterContainer::getParams()
	{
		return this->params;
	}
}