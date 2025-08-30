#include "common.h"
#include "gscript/parser/pAccessSpecifier.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserAccessSpecifierPublic")
{
	std::string txt = "public";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() == gscript::MODIFIER_T::M_ACCESS_PUBLIC);
}

TEST_CASE("ParserAccessSpecifierPrivate")
{
	std::string txt = "private";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() == gscript::MODIFIER_T::M_ACCESS_PRIVATE);
}

TEST_CASE("ParserAccessSpecifierProtected")
{
	std::string txt = "protected";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() == gscript::MODIFIER_T::M_ACCESS_PROTECTED);
}

TEST_CASE("ParserAccessSpecifierConst")
{
	std::string txt = "const";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() & (gscript::MODIFIER_T::M_ACCESS_PUBLIC | gscript::MODIFIER_T::M_CONST));
}

TEST_CASE("ParserAccessSpecifierStatic")
{
	std::string txt = "static";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() == (gscript::MODIFIER_T::M_ACCESS_PUBLIC | gscript::MODIFIER_T::M_STATIC));
}

TEST_CASE("ParserAccessSpecifierVirtual")
{
	std::string txt = "virtual";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() == (gscript::MODIFIER_T::M_ACCESS_PUBLIC | gscript::MODIFIER_T::M_VIRTUAL));
}

TEST_CASE("ParserAccessSpecifierUnknown")
{
	std::string txt = "something";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserAccessSpecifierMixed")
{
	std::string txt = "public static";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() & (gscript::MODIFIER_T::M_ACCESS_PUBLIC | gscript::MODIFIER_T::M_STATIC));
}

TEST_CASE("ParserAccessSpecifierRepeated")
{
	std::string txt = "public public";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() & Bitfield(gscript::MODIFIER_T::M_ACCESS_PUBLIC));
}

TEST_CASE("ParserAccessSpecifierEmpty")
{
	// Whatever is set in the constructor is the default

	{
		std::string txt = "";

		gscript::ParserAccessSpecifier pAcc;
		auto result = pAcc.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pAcc.getModifier() == gscript::MODIFIER_T::M_NONE);
	}

	{
		std::string txt = "			";

		gscript::ParserAccessSpecifier pAcc(gscript::MODIFIER_T::M_ACCESS_PUBLIC);
		auto result = pAcc.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pAcc.getModifier() == gscript::MODIFIER_T::M_ACCESS_PUBLIC);
	}
}