#include "common.h"
#include "gscript/parser/pCallArglist.hpp"
#include "gscript/parser/pLiteral.hpp"
#include "gscript/parser/pOperator.hpp"
#include "gscript/parser/pFuncCall.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserCallarglistSimple")
{
	std::string txt = "(1, 2, \"string\")";

	gscript::ParserCallArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArglist.parameters.size() == 3);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArglist.parameters.at(0)->components.at(0))->value == "1");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArglist.parameters.at(1)->components.at(0))->value == "2");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArglist.parameters.at(2)->components.at(0))->value == "string");
}

TEST_CASE("ParserCallarglistWithStatements")
{
	std::string txt = "(1, 4 + 2, \"string\", somefunc(555))";

	gscript::ParserCallArglist pArglist;
	auto result = pArglist.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArglist.parameters.size() == 4);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArglist.parameters.at(0)->components.at(0))->value == "1");
	REQUIRE(std::dynamic_pointer_cast<gscript::ParserOperatorAdd>(pArglist.parameters.at(1)->components.at(1)));
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArglist.parameters.at(2)->components.at(0))->value == "string");
	REQUIRE(std::dynamic_pointer_cast<gscript::ParserFuncCall>(pArglist.parameters.at(3)->components.at(0)));
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
	std::string txt = "(some#thing)";

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