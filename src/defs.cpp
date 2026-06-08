#include "gscript/defs.hpp"
#include "gscript/util.hpp"
#include "gscript/runtime/scriptValue.hpp"

namespace gscript
{
	std::shared_ptr<ScriptNullValue> script_null;
	std::shared_ptr<ScriptBoolValue> script_true = std::make_shared<ScriptBoolValue>(true);
	std::shared_ptr<ScriptBoolValue> script_false = std::make_shared<ScriptBoolValue>(false);

	/*ScriptNullValue* SCR_NULL = new ScriptNullValue;
	ScriptBoolValue* SCR_TRUE = new ScriptBoolValue(true);
	ScriptBoolValue* SCR_FALSE = new ScriptBoolValue(false);*/

	/*const char *KW_NULL = "NULL";
	const char *KW_TRUE = "TRUE";
	const char *KW_FALSE = "FALSE";*/

	const std::shared_ptr<ScriptNullValue> getNull()
	{
		return script_null;
	}

	const std::shared_ptr<ScriptBoolValue> getTrue()
	{
		return script_true;
	}

	const std::shared_ptr<ScriptBoolValue> getFalse()
	{
		return script_false;
	}

	const std::map<std::string, OperatorType>& getOperatorMap()
	{
		static const std::map<std::string, OperatorType> typemap
		{
			{ "+", OperatorType::Add },
			{ "+=", OperatorType::AddTo },
			{ "-", OperatorType::Subtract },
			{ "-=", OperatorType::SubtractFrom },
			{ "*", OperatorType::Multiply },
			{ "*=", OperatorType::MultiplyBy },
			{ "/", OperatorType::Divide },
			{ "/=", OperatorType::DivideBy },
			{ "==", OperatorType::Equals },
			{ "!=", OperatorType::NotEquals },
			{ ">", OperatorType::GreaterThan },
			{ ">=", OperatorType::GreaterThanOrEqual },
			{ "<", OperatorType::LesserThan },
			{ "<=", OperatorType::LesserThanOrEqual },
			{ "=", OperatorType::Assign },
			{ "!", OperatorType::Negate },
			{ ".++", OperatorType::PostIncrement },
			{ "++.", OperatorType::PreIncrement },
			{ ".--", OperatorType::PostDecrement },
			{ "--.", OperatorType::PreDecrement },
			{ "()", OperatorType::Call }
		};
		return typemap;
	}

	const std::map<OperatorType, std::string>& getOperatorNameMap()
	{
		static const auto typemap = flip<std::string, OperatorType>(getOperatorMap());
		return typemap;
	}

	const std::string getOperatorFunctionName(OperatorType type)
	{
		return "operator" + getOperatorNameMap().at(type);
	}
}