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
	REQUIRE(pMethod.accessSpecifier.modifier == gscript::MODIFIER_T::M_NONE);
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
	REQUIRE(pMethod.accessSpecifier.modifier == gscript::MODIFIER_T::M_ACCESS_PROTECTED);
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
	REQUIRE(pMethod.accessSpecifier.modifier & (gscript::MODIFIER_T::M_ACCESS_PROTECTED | gscript::MODIFIER_T::M_STATIC));
}