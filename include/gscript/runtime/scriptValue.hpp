#ifndef _h_gscript_value
#define _h_gscript_value

#include "defs.hpp"
#include "type.hpp"
#include "lib.hpp"
#include "runtimeException.hpp"

#include <string>
#include <vector>
#include <memory>

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

		SCRIPT_API virtual ScriptValue* data() { return this; };
		SCRIPT_API virtual const ScriptValue* data() const { return this; };

		SCRIPT_API virtual void assign(const ScriptValue &val) = 0;
		SCRIPT_API virtual size_t getSize() const = 0;
		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() = 0;
		SCRIPT_API virtual std::unique_ptr<ScriptValue> clone() const = 0;

		SCRIPT_API virtual ScriptBoolValue boolean() const = 0;

		template<typename T>
		T &as()
		{
			return static_cast<T&>(*this->data());
		}
	};

	class ScriptBoolValue : public ScriptValue
	{
	public:
		SCRIPT_API ScriptBoolValue(const ScriptBoolValue& b);
		SCRIPT_API ScriptBoolValue(ScriptBoolValue&& b) noexcept;
		SCRIPT_API ScriptBoolValue(bool val = {});

		SCRIPT_API virtual size_t getSize() const override;
		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() override;
		SCRIPT_API virtual std::unique_ptr<ScriptValue> clone() const override;
		SCRIPT_API virtual ScriptBoolValue boolean() const override;
		SCRIPT_API virtual bool getValue() const;
		SCRIPT_API virtual void setValue(bool val);
		SCRIPT_API virtual void assign(const ScriptValue& val) override;

	private:
		bool val = false;
	};

	class ScriptCharValue : public ScriptValue
	{
	public:
		SCRIPT_API ScriptCharValue(const ScriptCharValue& b);
		SCRIPT_API ScriptCharValue(ScriptCharValue&& b) noexcept;
		SCRIPT_API ScriptCharValue(char val = {});

		SCRIPT_API virtual size_t getSize() const override;
		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() override;
		SCRIPT_API virtual std::unique_ptr<ScriptValue> clone() const override;
		SCRIPT_API virtual ScriptBoolValue boolean() const override;
		SCRIPT_API virtual char getValue() const;
		SCRIPT_API virtual void setValue(char val);
		SCRIPT_API virtual void assign(const ScriptValue& val) override;

	protected:
		char val = 0;
	};

	class ScriptByteValue : public ScriptValue
	{
	public:
		SCRIPT_API ScriptByteValue(const ScriptByteValue& b);
		SCRIPT_API ScriptByteValue(ScriptByteValue&& b) noexcept;
		SCRIPT_API ScriptByteValue(unsigned char val = {});

		SCRIPT_API virtual size_t getSize() const override;
		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() override;
		SCRIPT_API virtual std::unique_ptr<ScriptValue> clone() const override;
		SCRIPT_API virtual ScriptBoolValue boolean() const override;
		SCRIPT_API virtual unsigned char getValue() const;
		SCRIPT_API virtual void setValue(unsigned char val);
		SCRIPT_API virtual void assign(const ScriptValue& val) override;

	protected:
		unsigned char val = 0;
	};

	class ScriptIntValue : public ScriptValue
	{
	public:
		SCRIPT_API ScriptIntValue(const ScriptIntValue& b);
		SCRIPT_API ScriptIntValue(ScriptIntValue&& b) noexcept;
		SCRIPT_API ScriptIntValue(int val = {});

		SCRIPT_API virtual size_t getSize() const override;
		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() override;
		SCRIPT_API virtual std::unique_ptr<ScriptValue> clone() const override;
		SCRIPT_API virtual ScriptBoolValue boolean() const override;
		SCRIPT_API virtual int getValue() const;
		SCRIPT_API virtual void setValue(int val);
		SCRIPT_API virtual void assign(const ScriptValue& val) override;

	protected:
		int val = 0;
	};

	class ScriptUnsignedIntValue : public ScriptValue
	{
	public:
		SCRIPT_API ScriptUnsignedIntValue(const ScriptUnsignedIntValue& b);
		SCRIPT_API ScriptUnsignedIntValue(ScriptUnsignedIntValue&& b) noexcept;
		SCRIPT_API ScriptUnsignedIntValue(unsigned int val = {});

		SCRIPT_API virtual size_t getSize() const override;
		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() override;
		SCRIPT_API virtual std::unique_ptr<ScriptValue> clone() const override;
		SCRIPT_API virtual ScriptBoolValue boolean() const override;
		SCRIPT_API virtual unsigned int getValue() const;
		SCRIPT_API virtual void setValue(unsigned int val);
		SCRIPT_API virtual void assign(const ScriptValue& val) override;

	protected:
		unsigned int val = 0;
	};

	class ScriptFloatValue : public ScriptValue
	{
	public:
		SCRIPT_API ScriptFloatValue(const ScriptFloatValue& b);
		SCRIPT_API ScriptFloatValue(ScriptFloatValue&& b) noexcept;
		SCRIPT_API ScriptFloatValue(float val = {});

		SCRIPT_API virtual size_t getSize() const override;
		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() override;
		SCRIPT_API virtual std::unique_ptr<ScriptValue> clone() const override;
		SCRIPT_API virtual ScriptBoolValue boolean() const override;
		SCRIPT_API virtual float getValue() const;
		SCRIPT_API virtual void setValue(float val);
		SCRIPT_API virtual void assign(const ScriptValue& val) override;

	protected:
		float val = 0;
	};

	class ScriptDoubleValue : public ScriptValue
	{
	public:
		SCRIPT_API ScriptDoubleValue(const ScriptDoubleValue& b);
		SCRIPT_API ScriptDoubleValue(ScriptDoubleValue&& b) noexcept;
		SCRIPT_API ScriptDoubleValue(double val = {});

		SCRIPT_API virtual size_t getSize() const override;
		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() override;
		SCRIPT_API virtual std::unique_ptr<ScriptValue> clone() const override;
		SCRIPT_API virtual ScriptBoolValue boolean() const override;
		SCRIPT_API virtual double getValue() const;
		SCRIPT_API virtual void setValue(double val);
		SCRIPT_API virtual void assign(const ScriptValue& val) override;

	protected:
		double val = 0;
	};

	class ScriptStringValue : public ScriptValue
	{
	public:
		SCRIPT_API ScriptStringValue(const ScriptStringValue& b);
		SCRIPT_API ScriptStringValue(ScriptStringValue&& b) noexcept;
		SCRIPT_API ScriptStringValue(const std::string val = {});

		SCRIPT_API virtual size_t getSize() const override;
		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() override;
		SCRIPT_API virtual std::unique_ptr<ScriptValue> clone() const override;
		SCRIPT_API virtual ScriptBoolValue boolean() const override;
		SCRIPT_API virtual const std::string& getValue() const;
		SCRIPT_API virtual void setValue(const std::string& val);
		SCRIPT_API virtual void assign(const ScriptValue& val) override;

	protected:
		std::string val;
	};

	class ScriptClassValue : public ScriptValue
	{
	public:
		SCRIPT_API ScriptClassValue(const ScriptClassValue& b);
		SCRIPT_API ScriptClassValue(ScriptClassValue&& b) noexcept;
		SCRIPT_API ScriptClassValue(std::unique_ptr<ScriptClassInstance>&& val);
		SCRIPT_API ScriptClassValue(std::unique_ptr<ScriptClassInstance>&& val, ScriptClass& sclass);

		SCRIPT_API virtual size_t getSize() const override;
		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() override;
		SCRIPT_API virtual std::unique_ptr<ScriptValue> clone() const override;
		SCRIPT_API virtual ScriptBoolValue boolean() const override;
		SCRIPT_API virtual std::unique_ptr<ScriptClassInstance>& getValue();
		SCRIPT_API virtual void setValue(std::unique_ptr<ScriptClassInstance>&& val);
		SCRIPT_API virtual void assign(const ScriptValue& val) override;

	protected:
		const std::shared_ptr<ScriptType> type;
		std::unique_ptr<ScriptClassInstance> val;
	};

	class ScriptArrayValue : public ScriptValue
	{
	public:
		SCRIPT_API ScriptArrayValue() = default;
		SCRIPT_API ScriptArrayValue(const ScriptArrayValue& b);
		SCRIPT_API ScriptArrayValue(ScriptArrayValue&& b) noexcept;
		SCRIPT_API ScriptArrayValue(std::shared_ptr<ScriptType> elementType, const std::vector<std::unique_ptr<ScriptValue>>& val = {});
		SCRIPT_API ScriptArrayValue(std::shared_ptr<ScriptType> elementType, std::vector<std::unique_ptr<ScriptValue>>&& val = {});
		SCRIPT_API ScriptArrayValue(const std::vector<std::unique_ptr<ScriptValue>>& val);
		SCRIPT_API ScriptArrayValue(std::vector<std::unique_ptr<ScriptValue>>&& val);

		SCRIPT_API virtual size_t getSize() const override;
		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() override;
		SCRIPT_API virtual std::unique_ptr<ScriptValue> clone() const override;
		SCRIPT_API virtual ScriptBoolValue boolean() const override;
		SCRIPT_API virtual const std::vector<std::unique_ptr<ScriptValue>>& getValue() const;
		SCRIPT_API virtual std::vector<std::unique_ptr<ScriptValue>>& getValue();
		SCRIPT_API virtual void setValue(const std::vector<std::unique_ptr<ScriptValue>>& val);
		SCRIPT_API virtual void setValue(std::vector<std::unique_ptr<ScriptValue>>&& val);
		SCRIPT_API virtual void assign(const ScriptValue& val) override;

		SCRIPT_API virtual std::unique_ptr<ScriptValue>& at(size_t idx);

	protected:
		std::shared_ptr<ScriptArrayType> type;
		std::vector<std::unique_ptr<ScriptValue>> val;
	};

	class ScriptReferenceValue : public ScriptValue
	{
	public:
		SCRIPT_API ScriptReferenceValue(const ScriptReferenceValue& b);
		SCRIPT_API ScriptReferenceValue(ScriptReferenceValue&& b) noexcept;
		SCRIPT_API ScriptReferenceValue(std::shared_ptr<ScriptReferenceType> type = nullptr, ScriptValue* val = nullptr);
		SCRIPT_API ScriptReferenceValue(ScriptValue* v);

		SCRIPT_API static std::unique_ptr<ScriptReferenceValue> create(std::unique_ptr<ScriptValue>& v);
		SCRIPT_API static std::unique_ptr<ScriptReferenceValue> create(ScriptValue* v);

		SCRIPT_API virtual ScriptValue* data() override { return this->val; };
		SCRIPT_API virtual const ScriptValue* data() const override { return this->val; };

		SCRIPT_API virtual size_t getSize() const override;
		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() override;
		SCRIPT_API virtual std::unique_ptr<ScriptValue> clone() const override;
		SCRIPT_API virtual ScriptBoolValue boolean() const override;
		SCRIPT_API virtual ScriptValue* getValue() const;
		SCRIPT_API virtual void assign(const ScriptValue& val) override;
		SCRIPT_API void retarget(ScriptValue* val);
		SCRIPT_API virtual void setValue(ScriptValue* val);
		SCRIPT_API void put(ScriptValue* val) const;

		template<typename T>
		T resolve() const
		{
			return static_cast<T>(this->getValue());
		}

	protected:
		std::shared_ptr<ScriptType> type;
		ScriptValue *val = nullptr;
	};

	class ScriptNullValue : public ScriptValue
	{
	public:
		SCRIPT_API size_t getSize() const override;
		SCRIPT_API virtual const std::shared_ptr<ScriptType> getType() override;
		SCRIPT_API virtual std::unique_ptr<ScriptValue> clone() const override;
		SCRIPT_API virtual void assign(const ScriptValue&) override;
		SCRIPT_API virtual ScriptBoolValue boolean() const override;
	};
}

#endif