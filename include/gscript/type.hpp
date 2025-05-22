#ifndef _h_gscript_type
#define _h_gscript_type

#include "defs.hpp"

#include "lib.hpp"

#include <string>

namespace gscript
{
	class ScriptVariable;
	class ScriptClass;
	class ScriptScope;

	struct TypeDescriptor
	{
		VALUE_TYPE_T type;
		std::string name;
		TypeDescriptor *subType;

		explicit TypeDescriptor(VALUE_TYPE_T type, const TypeDescriptor &subType, const std::string &name = "")
			:type(type),
			name(name),
			subType(new TypeDescriptor(subType))
		{}

		TypeDescriptor(VALUE_TYPE_T type, const std::string &name = "")
			:type(type),
			name(name),
			subType(NULL)
		{}

		TypeDescriptor(const TypeDescriptor &copy)
			:type(copy.type),
			name(copy.name),
			subType(copy.subType ? new TypeDescriptor(*copy.subType) : NULL)
		{ }

		TypeDescriptor() = default;

		~TypeDescriptor()
		{
			if (this->subType)
				delete this->subType;
		}
	};
	
	class ScriptType
	{
	protected:
		VALUE_TYPE_T type;

	public:
		typedef std::pair<std::string, VALUE_TYPE_T> TYPEMAP_DATA_T;
		typedef std::unordered_map<std::string, VALUE_TYPE_T> TYPEMAP_T;

		static TYPEMAP_DATA_T typemapData[];
		static TYPEMAP_T typemap;

		static TypeDescriptor translateType(const std::string &name);
		static ScriptValue *createEmptyValue(VALUE_TYPE_T type, const ScriptType *t = nullptr);

		SCRIPT_API explicit ScriptType(VALUE_TYPE_T type);

		SCRIPT_API virtual bool matches(const ScriptVariable &var);

		SCRIPT_API static ScriptType *create(TypeDescriptor type, ScriptScope &scope, const std::string &cname = "");
		SCRIPT_API static ScriptType *create(const std::string &tname, ScriptScope &scope);

		SCRIPT_API bool equals(const ScriptType &b) const;

		SCRIPT_API virtual bool operator ==(const ScriptType &b) const;
		SCRIPT_API virtual bool operator !=(const ScriptType &b) const;

		SCRIPT_API virtual VALUE_TYPE_T getTypeDescriptor() const;
		SCRIPT_API virtual VALUE_TYPE_T getAbsoluteTypeDescriptor() const;
	};

	class ScriptClassType : public ScriptType
	{
	public:
		ScriptClass &sclass;

		SCRIPT_API ScriptClassType(ScriptClass &sclass);

		SCRIPT_API virtual bool matches(const ScriptVariable &var) override;

		SCRIPT_API virtual bool operator ==(const ScriptType &b) const override;
	};

	class ScriptArrayType : public ScriptType
	{
	public:
		const ScriptType *subType;

		SCRIPT_API ScriptArrayType(const ScriptType *subType);

		SCRIPT_API virtual bool matches(const ScriptVariable &var) override;

		SCRIPT_API virtual bool operator ==(const ScriptType &b) const override;
	};

	class ScriptReferenceType : public ScriptType
	{
	public:
		const ScriptType *subType;

		SCRIPT_API ScriptReferenceType(const ScriptType *subType);

		SCRIPT_API virtual bool matches(const ScriptVariable &var) override;

		SCRIPT_API virtual bool operator ==(const ScriptType &b) const override;

		SCRIPT_API virtual VALUE_TYPE_T getTypeDescriptor() const override;
	};
}

#endif