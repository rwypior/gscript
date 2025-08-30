#include "common.h"
#include "gscript/parser/pVar.hpp"

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
	// TODO - array acessor is not really being used here - find out why, and fix or delete if necessary

	std::string txt = "myvar[42]";

	gscript::ParserVar pVar;
	auto result = pVar.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pVar.name == "myvar");
	// REQUIRE(pVar.arrayAccessor->staticIndex == 42);
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
}