#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/classInstance.hpp"

namespace gscript
{
	ScriptValue::~ScriptValue() = default;

	std::unique_ptr<ScriptReferenceValue> ScriptValue::makeref()
	{
		return std::make_unique<ScriptReferenceValue>(this);
	}

	// Bool

	ScriptBoolValue::ScriptBoolValue(const ScriptBoolValue& b)
		: val(b.val)
	{
	}

	ScriptBoolValue::ScriptBoolValue(ScriptBoolValue&& b) noexcept
		: val(std::move(b.val))
	{
	}

	ScriptBoolValue::ScriptBoolValue(bool val)
		: val(val)
	{
	}

	size_t ScriptBoolValue::getSize() const
	{
		return sizeof(bool);
	}

	const std::shared_ptr<ScriptType> ScriptBoolValue::getType()
	{
		return std::make_unique<ScriptType>(ValueType::Bool);
	}

	std::unique_ptr<ScriptValue> ScriptBoolValue::clone() const
	{
		return std::make_unique<ScriptBoolValue>(*this);
	}

	ScriptBoolValue ScriptBoolValue::boolean() const
	{
		return ScriptBoolValue(this->val);
	}

	bool ScriptBoolValue::getValue() const
	{
		return this->val;
	}

	void ScriptBoolValue::setValue(bool val)
	{
		this->val = val;
	}

	void ScriptBoolValue::assign(const ScriptValue& val)
	{
		this->val = static_cast<const ScriptBoolValue&>(val).val;
	}

	// Char

	ScriptCharValue::ScriptCharValue(const ScriptCharValue& b)
		: val(b.val)
	{
	}

	ScriptCharValue::ScriptCharValue(ScriptCharValue&& b) noexcept
		: val(std::move(b.val))
	{
	}

	ScriptCharValue::ScriptCharValue(char val)
		: val(val)
	{
	}

	size_t ScriptCharValue::getSize() const
	{
		return sizeof(char);
	}

	const std::shared_ptr<ScriptType> ScriptCharValue::getType()
	{
		return std::make_unique<ScriptType>(ValueType::Char);
	}

	std::unique_ptr<ScriptValue> ScriptCharValue::clone() const
	{
		return std::make_unique<ScriptCharValue>(*this);
	}

	ScriptBoolValue ScriptCharValue::boolean() const
	{
		return ScriptBoolValue(this->val);
	}

	char ScriptCharValue::getValue() const
	{
		return this->val;
	}

	void ScriptCharValue::setValue(char val)
	{
		this->val = val;
	}

	void ScriptCharValue::assign(const ScriptValue& val)
	{
		this->val = static_cast<const ScriptCharValue&>(val).val;
	}

	// Byte

	ScriptByteValue::ScriptByteValue(const ScriptByteValue& b)
		: val(b.val)
	{
	}

	ScriptByteValue::ScriptByteValue(ScriptByteValue&& b) noexcept
		: val(std::move(b.val))
	{
	}

	ScriptByteValue::ScriptByteValue(unsigned char val)
		: val(val)
	{
	}

	size_t ScriptByteValue::getSize() const
	{
		return sizeof(unsigned char);
	}

	const std::shared_ptr<ScriptType> ScriptByteValue::getType()
	{
		return std::make_unique<ScriptType>(ValueType::Byte);
	}

	std::unique_ptr<ScriptValue> ScriptByteValue::clone() const
	{
		return std::make_unique<ScriptByteValue>(*this);
	}

	ScriptBoolValue ScriptByteValue::boolean() const
	{
		return ScriptBoolValue(this->val);
	}

	unsigned char ScriptByteValue::getValue() const
	{
		return this->val;
	}

	void ScriptByteValue::setValue(unsigned char val)
	{
		this->val = val;
	}

	void ScriptByteValue::assign(const ScriptValue& val)
	{
		this->val = static_cast<const ScriptByteValue&>(val).val;
	}

	// Int

