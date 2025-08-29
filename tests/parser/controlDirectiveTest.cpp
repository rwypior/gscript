#include "common.h"
#include "gscript/parser/pControlDirective.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserControlDirectiveSimple")
{
	std::string directive = "something";
	std::string txt = "#something";

	gscript::ParserControlDirective pCtrl(directive);
	auto result = pCtrl.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pCtrl.directive == directive);
}

TEST_CASE("ParserControlDirectiveFailure")
{
	std::string directive = "something";
	std::string txt = "#nothing";

	gscript::ParserControlDirective pCtrl(directive);
	auto result = pCtrl.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"something\", got \"nothing\"");
}

TEST_CASE("ParserControlDirectiveFailurePartial")
{
	std::string directive = "something";
	std::string txt = "#somethingelse";

	gscript::ParserControlDirective pCtrl(directive);
	auto result = pCtrl.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"something\"");
}

TEST_CASE("ParserControlDirectiveEmptyDirective")
{
	std::string directive = "";
	std::string txt = "#";
	std::string txt2 = "#something";

	gscript::ParserControlDirective pCtrl(directive);
	auto result = pCtrl.parse(txt);

	REQUIRE(!result.isOk());

	gscript::ParserControlDirective pCtrl2(directive);
	auto result2 = pCtrl2.parse(txt);

	REQUIRE(!result2.isOk());
}

TEST_CASE("ParserControlDirectiveEmptyString")
{
	std::string directive = "something";
	std::string txt = "";

	gscript::ParserControlDirective pCtrl(directive);
	auto result = pCtrl.parse(txt);

	REQUIRE(!result.isOk());
}