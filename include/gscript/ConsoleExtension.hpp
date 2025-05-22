#ifndef _h_gscript_extension_console
#define _h_gscript_extension_console

#include "script.hpp"
#include "scriptExtension.hpp"
#include "function.hpp"
#include "class.hpp"

namespace gscript
{
	class ConsoleExtension : public ScriptExtension
	{
	public:
		class FuncPrint : public ScriptMethod
		{
		public:
			FuncPrint(ScriptScope &scope, const std::string &name)
				:ScriptMethod(
					scope,
					name,
					ScriptType::create(VALUE_TYPE_T::VT_VOID, this->scope),
					PARAMS_T({ FunctionParameter(ScriptType::create(VALUE_TYPE_T::VT_STRING, scope)) }),
					static_cast<BITFLAG_T>(MODIFIER_T::M_STATIC)
				)
			{
			}

			virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override
			{
				this->validateParams(c);

				const ScriptValue &val = *c[0];
				const ScriptStringValue &sval = static_cast<const ScriptStringValue&>(val);

				std::cout << sval.getValue() << std::flush;

				return Script::SCR_NULL;
			}
		};

		class FuncPrintLn : public ScriptMethod
		{
		public:
			FuncPrintLn(ScriptScope &scope, const std::string &name)
				:ScriptMethod(
					scope,
					name,
					ScriptType::create(VALUE_TYPE_T::VT_VOID, this->scope),
					PARAMS_T({ FunctionParameter(ScriptType::create(VALUE_TYPE_T::VT_STRING, scope)) }),
					static_cast<BITFLAG_T>(MODIFIER_T::M_STATIC)
				)
			{
			}

			virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override
			{
				this->validateParams(c);

				const ScriptValue &val = *c[0];
				const ScriptStringValue &sval = static_cast<const ScriptStringValue&>(val);

				std::cout << sval.getValue() << std::endl;

				return Script::SCR_NULL;
			}
		};

		class FuncReadLn : public ScriptMethod
		{
		public:
			FuncReadLn(ScriptScope &scope, const std::string &name)
				:ScriptMethod(
					scope,
					name,
					ScriptType::create(VALUE_TYPE_T::VT_STRING, this->scope),
					PARAMS_T(),
					static_cast<BITFLAG_T>(MODIFIER_T::M_STATIC)
				)
			{
			}

			virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override
			{
				this->validateParams(c);

				std::string result;
				std::cin >> result;

				return new ScriptStringValue(result);
			}
		};

		class NamespaceConsole : public ScriptNamespace
		{
		public:
			NamespaceConsole(ScriptScope &scope, const std::string &name)
				: ScriptNamespace(&scope, name)
			{
				this->registerFunction(new FuncPrint(*this, "print"));
				this->registerFunction(new FuncPrintLn(*this, "println"));
				this->registerFunction(new FuncReadLn(*this, "readln"));
			}
		};

		void run(Script &script)
		{
			script.getMainScope()->registerNamespace(new NamespaceConsole(*script.getMainScope(), "Console"));
		}

		std::string getName() const override
		{
			return "console";
		}
	};
}

#endif