	ScriptIntValue::ScriptIntValue(const ScriptIntValue& b)
		: val(b.val)
	{
	}

	ScriptIntValue::ScriptIntValue(ScriptIntValue&& b) noexcept
		: val(std::move(b.val))
	{
	}

	ScriptIntValue::ScriptIntValue(int val)
		: val(val)
	{
	}

	size_t ScriptIntValue::getSize() const
	{
		return sizeof(int);
	}

	const std::shared_ptr<ScriptType> ScriptIntValue::getType()
	{
		return std::make_unique<ScriptType>(ValueType::Int);
	}

	std::unique_ptr<ScriptValue> ScriptIntValue::clone() const
	{
		return std::make_unique<ScriptIntValue>(*this);
	}

	ScriptBoolValue ScriptIntValue::boolean() const
	{
		return ScriptBoolValue(this->val);
	}

	int ScriptIntValue::getValue() const
	{
		return this->val;
	}

	void ScriptIntValue::setValue(int val)
	{
		this->val = val;
	}

	void ScriptIntValue::assign(const ScriptValue& val)
	{
		this->val = static_cast<const ScriptIntValue*>(val.data())->val;
	}

	// Unsigned int

	ScriptUnsignedIntValue::ScriptUnsignedIntValue(const ScriptUnsignedIntValue& b)
		: val(b.val)
	{
	}

	ScriptUnsignedIntValue::ScriptUnsignedIntValue(ScriptUnsignedIntValue&& b) noexcept
		: val(std::move(val))
	{
	}

	ScriptUnsignedIntValue::ScriptUnsignedIntValue(unsigned int val)
		: val(val)
	{ }

	size_t ScriptUnsignedIntValue::getSize() const
	{
		return sizeof(unsigned int);
	}

	const std::shared_ptr<ScriptType> ScriptUnsignedIntValue::getType()
	{
		return std::make_unique<ScriptType>(ValueType::UnsignedInt);
	}

	std::unique_ptr<ScriptValue> ScriptUnsignedIntValue::clone() const
	{
		return std::make_unique<ScriptUnsignedIntValue>(*this);
	}

	ScriptBoolValue ScriptUnsignedIntValue::boolean() const
	{
		return ScriptBoolValue(this->val);
	}

	unsigned int ScriptUnsignedIntValue::getValue() const
	{
		return this->val;
	}

	void ScriptUnsignedIntValue::setValue(unsigned int val)
	{
		this->val = val;
	}

	void ScriptUnsignedIntValue::assign(const ScriptValue& val)
	{
		this->val = static_cast<const ScriptUnsignedIntValue&>(val).val;
	}

	// Float

	ScriptFloatValue::ScriptFloatValue(const ScriptFloatValue& b)
		: val(b.val)
	{
	}

	ScriptFloatValue::ScriptFloatValue(ScriptFloatValue&& b) noexcept
		: val(std::move(b.val))
	{
	}

	ScriptFloatValue::ScriptFloatValue(float val)
		: val(val)
	{
	}

	size_t ScriptFloatValue::getSize() const
	{
		return sizeof(float);
	}

	const std::shared_ptr<ScriptType> ScriptFloatValue::getType()
	{
		return std::make_unique<ScriptType>(ValueType::Float);
	}

	std::unique_ptr<ScriptValue> ScriptFloatValue::clone() const
	{
		return std::make_unique<ScriptFloatValue>(*this);
	}

	ScriptBoolValue ScriptFloatValue::boolean() const
	{
		return ScriptBoolValue(this->val);
	}

	float ScriptFloatValue::getValue() const
	{
		return this->val;
	}

	void ScriptFloatValue::setValue(float val)
	{
		this->val = val;
	}

	void ScriptFloatValue::assign(const ScriptValue& val)
	{
		this->val = static_cast<const ScriptFloatValue&>(val).val;
	}

	// Double

	ScriptDoubleValue::ScriptDoubleValue(const ScriptDoubleValue& b)
		: val(b.val)
	{
	}

