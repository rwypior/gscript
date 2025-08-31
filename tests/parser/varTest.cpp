#include "common.h"
#include "gscript/parser/pVar.hpp"
#include "gscript/parser/pLiteral.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserVarSimple")
{
	std::string txt = "myvar";

	gscript::ParserVar pVar;
	auto result = pVar.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pVar.name == "myvar");
}

TEST_CASE("ParserVarArray")
{
	std::string txt = "myvar[42]";

	gscript::ParserVar pVar;
	auto result = pVar.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pVar.name == "myvar");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pVar.arrayAccessor->statement.components.at(0))->value == "42");
}

TEST_CASE("ParserVarFailureEmptyString")
{
	std::string txt = "";

	gscript::ParserVar pVar;
	auto result = pVar.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("ParserVarFailureArrayNoEnclosure")
{
	std::string txt = "myvar[";

	gscript::ParserVar pVar;
	auto result = pVar.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Empty statement"); // TODO - this needs better error message
}

TEST_CASE("ParserVarFailureArrayEmpty")
{
	std::string txt = "myvar[]";

	gscript::ParserVar pVar;
	auto result = pVar.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected non-empty statement");
}