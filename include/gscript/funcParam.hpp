#ifndef _h_gscript_funcparam
#define _h_gscript_funcparam

#include "defs.hpp"
#include "variable.hpp"

#include "lib.hpp"

#include <string>
#include <memory>

namespace gscript
{
	class ScriptScope;
	class ParserCallArglist;
	class ScriptStatement;

	class FunctionParameter : public ScriptVariable
	{
	public:
		SCRIPT_API FunctionParameter(const ScriptType &type, const std::string &name, ScriptValue *val);
		SCRIPT_API FunctionParameter(const ScriptType &type, const std::string &name = "");
		SCRIPT_API FunctionParameter(const ScriptType *type, const std::string &name = "");
	};

	class FunctionParameterContainer
	{
	public:
		typedef std::vector<ScriptStatement> STATEMENTS_CONTAINER;

	public:
		SCRIPT_API FunctionParameterContainer(ScriptScope &scope, const ParserCallArglist &arglist);
		SCRIPT_API FunctionParameterContainer(const STATEMENTS_CONTAINER &params);

		SCRIPT_API PARAMS_T getParamTypes();
		SCRIPT_API const STATEMENTS_CONTAINER &getParams();

	protected:
		const STATEMENTS_CONTAINER params;
	};
}

#endif