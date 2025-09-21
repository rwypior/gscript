#include "common.h"
#include "gscript/parser/pArglist.hpp"
#include "gscript/parser/pLiteral.hpp"

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

TEST_CASE("ParserArglistDefaults")
{
	std::string txt = "(int a, int b = 42, int c = 1337)";

	gscript::ParserArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArglist.parameters.size() == 3);
	REQUIRE(pArglist.parameters.at(0).type == "int");
	REQUIRE(pArglist.parameters.at(0).name == "a");
	REQUIRE(pArglist.parameters.at(0).value.components.empty());
	REQUIRE(pArglist.parameters.at(1).type == "int");
	REQUIRE(pArglist.parameters.at(1).name == "b");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArglist.parameters.at(1).value.components.at(0))->value == "42");
	REQUIRE(pArglist.parameters.at(2).type == "int");
	REQUIRE(pArglist.parameters.at(2).name == "c");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArglist.parameters.at(2).value.components.at(0))->value == "1337");
}

TEST_CASE("ParserArglistEmpty")
{
	std::string txt = "()";

	gscript::ParserArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArglist.parameters.size() == 0);
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

TEST_CASE("ParserArglistFailureNoEnclosure")
{
	std::string txt = "(";

	gscript::ParserArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserArglistFailureEmptyString")
{
	std::string txt = "";

	gscript::ParserArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserArglistCommentLineBeforeStart")
{
	std::string txt = 
		"// This is a comment\n"
		"(int x, string str)";

	gscript::ParserArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserArglistCommentBlockBetweenArgs")
{
	std::string txt =
		"(int x, /* This is a comment */ string str)";

	gscript::ParserArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserArglistCommentLineBetweenArgs")
{
	std::string txt =
		"(int x, // This is a comment - string str)\n"
		"string str)"
		;

	gscript::ParserArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(result.isOk());
}

TEST_CASE("ParserArglistCommentBlockBetweenTypeAndName")
{
	std::string txt =
		"(int /* This is a comment */ x, string str)";

	gscript::ParserArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(result.isOk());
}