	ScriptDoubleValue::ScriptDoubleValue(ScriptDoubleValue&& b) noexcept
		: val(std::move(b.val))
	{
	}

	ScriptDoubleValue::ScriptDoubleValue(double val)
		: val(val)
	{
	}

	size_t ScriptDoubleValue::getSize() const
	{
		return sizeof(double);
	}

	const std::shared_ptr<ScriptType> ScriptDoubleValue::getType()
	{
		return std::make_unique<ScriptType>(ValueType::Double);
	}

	std::unique_ptr<ScriptValue> ScriptDoubleValue::clone() const
	{
		return std::make_unique<ScriptDoubleValue>(*this);
	}

	ScriptBoolValue ScriptDoubleValue::boolean() const
	{
		return ScriptBoolValue(this->val);
	}

	double ScriptDoubleValue::getValue() const
	{
		return this->val;
	}

	void ScriptDoubleValue::setValue(double val)
	{
		this->val = val;
	}

	void ScriptDoubleValue::assign(const ScriptValue& val)
	{
		this->val = static_cast<const ScriptDoubleValue&>(val).val;
	}

	// String

	ScriptStringValue::ScriptStringValue(const ScriptStringValue& b)
		: val(b.val)
	{
	}

	ScriptStringValue::ScriptStringValue(ScriptStringValue&& b) noexcept
		: val(std::move(b.val))
	{
	}

	ScriptStringValue::ScriptStringValue(const std::string val)
		: val(val)
	{
	}

	size_t ScriptStringValue::getSize() const
	{
		return sizeof(std::string);
	}

	const std::shared_ptr<ScriptType> ScriptStringValue::getType()
	{
		return std::make_unique<ScriptType>(ValueType::String);
	}

	std::unique_ptr<ScriptValue> ScriptStringValue::clone() const
	{
		return std::make_unique<ScriptStringValue>(*this);
	}

	ScriptBoolValue ScriptStringValue::boolean() const
	{
		return ScriptBoolValue(!this->val.empty());
	}

	const std::string& ScriptStringValue::getValue() const
	{
		return this->val;
	}

	void ScriptStringValue::setValue(const std::string& val)
	{
		this->val = val;
	}

	void ScriptStringValue::assign(const ScriptValue& val)
	{
		this->val = static_cast<const ScriptStringValue&>(val).val;
	}

	// Class

	ScriptClassValue::ScriptClassValue(const ScriptClassValue& b)
		: val(std::make_unique<ScriptClassInstance>(*b.val))
		, type(b.type)
	{
	}

	ScriptClassValue::ScriptClassValue(ScriptClassValue&& b) noexcept
		: val(std::move(b.val))
		, type(b.type)
	{
	}

	ScriptClassValue::ScriptClassValue(std::unique_ptr<ScriptClassInstance>&& v)
		: type(std::make_shared<ScriptClassType>(v->getClass()))
		, val(std::move(v))
	{
	}

	ScriptClassValue::ScriptClassValue(std::unique_ptr<ScriptClassInstance>&& v, ScriptClass &sclass)
		: val(std::move(v))
		, type(std::make_shared<ScriptClassType>(sclass))
	{
	}

	size_t ScriptClassValue::getSize() const
	{
		return sizeof(val);
	}

	const std::shared_ptr<ScriptType> ScriptClassValue::getType()
	{
		return this->type;
	}

	std::unique_ptr<ScriptValue> ScriptClassValue::clone() const
	{
		return std::make_unique<ScriptClassValue>(*this);
	}

	ScriptBoolValue ScriptClassValue::boolean() const
	{
		return ScriptBoolValue(this->val.get());
	}

	std::unique_ptr<ScriptClassInstance>& ScriptClassValue::getValue()
	{
		return this->val;
	}

	void ScriptClassValue::setValue(std::unique_ptr<ScriptClassInstance>&& val)
	{
		this->val = std::move(val);
	}

