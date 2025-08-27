#include "common.h"
#include "gscript/parser/pArglist.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserArglistSimple")
{
	std::string txt = "(int a, string something, float s0m3numb3r, someclass cls)";

	gscript::ParserArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArglist.parameters.size() == 4);
	REQUIRE(pArglist.parameters.at(0).type == "int");
	REQUIRE(pArglist.parameters.at(0).name == "a");
	REQUIRE(pArglist.parameters.at(1).type == "string");
	REQUIRE(pArglist.parameters.at(1).name == "something");
	REQUIRE(pArglist.parameters.at(2).type == "float");
	REQUIRE(pArglist.parameters.at(2).name == "s0m3numb3r");
	REQUIRE(pArglist.parameters.at(3).type == "someclass");
	REQUIRE(pArglist.parameters.at(3).name == "cls");
}

TEST_CASE("ParserArglistEmpty")
{
	std::string txt = "()";

	gscript::ParserArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArglist.parameters.size() == 0);
}

TEST_CASE("ParserArglistEmptyString")
{
	std::string txt = "";

	gscript::ParserArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserArglistNoArgName")
{
	// NOTE - maybe a parameter without a name should be allowed?

	std::string txt = "(int x, int)";

	gscript::ParserArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserArglistInvalidArgname")
{
	std::string txt = "(int some:thing)";

	gscript::ParserArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(!result.isOk());
}