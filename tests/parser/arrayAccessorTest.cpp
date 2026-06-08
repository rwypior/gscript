#include "common.h"
#include "gscript/parser/pArrayAccessor.hpp"
#include "gscript/parser/pVar.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::ArrayAccessor::LiteralRequired")
{
	std::string txt = "[0]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Literal);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArrAccessor.gotValue);
	REQUIRE(pArrAccessor.staticIndex == 0);
}

TEST_CASE("Parser::ArrayAccessor::LiteralRequiredFailed")
{
	std::string txt = "[]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Literal);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::ArrayAccessor::LiteralFailed")
{
	std::string txt = "[somevar]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Literal);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::ArrayAccessor::Statement")
{
	std::string txt = "[somevar]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Statement);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pArrAccessor.statement.components.at(0))->name == "somevar");
}

TEST_CASE("Parser::ArrayAccessor::StatementRequiredFailed")
{
	std::string txt = "[]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Statement);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::ArrayAccessor::Optional")
{
	std::string txt1 = "[]";
	std::string txt2 = "[something]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Optional | gscript::ParserArrayAccessor::IndexType::Statement);

	auto result1 = pArrAccessor.parse(txt1);
	REQUIRE(result1.isOk());

	auto result2 = pArrAccessor.parse(txt2);
	REQUIRE(result2.isOk());
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pArrAccessor.statement.components.at(0))->name == "something");
}

TEST_CASE("Parser::ArrayAccessor::NoIndex")
{
	std::string txt1 = "[]";
	std::string txt2 = "[123]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Literal);

	auto result1 = pArrAccessor.parse(txt1);
	REQUIRE(result1.isOk());

	auto result2 = pArrAccessor.parse(txt2);
	REQUIRE(!result2.isOk());
}

TEST_CASE("Parser::ArrayAccessor::FailureNoEnclosure")
{
	std::string txt = "[";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Literal);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::ArrayAccessor::FailureEmptyString")
{
	std::string txt = "";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Literal);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("Parser::ArrayAccessor::CommentLineBeforeBegin")
{
	std::string txt = 
		"// This is a comment\n"
		"[0]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Literal);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArrAccessor.gotValue);
	REQUIRE(pArrAccessor.staticIndex == 0);
}

TEST_CASE("Parser::ArrayAccessor::CommentLineAfterBegin")
{
	std::string txt = 
		"[\n"
		"// This is a comment\n"
		"0]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Literal);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArrAccessor.gotValue);
	REQUIRE(pArrAccessor.staticIndex == 0);
}

TEST_CASE("Parser::ArrayAccessor::CommentBlockBeforeEnd")
{
	std::string txt = 
		"[0 /* This is a comment */]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Literal);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArrAccessor.gotValue);
	REQUIRE(pArrAccessor.staticIndex == 0);
}