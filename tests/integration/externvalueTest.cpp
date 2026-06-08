#include "common.h"
#include "gscript/script.hpp"
#include "gscript/extension/ConsoleExtension.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/varDeclaration.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/operator.hpp"
#include "gscript/runtime/function.hpp"
#include "gscript/StringUtils.hpp"
#include "gscript/util.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

/// This test tests the storage of host-defined data inside script's class
/// field declarations.
/// In order to achieve that, a separate type, value and class must be created.
/// 1. Type - is used to assure type correctness and other operations
/// 2. Value - is used to store the actual data (Payload class in this example)
/// 3. Class - is used to represent the data in the script code

namespace
{
	// Host-side data representation

	class Payload
	{
	public:
		int x = 0;
		int y = 0;
	};

	class PayloadRepository
	{
	public:
		std::vector<std::shared_ptr<Payload>> payloads;
	};

	// Script-side data representation

	class PayloadValue : public gscript::ScriptExternValue
	{
	public:
		class Type : public gscript::ScriptExternType
		{
		public:
			virtual std::unique_ptr<ScriptType> clone() const override
			{
				return std::unique_ptr<Type>();
			}

			virtual bool matches(const gscript::ScriptVariable& var) override
			{
				if (var.getValue()->getType()->getTypeDescriptor() != this->type)
					return false;

				return !!std::dynamic_pointer_cast<const Type>(var.getValue()->getType());
			}

			virtual bool operator==(const ScriptType& b) const override
			{
				if (b.getTypeDescriptor() != this->type)
					return false;

				return !!dynamic_cast<const Type*>(&b);
			}
		};

	public:
		explicit PayloadValue(std::shared_ptr<Payload> payload)
			: payload(payload)
			, type(std::make_shared<Type>())
		{
		}

		size_t getSize() const override
		{
			return sizeof(payload);
		}

		virtual const std::shared_ptr<gscript::ScriptType> getType() override
		{
			return this->type;
		}

		virtual std::unique_ptr<ScriptValue> clone() const override
		{
			return std::make_unique<PayloadValue>(payload);
		}

		virtual void assign(const ScriptValue& b) override
		{
			if (auto bb = dynamic_cast<const PayloadValue*>(&b))
			{
				payload = bb->payload;
			}
		}

		virtual gscript::ScriptBoolValue boolean() const override
		{
			return !!payload;
		}

	public:
		std::shared_ptr<Payload> payload;
		std::shared_ptr<Type> type;
	};

	// Functions

	class PayloadClass : public gscript::ScriptClass
	{
	public:
		class PayloadConstructor : public gscript::ScriptMethod
		{
		public:
			PayloadConstructor(gscript::ScriptScope& scope)
				: gscript::ScriptMethod(
					scope,
					"Payload",
					gscript::ScriptType::create(gscript::ValueType::Void, scope),
					gscript::PARAMS_T({
						gscript::FunctionParameter(std::make_unique<PayloadValue::Type>())
					}),
					gscript::Modifier::AccessPublic
				)
			{
			}

			std::unique_ptr<gscript::ScriptValue> run(ScriptScopeBase& scope, const gscript::CALLABLE_PARAMS_T& c = {}) override
			{
				validateParams(c);
				scope.getVariable("payload")->setValue(c[0]);
				return nullptr;
			}
		};

	public:
		PayloadClass(gscript::ScriptScope& scope)
			: gscript::ScriptClass(scope, "Payload", nullptr)
		{
			this->fieldDeclarations.push_back(gscript::createFieldDeclaration<PayloadValue::Type>(*this, "payload"));
			this->registerFunction(std::make_unique<PayloadConstructor>(*this));
		}
	};

	class CreatePayloadFunction : public gscript::ScriptFunction
	{
	public:
		CreatePayloadFunction(gscript::ScriptScope& scope)
			: gscript::ScriptFunction(
				scope,
				"createpayload",
				gscript::ScriptType::create(gscript::ValueType::Void, scope),
				gscript::PARAMS_T({
					gscript::FunctionParameter(gscript::ScriptType::create(gscript::ValueType::Int, scope)),
					gscript::FunctionParameter(gscript::ScriptType::create(gscript::ValueType::Int, scope))
				})
			)
		{
		}

		std::unique_ptr<gscript::ScriptValue> run(ScriptScopeBase& scope, const gscript::CALLABLE_PARAMS_T& c = {}) override
		{
			validateParams(c);
			const int x = static_cast<const gscript::ScriptIntValue&>(*c[0]).getValue();
			const int y = static_cast<const gscript::ScriptIntValue&>(*c[1]).getValue();
			auto payload = std::make_shared<Payload>();
			payload->x = x;
			payload->y = y;
			auto payloadValue = std::make_unique<PayloadValue>(std::move(payload));
			std::vector<std::unique_ptr<gscript::ScriptValue>> constructorParams;
			constructorParams.push_back(std::move(payloadValue));
			auto payloadInstance = scope.getGlobalNamespace()->findClass("Payload")->instantiate(std::move(constructorParams));
			return std::make_unique<gscript::ScriptClassValue>(std::move(payloadInstance));
		}
	};

	class StorePayloadFunction : public gscript::ScriptFunction
	{
	public:
		StorePayloadFunction(gscript::ScriptScope& scope, PayloadRepository& payloadRepository)
			: gscript::ScriptFunction(
				scope,
				"storepayload",
				gscript::ScriptType::create(gscript::ValueType::Void, scope),
				gscript::PARAMS_T({
					gscript::FunctionParameter(
						gscript::ScriptType::create(gscript::ValueType::Class, scope, "Payload"),
						std::make_unique<gscript::ScriptClassType>(*scope.getGlobalNamespace()->findClass("Payload"))
					)
				})
			)
			, payloadRepository(payloadRepository)
		{
		}

		std::unique_ptr<gscript::ScriptValue> run(ScriptScopeBase& scope, const gscript::CALLABLE_PARAMS_T& c = {}) override
		{
			validateParams(c);
			
			auto payloadVar = static_cast<PayloadValue*>(static_cast<gscript::ScriptClassValue&>(*c[0]->data()).getValue()->getVariable("payload")->getValue().get());
			
			payloadRepository.payloads.push_back(payloadVar->payload);
			return gscript::ScriptType::null();
		}

	private:
		PayloadRepository& payloadRepository;
	};
}

TEST_CASE_METHOD(GscriptTest, "Integration::ExternValueTest::Basic")
{
	std::string txt =
		R"GSCRIPT(
#import <console>

class Main : EntryPoint
{
	int run(int argc, string[] argv)
	{
		Payload payload = createpayload(42, 1337);
		storepayload(payload);
		
		return 0;
	}
}
		)GSCRIPT";

	PayloadRepository payloadRepository;

	std::ostringstream out;

	gscript::Script script;
	script.loadDefaultExtensions();
	script.getMainScope()->registerClass(std::make_unique<PayloadClass>(*script.getMainScope()));
	script.getMainScope()->registerFunction(std::make_unique<CreatePayloadFunction>(*script.getMainScope()));
	script.getMainScope()->registerFunction(std::make_unique<StorePayloadFunction>(*script.getMainScope(), payloadRepository));
	static_cast<gscript::ConsoleExtension*>(script.findExtension("Console"))->out = &out;

	bool compileResult = script.compile(txt);
	REQUIRE(compileResult);

	int res = script.run(0, nullptr);

	REQUIRE(res == 0);
	REQUIRE(payloadRepository.payloads.size() == 1);
	REQUIRE(payloadRepository.payloads.at(0)->x == 42);
	REQUIRE(payloadRepository.payloads.at(0)->y == 1337);
}