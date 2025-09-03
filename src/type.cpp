#include "type.hpp"
#include "defs.hpp"
#include "compileException.hpp"
#include "runtime/scope.hpp"
#include "runtime/namespace.hpp"
#include "runtime/class.hpp"
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

	ScriptValue *ScriptType::createEmptyValue(VALUE_TYPE_T type, const ScriptType *t)
	{
		switch (type)
		{
		case VALUE_TYPE_T::VT_BOOL: return new ScriptBoolValue();
		case VALUE_TYPE_T::VT_BYTE: return new ScriptByteValue();
		case VALUE_TYPE_T::VT_CHAR: return new ScriptCharValue();
		case VALUE_TYPE_T::VT_INT: return new ScriptIntValue();
		case VALUE_TYPE_T::VT_UNSIGNED_INT: return new ScriptUnsignedIntValue();
		case VALUE_TYPE_T::VT_FLOAT: return new ScriptFloatValue();
		case VALUE_TYPE_T::VT_DOUBLE: return new ScriptDoubleValue();
		case VALUE_TYPE_T::VT_STRING: return new ScriptStringValue();
		case VALUE_TYPE_T::VT_CLASS: return new ScriptClassValue(nullptr, static_cast<const ScriptClassType*>(t)->sclass);
		case VALUE_TYPE_T::VT_ARRAY: return new ScriptArrayValue();
		case VALUE_TYPE_T::VT_REFERENCE: return new ScriptReferenceValue(static_cast<const ScriptReferenceType*>(t));
		case VALUE_TYPE_T::VT_NULL: return SCR_NULL;
		}

		throw new CompileException("Invalid type given");
	}

	ScriptType *ScriptType::create(TypeDescriptor type, ScriptScopeBase& scope, const std::string &cname)
	{
		if (type.type == VALUE_TYPE_T::VT_CLASS)
		{
			//if (ScriptClass *c = scope.getClosestNamespace(true)->findClass(cname))
			if (ScriptClass *c = scope.getGlobalNamespace()->findClass(cname))
				return new ScriptClassType(*c);

			throw CompileException(std::string("Class \"") + cname + "\" was not found");
		}
		else if (type.type == VALUE_TYPE_T::VT_ARRAY)
		{
			return new ScriptArrayType(ScriptType::create(type.subType->type, scope));
		}
		else if (type.type == VALUE_TYPE_T::VT_REFERENCE)
		{
			return new ScriptReferenceType(ScriptType::create(type.subType->type, scope));
		}

		return new ScriptType(type.type);
	}

	ScriptType *ScriptType::create(const std::string &tname, ScriptScopeBase& scope)
	{
		return ScriptType::create(translateType(tname), scope, tname);
	}

	// GENERAL TYPE

	ScriptType::ScriptType(VALUE_TYPE_T type)
		:type(type)
	{
	}

	VALUE_TYPE_T ScriptType::getTypeDescriptor() const
	{
		return this->type;
	}

	VALUE_TYPE_T ScriptType::getAbsoluteTypeDescriptor() const
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

	bool ScriptType::operator ==(const ScriptType &b) const
	{
		if (b.getAbsoluteTypeDescriptor() == VALUE_TYPE_T::VT_REFERENCE)
			return static_cast<const ScriptReferenceType&>(b).operator==(*this);

		/*if (this->getTypeDescriptor() == VALUE_TYPE_T::VT_CLASS || b.getTypeDescriptor() == VALUE_TYPE_T::VT_CLASS)
		{

		}*/

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

	bool ScriptClassType::matches(const ScriptVariable &variable)
	{
		if (variable.getValue()->getType()->getTypeDescriptor() != this->type)
			return false;

		const ScriptClassType *b = static_cast<const ScriptClassType*>(variable.getType());

		return this->sclass == b->sclass;
	}

	/*bool ScriptClassType::operator ==(const ScriptClassType &b) const
	{
		const ScriptClassType &classB = static_cast<const ScriptClassType&>(b);
		return this->sclass == classB.sclass;
	}*/

	bool ScriptClassType::operator ==(const ScriptType &b) const
	{
		if (b.getTypeDescriptor() == VALUE_TYPE_T::VT_CLASS)
		{
			const ScriptClassType &classB = static_cast<const ScriptClassType&>(b);
			return this->sclass == classB.sclass;
		}

		return false;
	}

	// ARRAY TYPE

	ScriptArrayType::ScriptArrayType(const ScriptType *subType)
		:ScriptType(VALUE_TYPE_T::VT_ARRAY),
		subType(subType)
	{
	}

	bool ScriptArrayType::matches(const ScriptVariable &variable)
	{
		if (variable.getValue()->getType()->getTypeDescriptor() != this->type)
			return false;

		const ScriptArrayType *b = static_cast<const ScriptArrayType*>(variable.getType());

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

	/*bool ScriptArrayType::operator ==(const ScriptArrayType &b) const
	{
		const ScriptArrayType &classB = static_cast<const ScriptArrayType&>(b);
		return *this->subType == *classB.subType;
	}*/

	// REFERENCE TYPE

	ScriptReferenceType::ScriptReferenceType(const ScriptType *subType)
		:ScriptType(VALUE_TYPE_T::VT_REFERENCE),
		subType(subType)
	{
	}

	bool ScriptReferenceType::matches(const ScriptVariable &variable)
	{
		if (variable.getValue()->getType()->getTypeDescriptor() != this->type)
			return false;

		const ScriptArrayType *b = static_cast<const ScriptArrayType*>(variable.getType());

		return this->subType == b->subType;
	}

	/*bool ScriptReferenceType::operator ==(const ScriptReferenceType &b) const
	{
		return this->subType->getTypeDescriptor() == b.subType->getTypeDescriptor();
	}*/

	bool ScriptReferenceType::operator ==(const ScriptType &b) const
	{
		if (b.getTypeDescriptor() == VALUE_TYPE_T::VT_REFERENCE)
		{
			const ScriptReferenceType &classB = static_cast<const ScriptReferenceType&>(b);
			return *this->subType == *classB.subType;
		}

		return this->subType->getTypeDescriptor() == b.getTypeDescriptor();
	}

	VALUE_TYPE_T ScriptReferenceType::getTypeDescriptor() const
	{
		return this->subType->getTypeDescriptor();
	}
}