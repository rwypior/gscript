#include "common.h"
#include "gscript/parser/pWhile.hpp"
#include "gscript/parser/pVar.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::While::Simple")
{
	std::string txt = "while (myvar) {}";

	gscript::ParserWhile pwhile;
	auto result = pwhile.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pwhile.arglist.parameters.at(0)->components.at(0))->name == "myvar");
}

TEST_CASE("Parser::While::WithBody")
{
	std::string txt = 
		"while (myvar)\n"
		"{\n"
		"somevar += i;\n"
		"}\n";

	gscript::ParserWhile pwhile;
	auto result = pwhile.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pwhile.arglist.parameters.at(0)->components.at(0))->name == "myvar");
	REQUIRE(pwhile.body.body.statements.size() == 1);
}

TEST_CASE("Parser::While::FailureNoBody")
{
	std::string txt = "while (myvar)";

	gscript::ParserWhile pwhile;
	auto result = pwhile.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected statement");
}

TEST_CASE("Parser::While::FailureNoArglist")
{
	std::string txt = "while";

	gscript::ParserWhile pwhile;
	auto result = pwhile.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected argument list");
}

TEST_CASE("Parser::While::FailureNoArgs")
{
	std::string txt = "while ()";

	gscript::ParserWhile pwhile;
	auto result = pwhile.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected statement");
}

TEST_CASE("Parser::While::CommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"while (myvar) {}";

	gscript::ParserWhile pwhile;
	auto result = pwhile.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pwhile.arglist.parameters.at(0)->components.at(0))->name == "myvar");
}

TEST_CASE("Parser::While::CommentBlockAfterWhile")
{
	std::string txt = "while /* This is a comment */ (myvar) {}";

	gscript::ParserWhile pwhile;
	auto result = pwhile.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pwhile.arglist.parameters.at(0)->components.at(0))->name == "myvar");
}

TEST_CASE("Parser::While::CommentBlockAfterArglist")
{
	std::string txt = "while (myvar) /* This is a comment */ {}";

	gscript::ParserWhile pwhile;
	auto result = pwhile.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pwhile.arglist.parameters.at(0)->components.at(0))->name == "myvar");
}