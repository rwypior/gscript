#ifndef _h_gscript_value
#define _h_gscript_value

#include "defs.hpp"
#include "type.hpp"

#include "lib.hpp"

#include <string>

#include <iostream>

namespace gscript
{
	class ScriptValue;
	class ScriptIntValue;
	class ScriptUnsignedIntValue;
	class ScriptBoolValue;
	class ScriptCharValue;
	class ScriptByteValue;
	class ScriptFloatValue;
	class ScriptDoubleValue;
	class ScriptStringValue;
	class ScriptClassValue;
	class ScriptReferenceValue;
	class ScriptNullValue;

	class ScriptClassInstance;

	// TYPE
#define SV_VAL_INIT(v) : val(v)
#define SV_TYPE_INIT(val_type) ,type(new ScriptType(val_type)) 
#define SV_TYPE_INIT_CLS(c) ,type(new ScriptClassType(*c)) 
#define SV_TYPE_INIT_ARR(c) ,type(new ScriptArrayType(*c)) 
#define SV_GETTYPE_M() virtual const ScriptType *getType() override { return this->type; }
#define SV_TYPE_M() const ScriptType *type;
// END TYPE

// CONSTRUCTOR
#define SV_CTOR_M(cls, type) cls(type v = type())
#define SV_CTOR_NULL_M(cls, type) cls(type v = NULL)
#define SV_CONVERSION_CTOR_M(cls, type) cls(const type &v)
//#define SV_CTOR_M(cls, type) cls(type v = type()) : val(v)
//#define SV_CTOR_NULL_M(cls, type) cls(type v = NULL) : val(v)
//#define SV_CONVERSION_CTOR_M(cls, type) cls(const type &v) : val(v.val)
#define SV_CTOR_END_M() {}
// END CONSTRUCTOR

// UTILS
#define SV_GETSIZE_M(type) int getSize() override { return sizeof(type); }
#define SV_CLONE_M(type) virtual ScriptValue *clone() override { return new type(*this); }
#define SV_BOOLEAN() virtual ScriptBoolValue boolean() override { return ScriptBoolValue(this->val); }
#define SV_ASSIGN(type) virtual void assign(const ScriptValue &val) override { this->val = static_cast<const type&>(val).val; }
// END UTILS

// VALUE
#define SV_GETVAL(M_TYPE, M_RETURNTYPE) virtual M_RETURNTYPE getValue() const { return this->val; }
//#define SV_GETVAL(type, returntype) virtual returntype getValue() const { return this->type->getAbsoluteTypeDescriptor() == VALUE_TYPE_T::VT_REFERENCE ? static_cast<ScriptReferenceValue*>(this)->resolve<type>() : this->val; }
//#define SV_GETVAL(M_TYPE, returntype) virtual returntype getValue() const {\
//	return this->type->getAbsoluteTypeDescriptor() == VALUE_TYPE_T::VT_REFERENCE ? (reinterpret_cast<const ScriptReferenceValue*>(this))->resolve<M_TYPE>().getValue() : this->val;\
//}
#define SV_SETVAL(type) virtual void setValue(type val) { this->val = val; }
// END VALUE

	class ScriptClass;
	class ScriptBoolValue;

	class ScriptValue
	{
	public:
		virtual void assign(const ScriptValue &val) = 0;
		virtual int getSize() = 0;
		virtual const ScriptType *getType() = 0;
		virtual ScriptValue *clone() = 0;

		virtual ScriptBoolValue boolean() = 0;

		template<typename T>
		T &as()
		{
			return static_cast<T&>(*this);
		}

		SCRIPT_API ~ScriptValue();
	};

	class ScriptBoolValue : public ScriptValue
	{
	private:
		bool val;
		SV_TYPE_M();

	public:
		SV_CTOR_M(ScriptBoolValue, bool) SV_VAL_INIT(v) SV_TYPE_INIT(VALUE_TYPE_T::VT_BOOL) SV_CTOR_END_M();
		SV_GETSIZE_M(bool);
		SV_GETTYPE_M();
		SV_CLONE_M(ScriptBoolValue);
		SV_BOOLEAN();
		SV_GETVAL(ScriptBoolValue, bool);
		SV_SETVAL(bool);
		SV_ASSIGN(ScriptBoolValue);

		/*virtual bool getValue() const
		{
			return this->type->getAbsoluteTypeDescriptor() == VALUE_TYPE_T::VT_REFERENCE ? (reinterpret_cast<const ScriptReferenceValue*>(this))->resolve<ScriptBoolValue>().getValue() : this->val;
		}*/
	};

