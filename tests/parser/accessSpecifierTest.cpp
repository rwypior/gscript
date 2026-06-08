#include "common.h"
#include "gscript/parser/pAccessSpecifier.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::AccessSpecifier::Public")
{
	std::string txt = "public";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() == gscript::Modifier::AccessPublic);
}

TEST_CASE("Parser::AccessSpecifier::Private")
{
	std::string txt = "private";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() == gscript::Modifier::AccessPrivate);
}

TEST_CASE("Parser::AccessSpecifier::Protected")
{
	std::string txt = "protected";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() == gscript::Modifier::AccessProtected);
}

TEST_CASE("Parser::AccessSpecifier::Const")
{
	std::string txt = "const";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() & (gscript::Modifier::AccessPublic | gscript::Modifier::Const));
}

TEST_CASE("Parser::AccessSpecifier::Static")
{
	std::string txt = "static";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() == (gscript::Modifier::AccessPublic | gscript::Modifier::Static));
}

TEST_CASE("Parser::AccessSpecifier::Virtual")
{
	std::string txt = "virtual";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() == (gscript::Modifier::AccessPublic | gscript::Modifier::Virtual));
}

TEST_CASE("Parser::AccessSpecifier::Unknown")
{
	std::string txt = "something";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::AccessSpecifier::Mixed")
{
	std::string txt = "public static";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() & (gscript::Modifier::AccessPublic | gscript::Modifier::Static));
}

TEST_CASE("Parser::AccessSpecifier::Repeated")
{
	std::string txt = "public public";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() & Bitfield(gscript::Modifier::AccessPublic));
}

TEST_CASE("Parser::AccessSpecifier::Empty")
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

TEST_CASE("Parser::AccessSpecifier::LineCommentBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"public static";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() & (gscript::Modifier::AccessPublic | gscript::Modifier::Static));
}

TEST_CASE("Parser::AccessSpecifier::LineCommentBetween")
{
	std::string txt = 
		"public\n"
		"// This is a comment\n"
		"static";

	gscript::ParserAccessSpecifier pAcc;
	auto result = pAcc.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pAcc.getModifier() & (gscript::Modifier::AccessPublic | gscript::Modifier::Static));
}