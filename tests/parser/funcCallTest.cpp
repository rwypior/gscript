#include "common.h"
#include "gscript/parser/pFuncCall.hpp"
#include "gscript/parser/pLiteral.hpp"
#include "gscript/parser/pStatement.hpp"
#include "gscript/parser/pVar.hpp"
#include "gscript/parser/pOperator.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserFuncCallSimple")
{
	std::string txt = "some_function();";

	gscript::ParserFuncCall pFunc;
	auto result = pFunc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pFunc.name == "some_function");
}

TEST_CASE("ParserFuncCallWithParams")
{
	std::string txt = "some_function(1, 2, 3);";

	gscript::ParserFuncCall pFunc;
	auto result = pFunc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pFunc.name == "some_function");
	REQUIRE(pFunc.arglist.parameters.size() == 3);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pFunc.arglist.parameters.at(0)->components.at(0))->value == "1");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pFunc.arglist.parameters.at(1)->components.at(0))->value == "2");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pFunc.arglist.parameters.at(2)->components.at(0))->value == "3");
}

TEST_CASE("ParserFuncCallFailureNoArglist")
{
	std::string txt = "some_function;";

	gscript::ParserFuncCall pFunc;
	auto result = pFunc.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserFuncCallMethod")
{
	// Method calling is done through statements - first an object is needed, followed by
	// member accessor, and finally function call

	{
		std::string txt = "object . some_function();";

		gscript::ParserStatement pFunc;
		auto result = pFunc.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pFunc.components.size() == 3);
		REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pFunc.components.at(0))->name == "object");
		REQUIRE(std::dynamic_pointer_cast<gscript::ParserOperatorMemberAccessor>(pFunc.components.at(1)));
		REQUIRE(std::static_pointer_cast<gscript::ParserFuncCall>(pFunc.components.at(2))->name == "some_function");
	}

	{
		std::string txt = "object. some_function();";

		gscript::ParserStatement pFunc;
		auto result = pFunc.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pFunc.components.size() == 3);
		REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pFunc.components.at(0))->name == "object");
		REQUIRE(std::dynamic_pointer_cast<gscript::ParserOperatorMemberAccessor>(pFunc.components.at(1)));
		REQUIRE(std::static_pointer_cast<gscript::ParserFuncCall>(pFunc.components.at(2))->name == "some_function");
	}

	{
		std::string txt = "object .some_function();";

		gscript::ParserStatement pFunc;
		auto result = pFunc.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pFunc.components.size() == 3);
		REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pFunc.components.at(0))->name == "object");
		REQUIRE(std::dynamic_pointer_cast<gscript::ParserOperatorMemberAccessor>(pFunc.components.at(1)));
		REQUIRE(std::static_pointer_cast<gscript::ParserFuncCall>(pFunc.components.at(2))->name == "some_function");
	}

	{
		std::string txt = "object.some_function();";

		gscript::ParserStatement pFunc;
		auto result = pFunc.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pFunc.components.size() == 3);
		REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pFunc.components.at(0))->name == "object");
		REQUIRE(std::dynamic_pointer_cast<gscript::ParserOperatorMemberAccessor>(pFunc.components.at(1)));
		REQUIRE(std::static_pointer_cast<gscript::ParserFuncCall>(pFunc.components.at(2))->name == "some_function");
	}
}