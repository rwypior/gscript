#include "common.h"
#include "gscript/parser/pAccessSpecifier.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserAccessSpecifierPublic")
{
	std::string txt = "public";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() == gscript::Modifier::AccessPublic);
}

TEST_CASE("ParserAccessSpecifierPrivate")
{
	std::string txt = "private";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() == gscript::Modifier::AccessPrivate);
}

TEST_CASE("ParserAccessSpecifierProtected")
{
	std::string txt = "protected";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() == gscript::Modifier::AccessProtected);
}

TEST_CASE("ParserAccessSpecifierConst")
{
	std::string txt = "const";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() & (gscript::Modifier::AccessPublic | gscript::Modifier::Const));
}

TEST_CASE("ParserAccessSpecifierStatic")
{
	std::string txt = "static";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() == (gscript::Modifier::AccessPublic | gscript::Modifier::Static));
}

TEST_CASE("ParserAccessSpecifierVirtual")
{
	std::string txt = "virtual";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() == (gscript::Modifier::AccessPublic | gscript::Modifier::Virtual));
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
	REQUIRE(pAcc.getModifier() & (gscript::Modifier::AccessPublic | gscript::Modifier::Static));
}

TEST_CASE("ParserAccessSpecifierRepeated")
{
	std::string txt = "public public";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() & Bitfield(gscript::Modifier::AccessPublic));
}

TEST_CASE("ParserAccessSpecifierEmpty")
{
	// Whatever is set in the constructor is the default

	{
		std::string txt = "";

		gscript::ParserAccessSpecifier pAcc;
		auto result = pAcc.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pAcc.getModifier() == gscript::Modifier::None);
	}

	{
		std::string txt = "			";

		gscript::ParserAccessSpecifier pAcc(gscript::Modifier::AccessPublic);
		auto result = pAcc.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pAcc.getModifier() == gscript::Modifier::AccessPublic);
	}
}