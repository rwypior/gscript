#include "extension/IOExtension.hpp"

#include <sys/stat.h>

namespace gscript
{
	int IOExtension::createFileMode(const std::string& mode)
	{
		if (mode == "r")
			return std::ios_base::in;
		else if (mode == "w")
			return std::ios_base::out;

		throw RuntimeException(std::string("Unrecognized file mode \"") + mode + "\"");
	}

	// File constructor

	IOExtension::ConstructorFile::ConstructorFile(ScriptScope& scope, const std::string& name)
		: ScriptMethod(
			scope,
			name,
			ScriptType::create(VALUE_TYPE_T::VT_VOID, this->scope),
			PARAMS_T({
				FunctionParameter(ScriptType::create(VALUE_TYPE_T::VT_STRING, scope)),
				FunctionParameter(ScriptType::create(VALUE_TYPE_T::VT_STRING, scope))
			}),
			MODIFIER_T::M_ACCESS_PUBLIC
		)
	{
	}

	ScriptValue* IOExtension::ConstructorFile::run(const CALLABLE_PARAMS_T& c)
	{
		this->validateParams(c);

		const ScriptStringValue& path = static_cast<const ScriptStringValue&>(*c[0]);
		const ScriptStringValue& modestr = static_cast<const ScriptStringValue&>(*c[1]);

		int mode = IOExtension::createFileMode(modestr.getValue());

		static_cast<ClassFile&>(this->scope).str = std::fstream(path.getValue(), mode);

		return SCR_NULL;
	}

	// Read all

	IOExtension::FuncReadAll::FuncReadAll(ScriptScope& scope, const std::string& name)
		: ScriptMethod(
			scope,
			name,
			ScriptType::create(VALUE_TYPE_T::VT_INT, this->scope),
			PARAMS_T({
				FunctionParameter(ScriptType::create(TypeDescriptor(VALUE_TYPE_T::VT_REFERENCE, VALUE_TYPE_T::VT_STRING), scope))
			}),
			MODIFIER_T::M_ACCESS_PUBLIC
		)
	{
	}

	ScriptValue* IOExtension::FuncReadAll::run(const CALLABLE_PARAMS_T& c)
	{
		this->validateParams(c);

		std::fstream& file = static_cast<ClassFile&>(this->scope).str;

		ScriptReferenceValue storage(c[0]);

		file.seekg(0, std::ios_base::end);
		std::streampos size = file.tellg();
		file.seekg(0, std::ios_base::beg);

		std::string result = std::string(size, '\0');
		file.read(&result[0], size);

		storage.put(new ScriptStringValue(result));

		return new ScriptIntValue(size);
	}

	// File exists

	IOExtension::FuncStaticExists::FuncStaticExists(ScriptScope& scope, const std::string& name)
		: ScriptMethod(
			scope,
			name,
			ScriptType::create(VALUE_TYPE_T::VT_BOOL, this->scope),
			PARAMS_T({
				FunctionParameter(ScriptType::create(TypeDescriptor(VALUE_TYPE_T::VT_STRING), scope))
			}),
			MODIFIER_T::M_ACCESS_PUBLIC | MODIFIER_T::M_STATIC
		)
	{
	}

	ScriptValue* IOExtension::FuncStaticExists::run(const CALLABLE_PARAMS_T& c)
	{
		this->validateParams(c);

		const ScriptStringValue& path = static_cast<const ScriptStringValue&>(*c[0]);

		struct stat buffer;
		bool exists = stat(path.getValue().c_str(), &buffer) == 0;

		return new ScriptBoolValue(exists);
	}

	// Class file

	IOExtension::ClassFile::ClassFile(ScriptScope& scope, const std::string& name)
		: ScriptClass(scope, name)
	{
		this->registerFunction(std::make_unique<ConstructorFile>(*this, "File"));
		this->registerFunction(std::make_unique<FuncReadAll>(*this, "readall"));
		this->registerFunction(std::make_unique<FuncStaticExists>(*this, "exists"));
	}

	IOExtension::ClassFile::~ClassFile()
	{
		if (this->str.is_open())
			this->str.close();
	}

	// IO extension

	void IOExtension::run(Script& script)
	{
		script.getMainScope()->registerClass(std::make_unique<ClassFile>(*script.getMainScope(), "File"));
	}

	std::string IOExtension::getName() const
	{
		return "io";
	}
}