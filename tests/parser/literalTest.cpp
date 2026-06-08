#include "common.h"
#include "gscript/parser/pLiteral.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Literal::Int")
{
	std::string txt = "42";

	gscript::ParserLiteral pLit;
	auto result = pLit.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pLit.value == "42");
	REQUIRE(pLit.type == gscript::ValueType::Int);
}

TEST_CASE("Parser::Literal::Double")
{
	std::string txt = "13.37";

	gscript::ParserLiteral pLit;
	auto result = pLit.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pLit.value == "13.37");
	REQUIRE(pLit.type == gscript::ValueType::Double);
}

TEST_CASE("Parser::Literal::Float")
{
	{
		std::string txt = "13.37f";

		gscript::ParserLiteral pLit;
		auto result = pLit.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pLit.value == "13.37");
		REQUIRE(pLit.type == gscript::ValueType::Float);
	}

	{
		std::string txt = "13.37F";

		gscript::ParserLiteral pLit;
		auto result = pLit.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pLit.value == "13.37");
		REQUIRE(pLit.type == gscript::ValueType::Float);
	}
}

TEST_CASE("Parser::Literal::DoubleFloat")
{
	std::string txt = "13.37ff";

	gscript::ParserLiteral pLit;
	auto result = pLit.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Invalid character \"f\" in numeric literal");
}

TEST_CASE("Parser::Literal::DoubleWithSpaces")
{
	std::string txt = "13 . 37";

	gscript::ParserLiteral pLit;
	auto result = pLit.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pLit.value == "13");
}

TEST_CASE("Parser::Literal::String")
{
	std::string txt = "\"something\"";

	gscript::ParserLiteral pLit;
	auto result = pLit.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pLit.value == "something");
	REQUIRE(pLit.type == gscript::ValueType::String);
}

TEST_CASE("Parser::Literal::DecimalSeparatorNoDigits")
{
	{
		std::string txt = ".";

		gscript::ParserLiteral pLit;
		auto result = pLit.parse(txt);

		REQUIRE(!result.isOk());
		REQUIRE(result.details.message == "Decimal separator must be adjacent to digits");
	}
	
	{
		std::string txt = " .";

		gscript::ParserLiteral pLit;
		auto result = pLit.parse(txt);

		REQUIRE(!result.isOk());
		REQUIRE(result.details.message == "Decimal separator must be adjacent to digits");
	}

	{
		std::string txt = ". ";

		gscript::ParserLiteral pLit;
		auto result = pLit.parse(txt);

		REQUIRE(!result.isOk());
		REQUIRE(result.details.message == "Decimal separator must be adjacent to digits");
	}

	{
		std::string txt = "   .  ";

		gscript::ParserLiteral pLit;
		auto result = pLit.parse(txt);

		REQUIRE(!result.isOk());
		REQUIRE(result.details.message == "Decimal separator must be adjacent to digits");
	}
}