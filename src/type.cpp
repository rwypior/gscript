#include "type.hpp"
#include "defs.hpp"
#include "compileException.hpp"
#include "runtime/scope.hpp"
#include "runtime/namespace.hpp"
#include "runtime/class.hpp"
#include "runtime/classInstance.hpp"
#include "runtime/variable.hpp"
#include "parser/pReference.hpp"
#include "parser/pArrayAccessor.hpp"

#include <cassert>

namespace gscript
{
	// TYPE FACTORY

	std::unordered_map<std::string, VALUE_TYPE_T> ScriptType::typemap({
		{ "bool", VALUE_TYPE_T::VT_BOOL },
		{ "byte", VALUE_TYPE_T::VT_BYTE },
		{ "char", VALUE_TYPE_T::VT_CHAR },
		{ "int", VALUE_TYPE_T::VT_INT },
		{ "unsigned int", VALUE_TYPE_T::VT_UNSIGNED_INT },
		{ "float", VALUE_TYPE_T::VT_FLOAT },
		{ "double", VALUE_TYPE_T::VT_DOUBLE },
		{ "string", VALUE_TYPE_T::VT_STRING },
		{ "class", VALUE_TYPE_T::VT_CLASS },
		{ "array", VALUE_TYPE_T::VT_ARRAY },
		{ "reference", VALUE_TYPE_T::VT_REFERENCE },
		{ "null", VALUE_TYPE_T::VT_NULL },
		{ "void", VALUE_TYPE_T::VT_NULL }
	});

	TypeDescriptor ScriptType::translateType(const std::string &name)
	{
		auto it = typemap.find(name);
		if (it != typemap.end())
			return TypeDescriptor(it->second);

		size_t firstBracket = name.find_last_of(ParserArrayAccessor::KW_ARRAY_ACCESSOR_BEGIN);
		if (firstBracket != std::string::npos)
		{
			std::string subName = name.substr(0, firstBracket);
			auto it2 = typemap.find(subName);
			return TypeDescriptor(typemap.at("array"), translateType(subName));
		}

		size_t firstRef = name.find_last_of(ParserReference::KW_REFERENCE);
		if (firstRef != std::string::npos)
		{
			std::string subName = name.substr(0, firstRef);
			auto it2 = typemap.find(subName);
			return TypeDescriptor(typemap.at("reference"), translateType(subName));
		}

		return TypeDescriptor(typemap.at("class"), name);
	}

	std::string ScriptType::translateType(VALUE_TYPE_T t)
	{
		auto it = std::find_if(typemap.begin(), typemap.end(), [t](const std::pair<std::string, VALUE_TYPE_T>& p) {
			return p.second == t;
		});
		if (it == typemap.end())
			return "";
		return it->first;
	}

	std::unique_ptr<ScriptValue> ScriptType::createEmptyValue(VALUE_TYPE_T type, const std::shared_ptr<ScriptType> t)
	{
		switch (type)
		{
		case VALUE_TYPE_T::VT_BOOL: return std::make_unique<ScriptBoolValue>();
		case VALUE_TYPE_T::VT_BYTE: return std::make_unique<ScriptByteValue>();
		case VALUE_TYPE_T::VT_CHAR: return std::make_unique<ScriptCharValue>();
		case VALUE_TYPE_T::VT_INT: return std::make_unique<ScriptIntValue>();
		case VALUE_TYPE_T::VT_UNSIGNED_INT: return std::make_unique<ScriptUnsignedIntValue>();
		case VALUE_TYPE_T::VT_FLOAT: return std::make_unique<ScriptFloatValue>();
		case VALUE_TYPE_T::VT_DOUBLE: return std::make_unique<ScriptDoubleValue>();
		case VALUE_TYPE_T::VT_STRING: return std::make_unique<ScriptStringValue>();
		case VALUE_TYPE_T::VT_CLASS: return std::make_unique<ScriptClassValue>(nullptr, std::static_pointer_cast<ScriptClassType>(t)->getClass());
		case VALUE_TYPE_T::VT_ARRAY: return std::make_unique<ScriptArrayValue>();
		case VALUE_TYPE_T::VT_REFERENCE: return std::make_unique<ScriptReferenceValue>(std::static_pointer_cast<ScriptReferenceType>(t));
		case VALUE_TYPE_T::VT_NULL: return ScriptType::null();
		}

		throw new CompileException("Invalid type given");
	}

	std::unique_ptr<ScriptNullValue> ScriptType::null()
	{
		return std::make_unique<ScriptNullValue>();
	}

	std::shared_ptr<ScriptType> ScriptType::nulltype()
	{
		return std::make_shared<ScriptType>(VALUE_TYPE_T::VT_NULL);
	}

	std::unique_ptr<ScriptBoolValue> ScriptType::btrue()
	{
		return std::make_unique<ScriptBoolValue>(true);
	}

	std::unique_ptr<ScriptBoolValue> ScriptType::bfalse()
	{
		return std::make_unique<ScriptBoolValue>(false);
	}

	std::shared_ptr<ScriptType> ScriptType::booltype()
	{
		return std::make_shared<ScriptType>(VALUE_TYPE_T::VT_BOOL);
	}

	std::unique_ptr<ScriptType> ScriptType::create(VALUE_TYPE_T valueType, ScriptScopeBase& scope, const std::string& cname)
	{
		return ScriptType::create(TypeDescriptor(valueType), scope, cname);
	}

