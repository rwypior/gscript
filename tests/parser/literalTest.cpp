#include "common.h"
#include "gscript/parser/pLiteral.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserLiteralInt")
{
	std::string txt = "42";

	gscript::ParserLiteral pLit;
	auto result = pLit.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pLit.value == "42");
	REQUIRE(pLit.type == gscript::VALUE_TYPE_T::VT_INT);
}

TEST_CASE("ParserLiteralDouble")
{
	std::string txt = "13.37";

	gscript::ParserLiteral pLit;
	auto result = pLit.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pLit.value == "13.37");
	REQUIRE(pLit.type == gscript::VALUE_TYPE_T::VT_DOUBLE);
}

TEST_CASE("ParserLiteralFloat")
{
	{
		std::string txt = "13.37f";

		gscript::ParserLiteral pLit;
		auto result = pLit.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pLit.value == "13.37");
		REQUIRE(pLit.type == gscript::VALUE_TYPE_T::VT_FLOAT);
	}

	{
		std::string txt = "13.37F";

		gscript::ParserLiteral pLit;
		auto result = pLit.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pLit.value == "13.37");
		REQUIRE(pLit.type == gscript::VALUE_TYPE_T::VT_FLOAT);
	}
}

TEST_CASE("ParserLiteralDoubleFloat")
{
	std::string txt = "13.37ff";

	gscript::ParserLiteral pLit;
	auto result = pLit.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Invalid character \"f\" in numeric literal");
}

TEST_CASE("ParserLiteralDoubleWithSpaces")
{
	std::string txt = "13 . 37";

	gscript::ParserLiteral pLit;
	auto result = pLit.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pLit.value == "13");
}

TEST_CASE("ParserLiteralString")
{
	std::string txt = "\"something\"";

	gscript::ParserLiteral pLit;
	auto result = pLit.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pLit.value == "something");
	REQUIRE(pLit.type == gscript::VALUE_TYPE_T::VT_STRING);
}

TEST_CASE("ParserLiteralDecimalSeparatorNoDigits")
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