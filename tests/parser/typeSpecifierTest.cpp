#include "common.h"
#include "gscript/parser/pTypeSpecifier.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::TypeSpecifier::Simple")
{
	std::string txt = "int";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(!pTypeSpecifier.isArray);
	REQUIRE(!pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}

TEST_CASE("Parser::TypeSpecifier::Reference")
{
	std::string txt = "int&";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(!pTypeSpecifier.isArray);
	REQUIRE(pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}

TEST_CASE("Parser::TypeSpecifier::Array")
{
	std::string txt = "int[]";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pTypeSpecifier.isArray);
	REQUIRE(!pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}

TEST_CASE("Parser::TypeSpecifier::ArrayReference")
{
	std::string txt = "int[]&";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pTypeSpecifier.isArray);
	REQUIRE(pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}

TEST_CASE("Parser::TypeSpecifier::FailureEmptyString")
{
	std::string txt = "";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("Parser::TypeSpecifier::FailureArrayNoName")
{
	std::string txt = "[]";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("Parser::TypeSpecifier::FailureReferenceNoName")
{
	std::string txt = "&";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("Parser::TypeSpecifier::FailureArrayReferenceNoName")
{
	std::string txt = "[]&";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected name");
}

TEST_CASE("Parser::TypeSpecifier::CommentLineBefore")
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

TEST_CASE("Parser::TypeSpecifier::CommentBlockBeforeReference")
{
	std::string txt = "int /* This is a comment */ &";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(!pTypeSpecifier.isArray);
	REQUIRE(pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}

TEST_CASE("Parser::TypeSpecifier::CommentBlockAfterReference")
{
	std::string txt = "int& /* This is a comment */";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(!pTypeSpecifier.isArray);
	REQUIRE(pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}

TEST_CASE("Parser::TypeSpecifier::CommentBlockBeforeArray")
{
	std::string txt = "int /* This is a comment */ []";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pTypeSpecifier.isArray);
	REQUIRE(!pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}

TEST_CASE("Parser::TypeSpecifier::CommentBlockAfterArray")
{
	std::string txt = "int [] /* This is a comment */";

	gscript::ParserTypeSpecifier pTypeSpecifier;
	auto result = pTypeSpecifier.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pTypeSpecifier.isArray);
	REQUIRE(!pTypeSpecifier.isReference);
	REQUIRE(pTypeSpecifier.name == "int");
}