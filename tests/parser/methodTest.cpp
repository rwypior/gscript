#include "common.h"
#include "gscript/parser/pMethod.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserMethodSimple")
{
	std::string txt =
		"void myfunc() {\n"
		"}"
		;

	gscript::ParserMethod pMethod;
	auto result = pMethod.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pMethod.accessSpecifier.modifier == gscript::Modifier::None);
}

TEST_CASE("ParserMethodProtected")
{
	std::string txt =
		"protected void myfunc() {\n"
		"}"
		;

	gscript::ParserMethod pMethod;
	auto result = pMethod.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pMethod.accessSpecifier.modifier == gscript::Modifier::AccessProtected);
}

TEST_CASE("ParserMethodStaticProtected")
{
	std::string txt =
		"static protected void myfunc() {\n"
		"}"
		;

	gscript::ParserMethod pMethod;
	auto result = pMethod.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pMethod.accessSpecifier.modifier & (gscript::Modifier::AccessProtected | gscript::Modifier::Static));
}