	void ScriptClassValue::assign(const ScriptValue& val)
	{
		//this->val = static_cast<const ScriptClassValue&>(val).val;
		this->val = std::move(static_cast<ScriptClassValue*>(val.clone().get())->val); // TODO - this should probably make a copy instead of moving
	}

	// Array

	ScriptArrayValue::~ScriptArrayValue() = default;

	ScriptArrayValue::ScriptArrayValue() = default;

	ScriptArrayValue::ScriptArrayValue(const ScriptArrayValue& b)
		: ScriptArrayValue(b.type, {})
	{
		for (auto& entry : b.val)
		{
			this->val.push_back(entry->clone());
		}
	}

	ScriptArrayValue::ScriptArrayValue(ScriptArrayValue&& b) noexcept
		: ScriptArrayValue(std::move(b.type), std::move(b.val))
	{
	}

	ScriptArrayValue::ScriptArrayValue(std::shared_ptr<ScriptType> elementType, const std::vector<std::unique_ptr<ScriptValue>>& val)
		: type(std::make_shared<ScriptArrayType>(elementType))
	{
		for (auto& el : val)
		{
			this->val.push_back(el->clone());
		}
	}

	ScriptArrayValue::ScriptArrayValue(std::shared_ptr<ScriptType> elementType, std::vector<std::unique_ptr<ScriptValue>>&& val)
		: type(std::make_shared<ScriptArrayType>(elementType))
		, val(std::move(val))
	{
	}

	ScriptArrayValue::ScriptArrayValue(const std::vector<std::unique_ptr<ScriptValue>>& val)
	{
		if (val.empty())
			throw RuntimeException("Array created directly from vector must have at least one element");

		this->type = std::make_shared<ScriptArrayType>(val.front()->getType());

		for (auto& entry : val)
		{
			this->val.push_back(entry->clone());
		}
	}

	ScriptArrayValue::ScriptArrayValue(std::vector<std::unique_ptr<ScriptValue>>&& val)
	{
		if (val.empty())
			throw RuntimeException("Array created directly from vector must have at least one element");

		this->type = std::make_shared<ScriptArrayType>(val.front()->getType());

		for (auto& entry : val)
		{
			this->val.push_back(std::move(entry));
		}
	}

	size_t ScriptArrayValue::getSize() const
	{
		return sizeof(val);
	}

	const std::shared_ptr<ScriptType> ScriptArrayValue::getType()
	{
		return this->type;
	}

	std::unique_ptr<ScriptValue> ScriptArrayValue::clone() const
	{
		return std::make_unique<ScriptArrayValue>(*this);
	}

	ScriptBoolValue ScriptArrayValue::boolean() const
	{
		return ScriptBoolValue(!this->val.empty());
	}

	const std::vector<std::unique_ptr<ScriptValue>>& ScriptArrayValue::getValue() const
	{
		return this->val;
	}

	std::vector<std::unique_ptr<ScriptValue>>& ScriptArrayValue::getValue()
	{
		return this->val;
	}

	void ScriptArrayValue::setValue(const std::vector<std::unique_ptr<ScriptValue>>& val)
	{
		this->val.clear();
		for (auto& el : val)
		{
			this->val.push_back(el->clone());
		}
	}

	void ScriptArrayValue::setValue(std::vector<std::unique_ptr<ScriptValue>>&& val)
	{
		this->val = std::move(val);
	}

	void ScriptArrayValue::assign(const ScriptValue& val)
	{
		this->val.clear();
		for (auto& el : static_cast<const ScriptArrayValue&>(val).val)
		{
			this->val.push_back(el->clone());
		}
	}

	std::unique_ptr<ScriptValue>& ScriptArrayValue::at(size_t idx)
	{
		return this->val.at(idx);
	}
	
	// Reference

	ScriptReferenceValue::ScriptReferenceValue(const ScriptReferenceValue& b)
		: type(b.type)
		, val(b.val)
	{
	}

	ScriptReferenceValue::ScriptReferenceValue(ScriptReferenceValue&& b) noexcept
		: type(std::move(b.type))
		, val(std::move(b.val))
	{
	}

