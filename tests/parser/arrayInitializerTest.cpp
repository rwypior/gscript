#include "common.h"
#include "gscript/parser/pArrayInitializer.hpp"
#include "gscript/parser/pLiteral.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::ArrayInitializer::Simple")
{
	std::string txt = "{1, 2, 3,4,5, 6}";

	gscript::ParserArrayInitializer pArrInitializer;
	auto result = pArrInitializer.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArrInitializer.arglist.parameters.size() == 6);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArrInitializer.arglist.parameters.at(0)->components.front())->value == "1");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArrInitializer.arglist.parameters.at(1)->components.front())->value == "2");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArrInitializer.arglist.parameters.at(2)->components.front())->value == "3");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArrInitializer.arglist.parameters.at(3)->components.front())->value == "4");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArrInitializer.arglist.parameters.at(4)->components.front())->value == "5");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArrInitializer.arglist.parameters.at(5)->components.front())->value == "6");
}

TEST_CASE("Parser::ArrayInitializer::Empty")
{
	std::string txt = "{}";

	gscript::ParserArrayInitializer pArrInitializer;
	auto result = pArrInitializer.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArrInitializer.arglist.parameters.size() == 0);
}

TEST_CASE("Parser::ArrayInitializer::FailureNoEnclosure")
{
	std::string txt = "{";

	gscript::ParserArrayInitializer pArrInitializer;
	auto result = pArrInitializer.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::ArrayInitializer::FailureEmptyString")
{
	std::string txt = "";

	gscript::ParserArrayInitializer pArrInitializer;
	auto result = pArrInitializer.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::ArrayInitializer::CommentLineBeforeBegin")
{
	std::string txt = 
		"// This is a comment\n"
		"{1, 2, 3}";

	gscript::ParserArrayInitializer pArrInitializer;
	auto result = pArrInitializer.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArrInitializer.arglist.parameters.size() == 3);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArrInitializer.arglist.parameters.at(0)->components.front())->value == "1");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArrInitializer.arglist.parameters.at(1)->components.front())->value == "2");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArrInitializer.arglist.parameters.at(2)->components.front())->value == "3");
}

TEST_CASE("Parser::ArrayInitializer::CommentBlockBetweenArgs")
{
	std::string txt = 
		"{1, /* This is a comment */ 2, 3}";

	gscript::ParserArrayInitializer pArrInitializer;
	auto result = pArrInitializer.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArrInitializer.arglist.parameters.size() == 3);
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArrInitializer.arglist.parameters.at(0)->components.front())->value == "1");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArrInitializer.arglist.parameters.at(1)->components.front())->value == "2");
	REQUIRE(std::static_pointer_cast<gscript::ParserLiteral>(pArrInitializer.arglist.parameters.at(2)->components.front())->value == "3");
}