	std::unique_ptr<ScriptType> ScriptType::create(TypeDescriptor type, ScriptScopeBase& scope, const std::string &cname)
	{
		if (type.type == VALUE_TYPE_T::VT_CLASS)
		{
			if (ScriptClass *c = scope.getGlobalNamespace()->findClass(cname))
				return std::make_unique<ScriptClassType>(*c);

			throw CompileException(std::string("Class \"") + cname + "\" was not found");
		}
		else if (type.type == VALUE_TYPE_T::VT_ARRAY)
		{
			return std::make_unique<ScriptArrayType>(ScriptType::create(type.subType->type, scope));
		}
		else if (type.type == VALUE_TYPE_T::VT_REFERENCE)
		{
			return std::make_unique<ScriptReferenceType>(ScriptType::create(type.subType->type, scope));
		}

		return std::make_unique<ScriptType>(type.type);
	}

	std::unique_ptr<ScriptType> ScriptType::create(const std::string &tname, ScriptScopeBase& scope)
	{
		return ScriptType::create(translateType(tname), scope, tname);
	}

	// GENERAL TYPE

	ScriptType::ScriptType(VALUE_TYPE_T type)
		:type(type)
	{
	}

	std::unique_ptr<ScriptType> ScriptType::clone() const
	{
		return std::make_unique<ScriptType>(this->type);
	}

	VALUE_TYPE_T ScriptType::getTypeDescriptor() const
	{
		return this->type;
	}

	VALUE_TYPE_T ScriptType::getUnderlyingTypeDescriptor() const
	{
		return this->type;
	}

	bool ScriptType::matches(const ScriptVariable &variable)
	{
		return variable.getValue()->getType()->type == this->type;
	}

	bool ScriptType::equals(const ScriptType &b) const
	{
		return *this == b;
	}

	bool ScriptType::operator==(const ScriptType &b) const
	{
		if (b.getTypeDescriptor() == VALUE_TYPE_T::VT_REFERENCE)
			return static_cast<const ScriptReferenceType&>(b).operator==(*this);

		return this->type == b.type;
	}

	bool ScriptType::operator !=(const ScriptType &b) const
	{
		return !(*this == b);
	}

	// CLASS TYPE

	ScriptClassType::ScriptClassType(ScriptClass &sclass)
		:ScriptType(VALUE_TYPE_T::VT_CLASS),
		sclass(sclass)
	{
	}

	std::unique_ptr<ScriptType> ScriptClassType::clone() const
	{
		return std::make_unique<ScriptClassType>(this->sclass);
	}

	bool ScriptClassType::matches(const ScriptVariable &variable)
	{
		if (variable.getValue()->getType()->getTypeDescriptor() != this->type)
			return false;

		const auto b = std::static_pointer_cast<const ScriptClassType>(variable.getType());

		return this->sclass == b->sclass;
	}

	bool ScriptClassType::operator ==(const ScriptType &b) const
	{
		if (b.getTypeDescriptor() == VALUE_TYPE_T::VT_CLASS)
		{
			const ScriptClassType &classB = static_cast<const ScriptClassType&>(b);
			return this->sclass == classB.sclass;
		}

		return false;
	}

	ScriptClass& ScriptClassType::getClass() const
	{
		return this->sclass;
	}

	// ARRAY TYPE

	ScriptArrayType::ScriptArrayType(const std::shared_ptr<ScriptType> subType)
		: ScriptType(VALUE_TYPE_T::VT_ARRAY)
		, subType(subType)
	{
	}

	std::unique_ptr<ScriptType> ScriptArrayType::clone() const
	{
		return std::make_unique<ScriptArrayType>(this->subType->clone());
	}

	bool ScriptArrayType::matches(const ScriptVariable &variable)
	{
		if (variable.getValue()->getType()->getTypeDescriptor() != this->type)
			return false;

		const auto b = std::static_pointer_cast<const ScriptArrayType>(variable.getType());
		
		return this->subType == b->subType;
	}

	bool ScriptArrayType::operator ==(const ScriptType &b) const
	{
		if (b.getTypeDescriptor() == VALUE_TYPE_T::VT_ARRAY)
		{
			const ScriptArrayType &classB = static_cast<const ScriptArrayType&>(b);
			return *this->subType == *classB.subType;
		}

		return false;
	}

	const std::shared_ptr<ScriptType>& ScriptArrayType::getSubType() const
	{
		return this->subType;
	}

	VALUE_TYPE_T ScriptArrayType::getUnderlyingTypeDescriptor() const
	{
		return this->subType->getTypeDescriptor();
	}

	// REFERENCE TYPE

	ScriptReferenceType::ScriptReferenceType(const std::shared_ptr<ScriptType> subType)
		: ScriptType(VALUE_TYPE_T::VT_REFERENCE)
		, subType(subType)
	{
	}

	std::unique_ptr<ScriptType> ScriptReferenceType::clone() const
	{
		return std::make_unique<ScriptReferenceType>(this->subType->clone());
	}

	bool ScriptReferenceType::matches(const ScriptVariable &variable)
	{
		if (variable.getValue()->getType()->getTypeDescriptor() != this->type)
			return false;

		const auto b = std::static_pointer_cast<const ScriptArrayType>(variable.getType());

		return *this->subType == *b->getSubType();
	}

	bool ScriptReferenceType::operator ==(const ScriptType &b) const
	{
		if (b.getTypeDescriptor() == VALUE_TYPE_T::VT_REFERENCE)
		{
			const ScriptReferenceType &classB = static_cast<const ScriptReferenceType&>(b);
			return *this->subType == *classB.subType;
		}

		return this->subType->getTypeDescriptor() == b.getTypeDescriptor();
	}

	VALUE_TYPE_T ScriptReferenceType::getUnderlyingTypeDescriptor() const
	{
		return this->subType->getTypeDescriptor();
	}
}