	ScriptReferenceValue::ScriptReferenceValue(std::shared_ptr<ScriptReferenceType> type, ScriptValue* val)
		: type(std::move(type))
		, val(val)
	{
	}

	ScriptReferenceValue::ScriptReferenceValue(ScriptValue* v)
	{
		this->type = v->getType();
		this->retarget(v);
	}

	std::unique_ptr<ScriptReferenceValue> ScriptReferenceValue::create(std::unique_ptr<ScriptValue>& v)
	{
		return std::make_unique<ScriptReferenceValue>(std::make_shared<ScriptReferenceType>(v->getType()), v.get());
	}

	std::unique_ptr<ScriptReferenceValue> ScriptReferenceValue::create(ScriptValue* v)
	{
		return std::make_unique<ScriptReferenceValue>(std::make_shared<ScriptReferenceType>(v->getType()), v);
	}

	size_t ScriptReferenceValue::getSize() const
	{
		return sizeof(val);
	}

	const std::shared_ptr<ScriptType> ScriptReferenceValue::getType()
	{
		return this->type;
	}

	std::unique_ptr<ScriptValue> ScriptReferenceValue::clone() const
	{
		return std::make_unique<ScriptReferenceValue>(*this);
	}

	ScriptBoolValue ScriptReferenceValue::boolean() const
	{
		return ScriptBoolValue(this->val);
	}

	ScriptValue* ScriptReferenceValue::getValue() const
	{
		return this->val;
	}

	void ScriptReferenceValue::assign(const ScriptValue& val)
	{
		this->val->assign(val);
	}

	void ScriptReferenceValue::retarget(ScriptValue* val)
	{
		this->val = val;
	}

	void ScriptReferenceValue::setValue(ScriptValue* val)
	{
		this->val->assign(*val);
		//this->val->setv = val;
	}

	void ScriptReferenceValue::put(ScriptValue* val) const
	{
		this->val->assign(*val);
	}

	// Null

	size_t ScriptNullValue::getSize() const
	{
		return 0;
	}

	const std::shared_ptr<ScriptType> ScriptNullValue::getType()
	{
		return nullptr;
	}

	std::unique_ptr<ScriptValue> ScriptNullValue::clone() const
	{
		return std::make_unique<ScriptNullValue>();
	}

	void ScriptNullValue::assign(const ScriptValue&)
	{
	}

	ScriptBoolValue ScriptNullValue::boolean() const
	{
		return ScriptBoolValue(false);
	}

	ScriptNull ScriptNullValue::getValue() const
	{
		return {};
	}

	// Return value

	ScriptReturnValue::ScriptReturnValue(std::unique_ptr<ScriptValue>&& val)
		: val(std::move(val))
	{
	}

	size_t ScriptReturnValue::getSize() const
	{
		return this->val ? this->val->getSize() : 0;
	}

	const std::shared_ptr<ScriptType> ScriptReturnValue::getType()
	{
		return this->val ? this->val->getType() : 0;
	}

	std::unique_ptr<ScriptValue> ScriptReturnValue::clone() const
	{
		return this->val ? this->val->clone() : nullptr;
	}

	void ScriptReturnValue::assign(const ScriptValue& val)
	{
		if (this->val)
			this->val->assign(val);
	}

	ScriptBoolValue ScriptReturnValue::boolean() const
	{
		return !!this->val;
	}

	std::unique_ptr<ScriptValue>& ScriptReturnValue::getValue()
	{
		return this->val;
	}

	ScriptValue* ScriptReturnValue::data()
	{
		return this->val ? this->val->data() : nullptr;
	}

	const ScriptValue* ScriptReturnValue::data() const
	{
		return this->val ? this->val->data() : nullptr;
	}

	std::unique_ptr<ScriptValue> ScriptReturnValue::returnedData()
	{
		return std::move(this->val);
	}

	bool ScriptReturnValue::isReturnedData() const
	{
		return !!this->val;
	}
}