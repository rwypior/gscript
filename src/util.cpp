#include "gscript/util.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/statement.hpp"
#include "gscript/runtime/funcParam.hpp"
#include "gscript/runtime/literal.hpp"

#include <sstream>

namespace gscript
{
	PARAMS_T extractParams(std::vector<std::unique_ptr<ScriptStatement>>& statements)
	{
		PARAMS_T params;
		for (auto& stmt : statements)
		{
			if (auto classType = std::dynamic_pointer_cast<ScriptClassType>(stmt->getType()))
			{
				params.push_back(FunctionParameter(
					std::make_shared<ScriptType>(stmt->getType()->getTypeDescriptor()), 
					std::make_shared<ScriptClassType>(classType->getClass()))
				);
			}
			else
			{
				params.push_back(std::make_shared<ScriptType>(stmt->getType()->getTypeDescriptor()));
			}
		}
		return params;
	}

	std::unique_ptr<ScriptStatement> createNullStatement()
	{
		auto stmtvecbody = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
		stmtvecbody.push_back(std::make_unique<gscript::ScriptLiteral>(gscript::ScriptType::null()));
		return std::make_unique<gscript::ScriptStatement>(std::move(stmtvecbody));
	}

	std::string parametersToString(const PARAMS_T& params, const bool withNewLines)
	{
		std::stringstream str;
		str << "(";
		if (withNewLines && !params.empty())
			str << "\n";
		for (unsigned int i = 0; i < params.size(); i++)
		{
			auto& param = params[i];
			if (withNewLines)
				str << "\t";

			str << ScriptType::translateType(param.getType()->getTypeDescriptor());
			if (withNewLines)
				str << "\n";
			else if (i < params.size() - 1)
				str << ", ";
		}
		str << ")";
		return str.str();
	}
}