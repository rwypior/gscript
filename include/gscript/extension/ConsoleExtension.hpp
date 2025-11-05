#ifndef _h_gscript_extension_console
#define _h_gscript_extension_console

#include "gscript/script.hpp"
#include "gscript/extension/scriptExtension.hpp"
#include "gscript/runtime/function.hpp"
#include "gscript/runtime/class.hpp"
#include "gscript/runtime/scope.hpp"

#include <string>
#include <ostream>

namespace gscript
{
	class ConsoleExtension : public ScriptExtension
	{
	public:
		class FuncPrint : public ScriptFunction
		{
		public:
			SCRIPT_API FuncPrint(ScriptScopeBase& scope, const std::string& name);
			SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T& c = CALLABLE_PARAMS_T()) override;
		};

		class FuncPrintLn : public ScriptFunction
		{
		public:
			SCRIPT_API FuncPrintLn(ScriptScopeBase& scope, const std::string& name);
			SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T& c = CALLABLE_PARAMS_T()) override;
		};

		class FuncReadLn : public ScriptFunction
		{
		public:
			SCRIPT_API FuncReadLn(ScriptScopeBase& scope, const std::string& name);
			SCRIPT_API virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T& c = CALLABLE_PARAMS_T()) override;
		};

		class NamespaceConsole : public ScriptNamespace
		{
		public:
			SCRIPT_API NamespaceConsole(ScriptScopeBase& scope, const std::string& name);
		};

		SCRIPT_API void run(Script& script);
		SCRIPT_API std::string getName() const override;

		/// Pointer to output stream. By default points to std::cout
		SCRIPT_API static std::ostream* out;
	};
}

#endif