#include "common.h"
#include "gscript/defs.hpp"
#include "gscript/util.hpp"

#include <catch2/catch_all.hpp>

#include <vector>
#include <memory>

TEST_CASE_METHOD(GscriptTest, "Util::Parameter::ToStringBasic")
{
	auto params = gscript::PARAMS_T{ 
		{ gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace) }, 
		{ gscript::ScriptType::create(gscript::ValueType::Float, globalNamespace) },
		{ gscript::ScriptType::create(gscript::ValueType::String, globalNamespace) }
	};

	std::string str = gscript::parametersToString(params);

	REQUIRE(str == "(int, float, string)");
}

TEST_CASE_METHOD(GscriptTest, "Util::Parameter::ToStringEmpty")
{
	auto params = gscript::PARAMS_T{
	};

	std::string str = gscript::parametersToString(params);

	REQUIRE(str == "()");
}

TEST_CASE_METHOD(GscriptTest, "Util::Parameter::ToStringNewLines")
{
	auto params = gscript::PARAMS_T{ 
		{ gscript::ScriptType::create(gscript::ValueType::Int, globalNamespace) }, 
		{ gscript::ScriptType::create(gscript::ValueType::Float, globalNamespace) },
		{ gscript::ScriptType::create(gscript::ValueType::String, globalNamespace) }
	};

	std::string str = gscript::parametersToString(params, true);

	REQUIRE(str == "(\n\tint\n\tfloat\n\tstring\n)");
}

TEST_CASE_METHOD(GscriptTest, "Util::Parameter::ToStringNewLinesEmpty")
{
	auto params = gscript::PARAMS_T{
	};

	std::string str = gscript::parametersToString(params, true);

	REQUIRE(str == "()");
}