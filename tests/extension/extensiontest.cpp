#include "common.h"

#include "gscript/script.hpp"
#include "gscript/extension/scriptExtension.hpp"
#include "gscript/runtime/function.hpp"
#include "gscript/runtime/class.hpp"
#include "gscript/runtime/funcCall.hpp"
#include "gscript/runtime/literal.hpp"

#include <catch2/catch_all.hpp>

class SampleExtension : public gscript::ScriptExtension
{
public:
	class FuncMultiply : public gscript::ScriptFunction
	{
	public:
		FuncMultiply(ScriptScope& scope, const std::string& name)
			: ScriptFunction(
				scope,
				name,
				gscript::ScriptType::create(gscript::ValueType::Int, this->scope),
				gscript::PARAMS_T({
					gscript::FunctionParameter(gscript::ScriptType::create(gscript::ValueType::Int, scope)),
					gscript::FunctionParameter(gscript::ScriptType::create(gscript::ValueType::Int, scope))
				})
			)
		{
		}

		virtual std::unique_ptr<gscript::ScriptValue> run(const gscript::CALLABLE_PARAMS_T& c = {}) override
		{
			this->validateParams(c);

			auto& a = static_cast<const gscript::ScriptIntValue&>(*c[0]);
			auto& b = static_cast<const gscript::ScriptIntValue&>(*c[1]);

			auto result = std::make_unique<gscript::ScriptIntValue>(a.getValue() * b.getValue());

			return result;
		}
	};

	void run(gscript::Script& script)
	{
		script.getMainScope()->registerFunction(std::make_unique<FuncMultiply>(*script.getMainScope(), "func_multiply"));
	}

	std::string getName() const override
	{
		return "sample";
	}
};

TEST_CASE_METHOD(GscriptTest, "ExtensionSampleExtension")
{
	auto ext = new SampleExtension();
	script.extend(ext);

	ext->load(script);

	auto fnc = globalNamespace.findFunction("func_multiply", {
		gscript::FunctionParameter(gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace)),
		gscript::FunctionParameter(gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace))
	});

	auto valvec = std::vector<std::unique_ptr<gscript::ScriptValue>>();
	valvec.push_back(std::make_unique<gscript::ScriptIntValue>(42));
	valvec.push_back(std::make_unique<gscript::ScriptIntValue>(1337));

	auto retval = fnc->run(valvec);

	REQUIRE(retval->as<gscript::ScriptIntValue>().getValue() == (42 * 1337));

	//gscript::ScriptFuncCall fc(globalNamespace,);
}