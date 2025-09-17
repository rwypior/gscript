#include "runtime/funcParam.hpp"
#include "runtime/statement.hpp"
#include "type.hpp"

namespace gscript
{
	FunctionParameter::FunctionParameter(const std::shared_ptr<ScriptType> type, const std::string &name, std::unique_ptr<ScriptValue>&& val)
		: ScriptVariable(name, type, std::move(val))
	{
	}

	FunctionParameter::FunctionParameter(const std::shared_ptr<ScriptType> type, const std::string &name)
		: ScriptVariable(name, type, ScriptType::createEmptyValue(type->getTypeDescriptor()))
	{
	}

	FunctionParameter::FunctionParameter(const std::shared_ptr<ScriptType> type, const std::shared_ptr<ScriptType> subtype, const std::string &name)
		: ScriptVariable(name, type, ScriptType::createEmptyValue(type->getTypeDescriptor(), subtype))
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