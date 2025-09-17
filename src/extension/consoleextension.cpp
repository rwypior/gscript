#include "extension/ConsoleExtension.hpp"

#include <iostream>

namespace gscript
{
	// Print

	ConsoleExtension::FuncPrint::FuncPrint(ScriptScopeBase& scope, const std::string& name)
		: ScriptMethod(
			scope,
			name,
			ScriptType::create(VALUE_TYPE_T::VT_VOID, this->scope),
			PARAMS_T({ FunctionParameter(ScriptType::create(VALUE_TYPE_T::VT_STRING, scope)) }),
			MODIFIER_T::M_STATIC
		)
	{
	}

	std::unique_ptr<ScriptValue> ConsoleExtension::FuncPrint::run(const CALLABLE_PARAMS_T& c)
	{
		this->validateParams(c);

		const ScriptValue& val = *c[0];
		const ScriptStringValue& sval = static_cast<const ScriptStringValue&>(val);

		std::cout << sval.getValue() << std::flush;

		return ScriptType::null();
	}

	// Println

	ConsoleExtension::FuncPrintLn::FuncPrintLn(ScriptScopeBase& scope, const std::string& name)
		:ScriptMethod(
			scope,
			name,
			ScriptType::create(VALUE_TYPE_T::VT_VOID, this->scope),
			PARAMS_T({ FunctionParameter(ScriptType::create(VALUE_TYPE_T::VT_STRING, scope)) }),
			MODIFIER_T::M_STATIC
		)
	{
	}

	std::unique_ptr<ScriptValue> ConsoleExtension::FuncPrintLn::run(const CALLABLE_PARAMS_T& c)
	{
		this->validateParams(c);

		const ScriptValue& val = *c[0];
		const ScriptStringValue& sval = static_cast<const ScriptStringValue&>(val);

		std::cout << sval.getValue() << std::endl;

		return ScriptType::null();
	}

	// Readln

	ConsoleExtension::FuncReadLn::FuncReadLn(ScriptScopeBase& scope, const std::string& name)
		: ScriptMethod(
			scope,
			name,
			ScriptType::create(VALUE_TYPE_T::VT_STRING, this->scope),
			PARAMS_T(),
			MODIFIER_T::M_STATIC
		)
	{
	}

	std::unique_ptr<ScriptValue> ConsoleExtension::FuncReadLn::run(const CALLABLE_PARAMS_T& c)
	{
		this->validateParams(c);

		std::string result;
		std::cin >> result;

		return std::make_unique<ScriptStringValue>(result);
	}

	// Console extension

	ConsoleExtension::NamespaceConsole::NamespaceConsole(ScriptScopeBase& scope, const std::string& name)
		: ScriptNamespace(&scope, name)
	{
		this->registerFunction(std::make_unique<FuncPrint>(*this, "print"));
		this->registerFunction(std::make_unique<FuncPrintLn>(*this, "println"));
		this->registerFunction(std::make_unique<FuncReadLn>(*this, "readln"));
	}

	void ConsoleExtension::run(Script& script)
	{
		script.getMainScope()->registerNamespace(std::make_unique<NamespaceConsole>(*script.getMainScope(), "Console"));
	}

	std::string ConsoleExtension::getName() const
	{
		return "console";
	}
}