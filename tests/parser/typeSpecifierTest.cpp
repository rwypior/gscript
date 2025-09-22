#include "common.h"
#include "gscript/parser/pTypeSpecifier.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserTypeSpecifierSimple")
{
	std::string txt = "int";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(!pTypeSpecifier.isArray);
	REQUIRE(!pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}

TEST_CASE("ParserTypeSpecifierReference")
{
	std::string txt = "int&";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(!pTypeSpecifier.isArray);
	REQUIRE(pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}

TEST_CASE("ParserTypeSpecifierArray")
{
	std::string txt = "int[]";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pTypeSpecifier.isArray);
	REQUIRE(!pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}

TEST_CASE("ParserTypeSpecifierArrayReference")
{
	std::string txt = "int[]&";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pTypeSpecifier.isArray);
	REQUIRE(pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}

TEST_CASE("ParserTypeSpecifierFailureEmptyString")
{
	std::string txt = "";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("ParserTypeSpecifierFailureArrayNoName")
{
	std::string txt = "[]";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("ParserTypeSpecifierFailureReferenceNoName")
{
	std::string txt = "&";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("ParserTypeSpecifierFailureArrayReferenceNoName")
{
	std::string txt = "[]&";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("ParserTypeSpecifierCommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"int";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(!pTypeSpecifier.isArray);
	REQUIRE(!pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}

TEST_CASE("ParserTypeSpecifierCommentBlockBeforeReference")
{
	std::string txt = "int /* This is a comment */ &";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(!pTypeSpecifier.isArray);
	REQUIRE(pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}

TEST_CASE("ParserTypeSpecifierCommentBlockAfterReference")
{
	std::string txt = "int& /* This is a comment */";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(!pTypeSpecifier.isArray);
	REQUIRE(pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}

TEST_CASE("ParserTypeSpecifierCommentBlockBeforeArray")
{
	std::string txt = "int /* This is a comment */ []";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pTypeSpecifier.isArray);
	REQUIRE(!pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}

TEST_CASE("ParserTypeSpecifierCommentBlockAfterArray")
{
	std::string txt = "int [] /* This is a comment */";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pTypeSpecifier.isArray);
	REQUIRE(!pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}