	class ScriptCharValue : public ScriptValue
	{
	protected:
		char val;
		SV_TYPE_M();

	public:
		SV_CTOR_M(ScriptCharValue, char) SV_VAL_INIT(v) SV_TYPE_INIT(VALUE_TYPE_T::VT_CHAR) SV_CTOR_END_M();
		SV_GETSIZE_M(char);
		SV_GETTYPE_M();
		SV_CLONE_M(ScriptCharValue);
		SV_BOOLEAN();
		SV_GETVAL(ScriptCharValue, char);
		SV_SETVAL(char);
		SV_ASSIGN(ScriptCharValue);
	};

	class ScriptByteValue : public ScriptValue
	{
	protected:
		unsigned char val;
		SV_TYPE_M();

	public:
		SV_CTOR_M(ScriptByteValue, unsigned char) SV_VAL_INIT(v) SV_TYPE_INIT(VALUE_TYPE_T::VT_BYTE) SV_CTOR_END_M();
		SV_GETSIZE_M(unsigned char);
		SV_GETTYPE_M();
		SV_CLONE_M(ScriptByteValue);
		SV_BOOLEAN();
		SV_GETVAL(ScriptByteValue, unsigned char);
		SV_SETVAL(unsigned char);
		SV_ASSIGN(ScriptByteValue);
	};

	class ScriptIntValue : public ScriptValue
	{
	protected:
		int val;
		SV_TYPE_M();

	public:
		SV_CTOR_M(ScriptIntValue, int) SV_VAL_INIT(v) SV_TYPE_INIT(VALUE_TYPE_T::VT_INT) SV_CTOR_END_M();
		SV_GETSIZE_M(int);
		SV_GETTYPE_M();
		SV_CLONE_M(ScriptIntValue);
		SV_BOOLEAN();
		SV_GETVAL(ScriptIntValue, int);
		SV_SETVAL(int);
		SV_ASSIGN(ScriptIntValue);
	};

	class ScriptUnsignedIntValue : public ScriptValue
	{
	protected:
		unsigned int val;
		SV_TYPE_M();

	public:
		SV_CTOR_M(ScriptUnsignedIntValue, unsigned int) SV_VAL_INIT(v) SV_TYPE_INIT(VALUE_TYPE_T::VT_UNSIGNED_INT) SV_CTOR_END_M();
		SV_GETSIZE_M(unsigned int);
		SV_GETTYPE_M();
		SV_CLONE_M(ScriptUnsignedIntValue);
		SV_BOOLEAN();
		SV_GETVAL(ScriptUnsignedIntValue, unsigned int);
		SV_SETVAL(unsigned int);
		SV_ASSIGN(ScriptUnsignedIntValue);
	};

	class ScriptFloatValue : public ScriptValue
	{
	protected:
		float val;
		SV_TYPE_M();

	public:
		SV_CTOR_M(ScriptFloatValue, float) SV_VAL_INIT(v) SV_TYPE_INIT(VALUE_TYPE_T::VT_FLOAT) SV_CTOR_END_M();
		SV_GETSIZE_M(float);
		SV_GETTYPE_M();
		SV_CLONE_M(ScriptFloatValue);
		SV_BOOLEAN();
		SV_GETVAL(ScriptFloatValue, float);
		SV_SETVAL(float);
		SV_ASSIGN(ScriptFloatValue);
	};

	class ScriptDoubleValue : public ScriptValue
	{
	protected:
		double val;
		SV_TYPE_M();

	public:
		SV_CTOR_M(ScriptDoubleValue, double) SV_VAL_INIT(v) SV_TYPE_INIT(VALUE_TYPE_T::VT_DOUBLE) SV_CTOR_END_M();
		SV_GETSIZE_M(double);
		SV_GETTYPE_M();
		SV_CLONE_M(ScriptDoubleValue);
		SV_BOOLEAN();
		SV_GETVAL(ScriptDoubleValue, double);
		SV_SETVAL(double);
		SV_ASSIGN(ScriptDoubleValue);
	};

	class ScriptStringValue : public ScriptValue
	{
	protected:
		std::string val;
		SV_TYPE_M();

	public:
		SV_CTOR_M(ScriptStringValue, std::string) SV_VAL_INIT(v) SV_TYPE_INIT(VALUE_TYPE_T::VT_STRING) SV_CTOR_END_M();
		SV_GETSIZE_M(std::string);
		SV_GETTYPE_M();
		SV_CLONE_M(ScriptStringValue);
		SV_GETVAL(ScriptStringValue, std::string);
		SV_SETVAL(const std::string&);
		SV_ASSIGN(ScriptStringValue);

