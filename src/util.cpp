#include "util.hpp"
#include "runtime/scriptValue.hpp"
#include "runtime/statement.hpp"
#include "runtime/funcParam.hpp"

namespace gscript
{
	PARAMS_T extractParams(std::vector<std::unique_ptr<ScriptStatement>>& statements)
	{
		PARAMS_T params;
		for (auto& stmt : statements)
		{
			params.push_back(std::make_shared<ScriptType>(stmt->getType()->getTypeDescriptor()));
		}
		return params;
	}
}