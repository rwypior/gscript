#ifndef _h_gscript_value
#define _h_gscript_value

#include "defs.hpp"
#include "type.hpp"
#include "lib.hpp"

#include <string>

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
	class ScriptClass;
	class ScriptBoolValue;

	class ScriptValue
	{
	public:
		SCRIPT_API virtual ~ScriptValue() = default;

		virtual void assign(const ScriptValue &val) = 0;
		virtual size_t getSize() const = 0;
		virtual const ScriptType *getType() = 0;
		virtual ScriptValue *clone() const = 0;

		virtual ScriptBoolValue boolean() const = 0;

		template<typename T>
		T &as()
		{
			return static_cast<T&>(*this);
		}
	};

	class ScriptBoolValue : public ScriptValue
	{
	public:
		ScriptBoolValue(bool val = {})
			: val(val)
			, type(new ScriptType(VALUE_TYPE_T::VT_BOOL))
		{ }

		virtual size_t getSize() const override
		{
			return sizeof(bool);
		}

		virtual const ScriptType *getType() override
		{
			return this->type;
		}

		virtual ScriptValue* clone() const override
		{
			return new ScriptBoolValue(*this);
		}

		virtual ScriptBoolValue boolean() const override
		{
			return ScriptBoolValue(this->val);
		}

		virtual bool getValue() const
		{
			return this->val;
		}

		virtual void setValue(bool val)
		{
			this->val = val;
		}

		virtual void assign(const ScriptValue& val) override
		{
			this->val = static_cast<const ScriptBoolValue&>(val).val;
		}

	private:
		bool val = 0;
		const ScriptType* type = nullptr;
	};

	class ScriptCharValue : public ScriptValue
	{
	public:
		ScriptCharValue(char val = {})
			: val(val)
			, type(new ScriptType(VALUE_TYPE_T::VT_CHAR))
		{ }

		virtual size_t getSize() const override
		{
			return sizeof(char);
		}

		virtual const ScriptType* getType() override
		{
			return this->type;
		}

		virtual ScriptValue* clone() const override
		{
			return new ScriptCharValue(*this);
		}

		virtual ScriptBoolValue boolean() const override
		{
			return ScriptBoolValue(this->val);
		}

		virtual char getValue() const
		{
			return this->val;
		}

		virtual void setValue(char val)
		{
			this->val = val;
		}

		virtual void assign(const ScriptValue& val) override
		{
			this->val = static_cast<const ScriptCharValue&>(val).val;
		}

	protected:
		char val = 0;
		const ScriptType* type = nullptr;
	};

	class ScriptByteValue : public ScriptValue
	{
	public:
		ScriptByteValue(unsigned char val = {})
			: val(val)
			, type(new ScriptType(VALUE_TYPE_T::VT_BYTE))
		{ }

		virtual size_t getSize() const override
		{
			return sizeof(unsigned char);
		}

		virtual const ScriptType* getType() override
		{
			return this->type;
		}

		virtual ScriptValue* clone() const override
		{
			return new ScriptByteValue(*this);
		}

		virtual ScriptBoolValue boolean() const override
		{
			return ScriptBoolValue(this->val);
		}

		virtual unsigned char getValue() const
		{
			return this->val;
		}

		virtual void setValue(unsigned char val)
		{
			this->val = val;
		}

		virtual void assign(const ScriptValue& val) override
		{
			this->val = static_cast<const ScriptByteValue&>(val).val;
		}

	protected:
		unsigned char val = 0;
		const ScriptType* type = nullptr;
	};

	class ScriptIntValue : public ScriptValue
	{
	public:
		ScriptIntValue(int val = {})
			: val(val)
			, type(new ScriptType(VALUE_TYPE_T::VT_INT))
		{ }

		virtual size_t getSize() const override
		{
			return sizeof(int);
		}

		virtual const ScriptType* getType() override
		{
			return this->type;
		}

		virtual ScriptValue* clone() const override
		{
			return new ScriptIntValue(*this);
		}

		virtual ScriptBoolValue boolean() const override
		{
			return ScriptBoolValue(this->val);
		}

		virtual int getValue() const
		{
			return this->val;
		}

		virtual void setValue(int val)
		{
			this->val = val;
		}

		virtual void assign(const ScriptValue& val) override
		{
			this->val = static_cast<const ScriptIntValue&>(val).val;
		}

	protected:
		int val = 0;
		const ScriptType* type = nullptr;
	};

	class ScriptUnsignedIntValue : public ScriptValue
	{
	public:
		ScriptUnsignedIntValue(unsigned int val = {})
			: val(val)
			, type(new ScriptType(VALUE_TYPE_T::VT_UNSIGNED_INT))
		{ }

		virtual size_t getSize() const override
		{
			return sizeof(unsigned int);
		}

		virtual const ScriptType* getType() override
		{
			return this->type;
		}

		virtual ScriptValue* clone() const override
		{
			return new ScriptUnsignedIntValue(*this);
		}

		virtual ScriptBoolValue boolean() const override
		{
			return ScriptBoolValue(this->val);
		}

		virtual unsigned int getValue() const
		{
			return this->val;
		}

		virtual void setValue(unsigned int val)
		{
			this->val = val;
		}

		virtual void assign(const ScriptValue& val) override
		{
			this->val = static_cast<const ScriptUnsignedIntValue&>(val).val;
		}

	protected:
		unsigned int val = 0;
		const ScriptType* type = nullptr;
	};

	class ScriptFloatValue : public ScriptValue
	{
	public:
		ScriptFloatValue(float val = {})
			: val(val)
			, type(new ScriptType(VALUE_TYPE_T::VT_FLOAT))
		{ }

		virtual size_t getSize() const override
		{
			return sizeof(float);
		}

		virtual const ScriptType* getType() override
		{
			return this->type;
		}

		virtual ScriptValue* clone() const override
		{
			return new ScriptFloatValue(*this);
		}

		virtual ScriptBoolValue boolean() const override
		{
			return ScriptBoolValue(this->val);
		}

		virtual float getValue() const
		{
			return this->val;
		}

		virtual void setValue(float val)
		{
			this->val = val;
		}

		virtual void assign(const ScriptValue& val) override
		{
			this->val = static_cast<const ScriptFloatValue&>(val).val;
		}

	protected:
		float val = 0;
		const ScriptType* type = nullptr;
	};

	class ScriptDoubleValue : public ScriptValue
	{
	public:
		ScriptDoubleValue(double val = {})
			: val(val)
			, type(new ScriptType(VALUE_TYPE_T::VT_DOUBLE))
		{ }

		virtual size_t getSize() const override
		{
			return sizeof(double);
		}

		virtual const ScriptType* getType() override
		{
			return this->type;
		}

		virtual ScriptValue* clone() const override
		{
			return new ScriptDoubleValue(*this);
		}

		virtual ScriptBoolValue boolean() const override
		{
			return ScriptBoolValue(this->val);
		}

		virtual double getValue() const
		{
			return this->val;
		}

		virtual void setValue(double val)
		{
			this->val = val;
		}

		virtual void assign(const ScriptValue& val) override
		{
			this->val = static_cast<const ScriptDoubleValue&>(val).val;
		}

	protected:
		double val = 0;
		const ScriptType* type = nullptr;
	};

	class ScriptStringValue : public ScriptValue
	{
	public:
		ScriptStringValue(const std::string val = {})
			: val(val)
			, type(new ScriptType(VALUE_TYPE_T::VT_STRING))
		{ }

		virtual size_t getSize() const override
		{
			return sizeof(std::string);
		}

		virtual const ScriptType* getType() override
		{
			return this->type;
		}

		virtual ScriptValue* clone() const override
		{
			return new ScriptStringValue(*this);
		}

		virtual ScriptBoolValue boolean() const override
		{
			return ScriptBoolValue(!this->val.empty());
		}

		virtual const std::string& getValue() const
		{
			return this->val;
		}

		virtual void setValue(const std::string& val)
		{
			this->val = val;
		}

		virtual void assign(const ScriptValue& val) override
		{
			this->val = static_cast<const ScriptStringValue&>(val).val;
		}

	protected:
		std::string val;
		const ScriptType* type = nullptr;
	};

	class ScriptClassValue : public ScriptValue
	{
	public:
		ScriptClassValue(ScriptClassInstance* val);		
		ScriptClassValue(ScriptClassInstance* val, ScriptClass& sclass);

		virtual size_t getSize() const override
		{
			return sizeof(val);
		}

		virtual const ScriptType* getType() override
		{
			return this->type;
		}

		virtual ScriptValue* clone() const override
		{
			return new ScriptClassValue(*this);
		}

		virtual ScriptBoolValue boolean() const override
		{
			return ScriptBoolValue(this->val);
		}

		virtual ScriptClassInstance* getValue() const
		{
			return this->val;
		}

		virtual void setValue(ScriptClassInstance* val)
		{
			this->val = val;
		}

		virtual void assign(const ScriptValue& val) override
		{
			this->val = static_cast<const ScriptClassValue&>(val).val;
		}

	protected:
		ScriptClassInstance *val = nullptr;
		const ScriptType* type = nullptr;
	};

	class ScriptArrayValue : public ScriptValue
	{
	public:
		ScriptArrayValue(const ScriptArrayType *type = nullptr, ScriptValue **val = nullptr)
			: val(val)
			, type(type)
		{ }

		virtual size_t getSize() const override
		{
			return sizeof(val);
		}

		virtual const ScriptType* getType() override
		{
			return this->type;
		}

		virtual ScriptValue* clone() const override
		{
			return new ScriptArrayValue(*this);
		}

		virtual ScriptBoolValue boolean() const override
		{
			return ScriptBoolValue(this->val);
		}

		virtual ScriptValue** getValue() const
		{
			return this->val;
		}

		virtual void setValue(ScriptValue** val)
		{
			this->val = val;
		}

		virtual void assign(const ScriptValue & val) override
		{
			this->val = static_cast<const ScriptArrayValue&>(val).val;
		}

	protected:
		ScriptValue **val = nullptr;
		const ScriptType* type = nullptr;
	};

	class ScriptReferenceValue : public ScriptValue
	{
	public:
		ScriptReferenceValue(const ScriptReferenceType *type = nullptr, ScriptValue *val = nullptr)
			: val(val)
			, type(type)
		{
		}

		ScriptReferenceValue(ScriptValue *v)
		{
			this->retarget(v);
		}

		virtual size_t getSize() const override
		{
			return sizeof(val);
		}

		virtual const ScriptType* getType() override
		{
			return this->type;
		}

		virtual ScriptValue* clone() const override
		{
			return new ScriptReferenceValue(*this);
		}

		virtual ScriptBoolValue boolean() const override
		{
			return ScriptBoolValue(this->val);
		}

		template<typename T>
		T resolve() const
		{
			return static_cast<T>(this->getValue());
		}

		/*virtual void *getValue() const
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
		}*/

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

	protected:
		ScriptValue *val = nullptr;
		const ScriptType* type = nullptr;
	};

	class ScriptNullValue : public ScriptValue
	{
	public:
		size_t getSize() const override
		{
			return 0;
		}

		virtual const ScriptType* getType() override
		{
			return nullptr;
		}

		virtual ScriptValue* clone() const override
		{
			return new ScriptNullValue();
		}

		virtual void assign(const ScriptValue &) override
		{
		}

		virtual ScriptBoolValue boolean() const override
		{
			return ScriptBoolValue(false);
		}
	};
}

#endif