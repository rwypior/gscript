#ifndef _h_gscript_extension_io
#define _h_gscript_extension_io

#include "script.hpp"
#include "defs.hpp"
#include "scriptExtension.hpp"
#include "runtime/function.hpp"
#include "runtime/class.hpp"
#include "runtimeException.hpp"

#include <fstream>

namespace gscript
{
	class IOExtension : public ScriptExtension
	{
	public:
		static int createFileMode(const std::string& mode);

		class ConstructorFile : public ScriptMethod, public ScriptExtension::ExtensionFunction
		{
		public:
			ConstructorFile(ScriptScopeBase& scope, const std::string& name);
			virtual std::unique_ptr<ScriptValue> run(const CALLABLE_PARAMS_T& c = CALLABLE_PARAMS_T()) override;
		};

		class FuncReadAll : public ScriptMethod, public ScriptExtension::ExtensionFunction
		{
		public:
			FuncReadAll(ScriptScopeBase& scope, const std::string& name);
			virtual std::unique_ptr<ScriptValue> run(const CALLABLE_PARAMS_T& c = CALLABLE_PARAMS_T()) override;
		};

		class FuncStaticExists : public ScriptMethod, public ScriptExtension::ExtensionFunction
		{
		public:
			FuncStaticExists(ScriptScopeBase& scope, const std::string& name);
			virtual std::unique_ptr<ScriptValue> run(const CALLABLE_PARAMS_T& c = CALLABLE_PARAMS_T()) override;
		};

		class ClassFile : public ScriptClass
		{
			friend class ConstructorFile;
			friend class FuncReadAll;

		public:
			ClassFile(ScriptScopeBase& scope, const std::string& name);
			~ClassFile();

		protected:
			std::fstream str;
		};

		void run(Script& script);
		std::string getName() const override;
	};
}

#endif