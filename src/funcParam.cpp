#include "funcParam.hpp"
#include "type.hpp"
#include "pCallArglist.hpp"
#include "statement.hpp"

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

	FunctionParameterContainer::STATEMENTS_CONTAINER arglistToParamContainer(ScriptScope &scope, const ParserCallArglist &arglist)
	{
		FunctionParameterContainer::STATEMENTS_CONTAINER params;
		params.reserve(arglist.parameters.size());
		for (
			std::vector<ParserStatement*>::const_iterator itp = arglist.parameters.begin();
			itp != arglist.parameters.end();
			++itp
			)
		{
			params.emplace_back(scope, **itp);
		}

		return params;
	}

	FunctionParameterContainer::FunctionParameterContainer(ScriptScope &scope, const ParserCallArglist &arglist)
		:params(arglistToParamContainer(scope, arglist))
	{

	}

	FunctionParameterContainer::FunctionParameterContainer(const STATEMENTS_CONTAINER &params)
		: params(params)
	{

	}

	PARAMS_T FunctionParameterContainer::getParamTypes()
	{
		PARAMS_T result;

		for (STATEMENTS_CONTAINER::const_iterator it = this->params.begin(); it != this->params.end(); ++it)
		{
			//auto x = it->getType();
			//result.emplace_back(*it->getType());
			result.emplace_back(it->getType());
		}

		return result;
	}

	const FunctionParameterContainer::STATEMENTS_CONTAINER &FunctionParameterContainer::getParams()
	{
		return this->params;
	}
}