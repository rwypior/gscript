#include "common.h"
#include "gscript/parser/pCallArglist.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserCallarglistSimple")
{
	std::string txt = "(1, 2, \"string\")";

	gscript::ParserCallArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArglist.parameters.size() == 3);
}

TEST_CASE("ParserCallarglistEmpty")
{
	std::string txt = "()";

	gscript::ParserCallArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArglist.parameters.size() == 0);
}

TEST_CASE("ParserCallarglistEmptyString")
{
	std::string txt = "";

	gscript::ParserCallArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserCallarglistTwoWords")
{
	std::string txt = "(int x, int y)";

	gscript::ParserCallArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserCallarglistSpecialWords")
{
	std::string txt = "(int x, class y)";

	gscript::ParserCallArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserCallarglistInvalidArg")
{
	std::string txt = "(some:thing)";

	gscript::ParserCallArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserCallarglistFailureNoEnclosure")
{
	std::string txt = "(";

	gscript::ParserCallArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserCallarglistFailureEmptyString")
{
	std::string txt = "";

	gscript::ParserCallArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(!result.isOk());
}