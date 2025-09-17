#ifndef _h_gscript_type
#define _h_gscript_type

#include "defs.hpp"
#include "lib.hpp"

#include <string>
#include <unordered_map>
#include <memory>

namespace gscript
{
	class ScriptVariable;
	class ScriptClass;
	class ScriptScope;
	class ScriptScopeBase;

	struct TypeDescriptor
	{
		VALUE_TYPE_T type = VALUE_TYPE_T::VT_VOID;
		std::string name;
		TypeDescriptor *subType = nullptr;

		explicit TypeDescriptor(VALUE_TYPE_T type, const TypeDescriptor &subType, const std::string &name = "")
			: type(type)
			, name(name)
			, subType(new TypeDescriptor(subType))
		{
		}

		TypeDescriptor(VALUE_TYPE_T type, const std::string &name = "")
			: type(type)
			, name(name)
			, subType(NULL)
		{
		}

		TypeDescriptor(const TypeDescriptor &copy)
			:type(copy.type)
			, name(copy.name)
			, subType(copy.subType ? new TypeDescriptor(*copy.subType) : NULL)
		{
		}

		TypeDescriptor() = default;

		~TypeDescriptor()
		{
			if (this->subType)
				delete this->subType;
		}
	};
	
	class ScriptType
	{
	public:
		static std::unordered_map<std::string, VALUE_TYPE_T> typemap;

		static TypeDescriptor translateType(const std::string &name);
		static std::string translateType(VALUE_TYPE_T t);
		static std::unique_ptr<ScriptValue> createEmptyValue(VALUE_TYPE_T type, const std::shared_ptr<ScriptType> t = nullptr);

		// Utility function to create null value
		static std::unique_ptr<ScriptNullValue> null();

		// Utility function to create null type
		static std::shared_ptr<ScriptType> nulltype();

		// Utility function to create boolean values
		static std::unique_ptr<ScriptBoolValue> btrue();
		static std::unique_ptr<ScriptBoolValue> bfalse();

		// Utility function to create boolean type
		static std::shared_ptr<ScriptType> booltype();

		SCRIPT_API explicit ScriptType(VALUE_TYPE_T type);

		SCRIPT_API virtual std::unique_ptr<ScriptType> clone() const;

		SCRIPT_API virtual bool matches(const ScriptVariable &var);

		SCRIPT_API static std::unique_ptr<ScriptType> create(VALUE_TYPE_T valueType, ScriptScopeBase& scope, const std::string &cname = "");
		SCRIPT_API static std::unique_ptr<ScriptType> create(TypeDescriptor type, ScriptScopeBase& scope, const std::string &cname = "");
		SCRIPT_API static std::unique_ptr<ScriptType> create(const std::string &tname, ScriptScopeBase& scope);

		SCRIPT_API bool equals(const ScriptType &b) const;

		SCRIPT_API virtual bool operator ==(const ScriptType &b) const;
		SCRIPT_API virtual bool operator !=(const ScriptType &b) const;

		SCRIPT_API virtual VALUE_TYPE_T getTypeDescriptor() const;
		SCRIPT_API virtual VALUE_TYPE_T getUnderlyingTypeDescriptor() const;

	protected:
		VALUE_TYPE_T type;
	};

	class ScriptClassType : public ScriptType
	{
	public:
		SCRIPT_API ScriptClassType(ScriptClass &sclass);

		SCRIPT_API virtual std::unique_ptr<ScriptType> clone() const override;

		SCRIPT_API virtual bool matches(const ScriptVariable &var) override;

		SCRIPT_API virtual bool operator==(const ScriptType &b) const override;

		SCRIPT_API ScriptClass& getClass() const;

	private:
		ScriptClass &sclass;
	};

	class ScriptArrayType : public ScriptType
	{
	public:
		SCRIPT_API ScriptArrayType(const std::shared_ptr<ScriptType> subType);

		SCRIPT_API virtual std::unique_ptr<ScriptType> clone() const override;

		SCRIPT_API virtual bool matches(const ScriptVariable &var) override;

		SCRIPT_API virtual bool operator==(const ScriptType &b) const override;

		SCRIPT_API const std::shared_ptr<ScriptType>& getSubType() const;
		SCRIPT_API virtual VALUE_TYPE_T getUnderlyingTypeDescriptor() const override;

	protected:
		const std::shared_ptr<ScriptType> subType;
	};

	class ScriptReferenceType : public ScriptType
	{
	public:
		SCRIPT_API ScriptReferenceType(const std::shared_ptr<ScriptType> subType);

		SCRIPT_API virtual std::unique_ptr<ScriptType> clone() const override;

		SCRIPT_API virtual bool matches(const ScriptVariable &var) override;

		SCRIPT_API virtual bool operator ==(const ScriptType &b) const override;

		SCRIPT_API virtual VALUE_TYPE_T getUnderlyingTypeDescriptor() const override;

	protected:
		const std::shared_ptr<ScriptType> subType;
	};
}

#endif