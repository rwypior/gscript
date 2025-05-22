#ifndef _h_gscript_extension_io
#define _h_gscript_extension_io

#include <sys/stat.h>

#include "script.hpp"
#include "defs.hpp"
#include "scriptExtension.hpp"
#include "function.hpp"
#include "class.hpp"

namespace gscript
{
	class IOExtension : public ScriptExtension
	{
	public:
		static int createFileMode(const std::string &mode)
		{
			if (mode == "r")
				return std::ios_base::in;
			else if (mode == "w")
				return std::ios_base::out;

			throw RuntimeException(std::string("Unrecognized file mode \"") + mode + "\"");
		}

		class ConstructorFile : public ScriptMethod, public ScriptExtension::ExtensionFunction
		{
		public:
			ConstructorFile(ScriptScope &scope, const std::string &name)
				:ScriptMethod(
					scope,
					name,
					ScriptType::create(VALUE_TYPE_T::VT_VOID, this->scope),
					PARAMS_T({
						FunctionParameter(ScriptType::create(VALUE_TYPE_T::VT_STRING, scope)),
						FunctionParameter(ScriptType::create(VALUE_TYPE_T::VT_STRING, scope))
						}),
					static_cast<BITFLAG_T>(MODIFIER_T::M_ACCESS_PUBLIC)
				)
			{
			}

			virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override
			{
				this->validateParams(c);

				const ScriptStringValue &path = static_cast<const ScriptStringValue&>(*c[0]);
				const ScriptStringValue &modestr = static_cast<const ScriptStringValue&>(*c[1]);

				int mode = IOExtension::createFileMode(modestr.getValue());

				static_cast<ClassFile&>(this->scope).str = std::fstream(path.getValue(), mode);

				return Script::SCR_NULL;
			}
		};

		class FuncReadAll : public ScriptMethod, public ScriptExtension::ExtensionFunction
		{
		public:
			FuncReadAll(ScriptScope &scope, const std::string &name)
				:ScriptMethod(
					scope,
					name,
					ScriptType::create(VALUE_TYPE_T::VT_INT, this->scope),
					PARAMS_T({
						FunctionParameter(ScriptType::create(TypeDescriptor(VALUE_TYPE_T::VT_REFERENCE, VALUE_TYPE_T::VT_STRING), scope))
						}),
					static_cast<BITFLAG_T>(MODIFIER_T::M_ACCESS_PUBLIC)
				)
			{
			}

			virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override
			{
				this->validateParams(c);
				
				std::fstream &file = static_cast<ClassFile&>(this->scope).str;

				ScriptReferenceValue storage(c[0]);

				file.seekg(0, std::ios_base::end);
				std::streampos size = file.tellg();
				file.seekg(0, std::ios_base::beg);

				std::string result = std::string(size, '\0');
				file.read(&result[0], size);

				storage.put(new ScriptStringValue(result));

				return new ScriptIntValue(size);
			}
		};

		class FuncStaticExists : public ScriptMethod, public ScriptExtension::ExtensionFunction
		{
		public:
			FuncStaticExists(ScriptScope &scope, const std::string &name)
				:ScriptMethod(
					scope,
					name,
					ScriptType::create(VALUE_TYPE_T::VT_BOOL, this->scope),
					PARAMS_T({
						FunctionParameter(ScriptType::create(TypeDescriptor(VALUE_TYPE_T::VT_STRING), scope))
						}),
					static_cast<BITFLAG_T>(MODIFIER_T::M_ACCESS_PUBLIC) | static_cast<BITFLAG_T>(MODIFIER_T::M_STATIC)
				)
			{
			}

			virtual ScriptValue *run(const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override
			{
				this->validateParams(c);

				const ScriptStringValue &path = static_cast<const ScriptStringValue&>(*c[0]);

				struct stat buffer;
				bool exists = stat(path.getValue().c_str(), &buffer) == 0;

				return new ScriptBoolValue(exists);
			}
		};

		class ClassFile : public ScriptClass
		{
			friend class ConstructorFile;
			friend class FuncReadAll;

		protected:
			std::fstream str;

		public:
			ClassFile(ScriptScope &scope, const std::string &name)
				: ScriptClass(scope, name)
			{
				this->registerFunction(new ConstructorFile(*this, "File"));
				this->registerFunction(new FuncReadAll(*this, "readall"));
				this->registerFunction(new FuncStaticExists(*this, "exists"));
			}

			~ClassFile()
			{
				if (this->str.is_open())
					this->str.close();
			}
		};

		void run(Script &script)
		{
			script.getMainScope()->registerClass(new ClassFile(*script.getMainScope(), "File"));
		}

		std::string getName() const override
		{
			return "io";
		}
	};
}

#endif