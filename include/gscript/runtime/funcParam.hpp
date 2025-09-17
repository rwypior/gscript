#ifndef _h_gscript_funcparam
#define _h_gscript_funcparam

#include "defs.hpp"
#include "variable.hpp"
#include "lib.hpp"

#include <string>
#include <vector>
#include <memory>

namespace gscript
{
	class ScriptScope;
	class ParserCallArglist;
	class ScriptStatement;

	class FunctionParameter : public ScriptVariable
	{
	public:
		SCRIPT_API FunctionParameter(const std::shared_ptr<ScriptType> type, const std::string &name, std::unique_ptr<ScriptValue>&& val);
		SCRIPT_API FunctionParameter(const std::shared_ptr<ScriptType> type, const std::string &name = "");
		SCRIPT_API FunctionParameter(const std::shared_ptr<ScriptType> type, const std::shared_ptr<ScriptType> subtype, const std::string &name = "");
	};

	class FunctionParameterContainer
	{
	public:
		SCRIPT_API FunctionParameterContainer(std::vector<std::unique_ptr<ScriptStatement>>&& params);

		SCRIPT_API PARAMS_T getParamTypes() const;
		SCRIPT_API std::vector<std::unique_ptr<ScriptStatement>>& getParams();

	protected:
		std::vector<std::unique_ptr<ScriptStatement>> params;
	};
}

#endif