		virtual ScriptBoolValue boolean() override
		{
			return ScriptBoolValue(this->val.length());
		}
	};

	class ScriptClassValue : public ScriptValue
	{
	protected:
		ScriptClassInstance * val;
		SV_TYPE_M();

	public:
		SCRIPT_API ScriptClassValue(ScriptClassInstance *inst);
		SCRIPT_API ScriptClassValue(ScriptClassInstance *inst, ScriptClass &sclass);
		//SCRIPT_API SV_CTOR_NULL_M(ScriptClassValue, ScriptClassInstance*);
		SV_GETSIZE_M(val);
		SV_GETTYPE_M();
		SV_CLONE_M(ScriptClassValue);
		SV_BOOLEAN();
		SV_GETVAL(ScriptClassValue, ScriptClassInstance*);
		SV_SETVAL(ScriptClassInstance*);
		SV_ASSIGN(ScriptClassValue);
	};

	class ScriptArrayValue : public ScriptValue
	{
	protected:
		ScriptValue * *val;
		SV_TYPE_M();

	public:
		ScriptArrayValue(const ScriptArrayType *type = NULL, ScriptValue **val = NULL)
			:val(val),
			type(type)
		{ }

		SV_GETSIZE_M(val);
		SV_GETTYPE_M();
		SV_CLONE_M(ScriptArrayValue);
		SV_BOOLEAN();
		SV_GETVAL(ScriptArrayValue, ScriptValue**);
		SV_ASSIGN(ScriptArrayValue);
	};

	class ScriptReferenceValue : public ScriptValue
	{
	protected:
		ScriptValue * val;
		SV_TYPE_M();

	public:
		ScriptReferenceValue(const ScriptReferenceType *type = NULL, ScriptValue *val = NULL)
			:val(val),
			type(type)
		{ }

		ScriptReferenceValue(ScriptValue *v)
		{
			this->retarget(v);
		}

		SV_GETSIZE_M(val);
		SV_GETTYPE_M();
		SV_CLONE_M(ScriptReferenceValue);
		SV_BOOLEAN();

		template<typename T>
		T resolve() const
		{
			return static_cast<T>(this->getValue());
		}

		virtual void *getValue() const
		{
			switch (this->type->getTypeDescriptor())
			{
			case VALUE_TYPE_T::VT_INT: return reinterpret_cast<void*>(static_cast<ScriptIntValue*>(this->val)->getValue());
			case VALUE_TYPE_T::VT_UNSIGNED_INT: return reinterpret_cast<void*>(static_cast<ScriptUnsignedIntValue*>(this->val)->getValue());
			case VALUE_TYPE_T::VT_BOOL: return reinterpret_cast<void*>(static_cast<ScriptBoolValue*>(this->val)->getValue());
			case VALUE_TYPE_T::VT_BYTE: return reinterpret_cast<void*>(static_cast<ScriptByteValue*>(this->val)->getValue());
			case VALUE_TYPE_T::VT_CHAR: return reinterpret_cast<void*>(static_cast<ScriptCharValue*>(this->val)->getValue());
			case VALUE_TYPE_T::VT_CLASS: return reinterpret_cast<void*>(static_cast<ScriptClassValue*>(this->val)->getValue());
			case VALUE_TYPE_T::VT_STRING: return reinterpret_cast<void*>(&static_cast<ScriptStringValue*>(this->val)->getValue());
			}

			return this->val;
		}

		virtual void assign(const ScriptValue &val) override
		{
			//this->val->assign(*static_cast<const ScriptReferenceValue&>(val).val);
			this->val->assign(val);
		}

		void retarget(ScriptValue *val)
		{
			this->val = val;
		}

		virtual void setValue(ScriptValue *val)
		{
			this->val->assign(*val);
			//this->val->setv = val;
		}

		void put(ScriptValue *val) const
		{
			this->val->assign(*val);
		}
	};

	class ScriptNullValue : public ScriptValue
	{
	protected:
		SV_TYPE_M();

	public:
		int getSize() override
		{
			return 0;
		}

		SV_GETTYPE_M();

		SV_CLONE_M(ScriptNullValue);

		virtual void assign(const ScriptValue &val) {}

		virtual ScriptBoolValue boolean() override
		{
			return ScriptBoolValue(false);
		}
	};
}

#endif