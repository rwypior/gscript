#include "common.h"
#include "gscript/parser/pOperator.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Operator::Simple")
{
	std::string txt = "+";

	gscript::ParserOperator pOper;
	auto result = pOper.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pOper.getChar() == "+");
}

TEST_CASE("Parser::Operator::Increment")
{
	std::string txt = "++";

	gscript::ParserOperator pOper;
	auto result = pOper.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pOper.getChar() == "++");
}

TEST_CASE("Parser::Operator::TriplePlus")
{
	std::string txt = "+++";

	gscript::ParserOperator pOper;
	auto result = pOper.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pOper.getChar() == "++");
}

TEST_CASE("Parser::Operator::MultipleOperators")
{
	// Things like this will fail in statement tests

	std::string txt = "+ - /";

	gscript::ParserOperator pOper;
	auto result = pOper.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pOper.getChar() == "+");
}

TEST_CASE("Parser::Operator::FailureNoOperator")
{
	std::string txt = "something";

	gscript::ParserOperator pOper;
	auto result = pOper.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected operator, got \"something\"");
}

TEST_CASE("Parser::Operator::FailureEmptyString")
{
	std::string txt = "";

	gscript::ParserOperator pOper;
	auto result = pOper.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected operator, got \"\"");
}