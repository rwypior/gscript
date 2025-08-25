#ifndef _h_gscript_extension_console
#define _h_gscript_extension_console

#include "script.hpp"
#include "scriptExtension.hpp"
#include "runtime/function.hpp"
#include "runtime/method.hpp"
#include "runtime/class.hpp"
#include "runtime/scope.hpp"

#include <string>

namespace gscript
{
	class ConsoleExtension : public ScriptExtension
	{
	public:
		class FuncPrint : public ScriptMethod
		{
		public:
			FuncPrint(ScriptScope& scope, const std::string& name);
			virtual ScriptValue* run(const CALLABLE_PARAMS_T& c = CALLABLE_PARAMS_T()) override;
		};

		class FuncPrintLn : public ScriptMethod
		{
		public:
			FuncPrintLn(ScriptScope& scope, const std::string& name);
			virtual ScriptValue* run(const CALLABLE_PARAMS_T& c = CALLABLE_PARAMS_T()) override;
		};

		class FuncReadLn : public ScriptMethod
		{
		public:
			FuncReadLn(ScriptScope& scope, const std::string& name);
			virtual ScriptValue* run(const CALLABLE_PARAMS_T& c = CALLABLE_PARAMS_T()) override;
		};

		class NamespaceConsole : public ScriptNamespace
		{
		public:
			NamespaceConsole(ScriptScope& scope, const std::string& name);
		};

		void run(Script& script);
		std::string getName() const override;
	};
}

#endif