#include "common.h"
#include "gscript/parser/pMethod.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::Method::Simple")
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

TEST_CASE("Parser::Method::Protected")
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

TEST_CASE("Parser::Method::StaticProtected")
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

TEST_CASE("Parser::Method::CommentLineBefore")
{
	std::string txt =
		"// This is a comment\n"
		"void myfunc() {\n"
		"}"
		;

	gscript::ParserMethod pMethod;
	auto result = pMethod.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pMethod.accessSpecifier.modifier == gscript::Modifier::None);
}

TEST_CASE("Parser::Method::CommentBlockAfterProtected")
{
	std::string txt =
		"protected /* This is a comment */ void myfunc() {\n"
		"}"
		;

	gscript::ParserMethod pMethod;
	auto result = pMethod.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pMethod.accessSpecifier.modifier == gscript::Modifier::AccessProtected);
}

TEST_CASE("Parser::Method::CommentBlockBetweenStaticAndProtected")
{
	std::string txt =
		"static /* This is a comment */ protected void myfunc() {\n"
		"}"
		;

	gscript::ParserMethod pMethod;
	auto result = pMethod.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pMethod.accessSpecifier.modifier & (gscript::Modifier::AccessProtected | gscript::Modifier::Static));
}

TEST_CASE("Parser::Method::Operator")
{
	std::string txt =
		"public void operator.++() {\n"
		"}"
		;

	gscript::ParserMethod pMethod;
	auto result = pMethod.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(!!(pMethod.accessSpecifier.modifier & (gscript::Modifier::AccessPublic)));
	REQUIRE(pMethod.operatorType == gscript::OperatorType::PostIncrement);
}