#include "common.h"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/literal.hpp"
#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/varDeclaration.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/operator.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

namespace
{
	class ExternIntValue : public gscript::ScriptExternValue
	{
	public:
		ExternIntValue(int x, int y)
			: x(x)
			, y(y)
		{
		}

		size_t getSize() const override
		{
			return sizeof(x) + sizeof(y);
		}

		virtual const std::shared_ptr<gscript::ScriptType> getType() override
		{
			return nullptr; // Not needed for this simple case
		}

		virtual std::unique_ptr<ScriptValue> clone() const override
		{
			return std::make_unique<ExternIntValue>(x, y);
		}

		virtual void assign(const ScriptValue& b) override
		{
			if (auto bb = dynamic_cast<const ExternIntValue*>(&b))
			{
				x = bb->x;
				y = bb->y;
			}
		}

		virtual gscript::ScriptBoolValue boolean() const override
		{
			return x || y;
		}

	public:
		int x = 0;
		int y = 0;
	};
}

TEST_CASE_METHOD(GscriptTest, "RuntimeExternValueBasicTest")
{
	auto& sv = globalNamespace.registerVariable("myExtern1", gscript::ScriptType::create(gscript::ValueType::Extern, globalNamespace), std::make_unique<ExternIntValue>(42, 1337));

	auto varread = std::make_unique<gscript::ScriptVarRead>(globalNamespace, "myExtern1");

	auto stmtvec = std::vector<std::unique_ptr<gscript::ScriptCallable>>();
	stmtvec.push_back(std::move(varread));

	auto stmt1 = std::make_unique<gscript::ScriptStatement>(std::move(stmtvec));
	stmt1->setup(globalNamespace);

	auto result = stmt1->run(globalNamespace);

	REQUIRE(result->as<ExternIntValue>().x == 42);
	REQUIRE(result->as<ExternIntValue>().y == 1337);
}