#include "common.h"
#include "gscript/parser/pOperator.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserOperatorSimple")
{
	std::string txt = "+";

	gscript::ParserOperator pOper;
	auto result = pOper.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pOper.getChar() == "+");
}

TEST_CASE("ParserOperatorIncrement")
{
	std::string txt = "++";

	gscript::ParserOperator pOper;
	auto result = pOper.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pOper.getChar() == "++");
}

TEST_CASE("ParserOperatorTriplePlus")
{
	std::string txt = "+++";

	gscript::ParserOperator pOper;
	auto result = pOper.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pOper.getChar() == "++");
}

TEST_CASE("ParserOperatorMultipleOperators")
{
	// Things like this will fail in statement tests

	std::string txt = "+ - /";

	gscript::ParserOperator pOper;
	auto result = pOper.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pOper.getChar() == "+");
}

TEST_CASE("ParserOperatorFailureNoOperator")
{
	std::string txt = "something";

	gscript::ParserOperator pOper;
	auto result = pOper.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected operator, got \"something\"");
}

TEST_CASE("ParserOperatorFailureEmptyString")
{
	std::string txt = "";

	gscript::ParserOperator pOper;
	auto result = pOper.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected operator, got \"\"");
}