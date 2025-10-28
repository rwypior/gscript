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

	std::unordered_map<std::string, ValueType> ScriptType::typemap({
		{ "bool", ValueType::Bool },
		{ "byte", ValueType::Byte },
		{ "char", ValueType::Char },
		{ "int", ValueType::Int },
		{ "unsigned int", ValueType::UnsignedInt },
		{ "float", ValueType::Float },
		{ "double", ValueType::Double },
		{ "string", ValueType::String },
		{ "class", ValueType::Class },
		{ "array", ValueType::Array },
		{ "reference", ValueType::Reference },
		{ "null", ValueType::Null },
		{ "void", ValueType::Void }
	});

	TypeDescriptor ScriptType::translateType(const std::string &name)
	{
		auto it = typemap.find(name);
		if (it != typemap.end())
			return TypeDescriptor(it->second);

		size_t firstBracket = name.find_last_of(ParserArrayAccessor::keycharArrayAccessorBegin);
		if (firstBracket != std::string::npos)
		{
			std::string subName = name.substr(0, firstBracket);
			auto it2 = typemap.find(subName);
			return TypeDescriptor(typemap.at("array"), translateType(subName));
		}

		size_t firstRef = name.find_last_of(ParserReference::keycharReference);
		if (firstRef != std::string::npos)
		{
			std::string subName = name.substr(0, firstRef);
			auto it2 = typemap.find(subName);
			return TypeDescriptor(typemap.at("reference"), translateType(subName));
		}

		return TypeDescriptor(typemap.at("class"), name);
	}

	std::string ScriptType::translateType(ValueType t)
	{
		auto it = std::find_if(typemap.begin(), typemap.end(), [t](const std::pair<std::string, ValueType>& p) {
			return p.second == t;
		});
		if (it == typemap.end())
			return "";
		return it->first;
	}

	std::unique_ptr<ScriptValue> ScriptType::createEmptyValue(ValueType type, const std::shared_ptr<ScriptType> t)
	{
		switch (type)
		{
		case ValueType::Bool: return std::make_unique<ScriptBoolValue>();
		case ValueType::Byte: return std::make_unique<ScriptByteValue>();
		case ValueType::Char: return std::make_unique<ScriptCharValue>();
		case ValueType::Int: return std::make_unique<ScriptIntValue>();
		case ValueType::UnsignedInt: return std::make_unique<ScriptUnsignedIntValue>();
		case ValueType::Float: return std::make_unique<ScriptFloatValue>();
		case ValueType::Double: return std::make_unique<ScriptDoubleValue>();
		case ValueType::String: return std::make_unique<ScriptStringValue>();
		case ValueType::Class: return std::make_unique<ScriptClassValue>(nullptr, std::static_pointer_cast<ScriptClassType>(t)->getClass());
		case ValueType::Array: return std::make_unique<ScriptArrayValue>();
		case ValueType::Reference: return std::make_unique<ScriptReferenceValue>(std::static_pointer_cast<ScriptReferenceType>(t));
		case ValueType::Null: return ScriptType::null();
		}

		throw new CompileException("Invalid type given");
	}

	std::unique_ptr<ScriptNullValue> ScriptType::null()
	{
		return std::make_unique<ScriptNullValue>();
	}

	std::shared_ptr<ScriptType> ScriptType::nulltype()
	{
		return std::make_shared<ScriptType>(ValueType::Null);
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
		return std::make_shared<ScriptType>(ValueType::Bool);
	}

	std::unique_ptr<ScriptType> ScriptType::createClass(const std::string& classname, ScriptScopeBase& scope)
	{
		if (ScriptClass* c = scope.getGlobalNamespace()->findClass(classname))
			return std::make_unique<ScriptClassType>(*c);

		throw CompileException(std::string("Class \"") + classname + "\" was not found");
	}

	std::unique_ptr<ScriptType> ScriptType::createPod(ValueType valuetype)
	{
		return std::make_unique<ScriptType>(valuetype);
	}

	std::unique_ptr<ScriptType> ScriptType::createVoid()
	{
		return ScriptType::createPod(ValueType::Void);
	}

	std::unique_ptr<ScriptType> ScriptType::createNull()
	{
		return ScriptType::createPod(ValueType::Null);
	}

	std::unique_ptr<ScriptType> ScriptType::create(ValueType valueType, ScriptScopeBase& scope, const std::string& cname)
	{
		return ScriptType::create(TypeDescriptor(valueType), scope, cname);
	}

	std::unique_ptr<ScriptType> ScriptType::create(TypeDescriptor type, ScriptScopeBase& scope, const std::string &cname)
	{
		if (type.type == ValueType::Class)
			return ScriptType::createClass(cname, scope);
		else if (type.type == ValueType::Array)
		{
			assert(type.subType && "Subtype must be specified for array type");
			return std::make_unique<ScriptArrayType>(ScriptType::create(*type.subType, scope));
		}
		else if (type.type == ValueType::Reference)
		{
			assert(type.subType && "Subtype must be specified for reference type");
			return std::make_unique<ScriptReferenceType>(ScriptType::create(*type.subType, scope));
		}

		return ScriptType::createPod(type.type);
	}

	std::unique_ptr<ScriptType> ScriptType::create(const std::string &tname, ScriptScopeBase& scope)
	{
		return ScriptType::create(translateType(tname), scope, tname);
	}

	// GENERAL TYPE

	ScriptType::ScriptType(ValueType type)
		: type(type)
	{
	}

	std::unique_ptr<ScriptType> ScriptType::clone() const
	{
		return std::make_unique<ScriptType>(this->type);
	}

	ValueType ScriptType::getTypeDescriptor() const
	{
		return this->type;
	}

	ValueType ScriptType::getUnderlyingTypeDescriptor() const
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
		if (b.getTypeDescriptor() == ValueType::Reference)
			return static_cast<const ScriptReferenceType&>(b).operator==(*this);

		return this->type == b.type;
	}

	bool ScriptType::operator !=(const ScriptType &b) const
	{
		return !(*this == b);
	}

	// CLASS TYPE

	ScriptClassType::ScriptClassType(ScriptClass &sclass)
		:ScriptType(ValueType::Class),
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
		if (b.getTypeDescriptor() == ValueType::Class)
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
		: ScriptType(ValueType::Array)
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
		if (b.getTypeDescriptor() == ValueType::Array)
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

	ValueType ScriptArrayType::getUnderlyingTypeDescriptor() const
	{
		return this->subType->getTypeDescriptor();
	}

	// REFERENCE TYPE

	ScriptReferenceType::ScriptReferenceType(const std::shared_ptr<ScriptType> subType)
		: ScriptType(ValueType::Reference)
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
		if (b.getTypeDescriptor() == ValueType::Reference)
		{
			const ScriptReferenceType &classB = static_cast<const ScriptReferenceType&>(b);
			return *this->subType == *classB.subType;
		}

		return this->subType->getTypeDescriptor() == b.getTypeDescriptor();
	}

	ValueType ScriptReferenceType::getUnderlyingTypeDescriptor() const
	{
		return this->subType->getTypeDescriptor();
	}
}