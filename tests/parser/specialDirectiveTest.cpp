#include "common.h"
#include "gscript/parser/pSpecialDirective.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserSpecialDirective")
{
	std::string txt = "@test";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.end());
}

TEST_CASE("ParserSpecialWithLeadingSpaces")
{
	std::string txt = "     @test";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	auto dupsko = result.result.end - txt.begin();

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 5);
	REQUIRE(result.result.end == txt.begin() + 5 + 5);
}

TEST_CASE("ParserSpecialWithTrailingSpaces")
{
	std::string txt = "@test     ";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin());
	REQUIRE(result.result.end == txt.begin() + 5);
}

TEST_CASE("ParserSpecialWithTrailingLeadingSpaces")
{
	std::string txt = "     @test     ";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(result.result.begin == txt.begin() + 5);
	REQUIRE(result.result.end == txt.begin() + 5 + 5);
}

TEST_CASE("ParserSpecialDirectiveFailureNoChar")
{
	std::string txt = "test";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserSpecialDirectiveFailureWrongChar")
{
	std::string txt = "#test";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserSpecialDirectiveFailureWrongDirective")
{
	std::string txt = "#something";

	gscript::ParserSpecialDirective pSpecDir("test");
	auto result = pSpecDir.parse(txt);

	REQUIRE(!result.isOk());
}