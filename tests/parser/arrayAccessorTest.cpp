#include "common.h"
#include "gscript/parser/pArrayAccessor.hpp"
#include "gscript/parser/pVar.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserArrayAccessorLiteralRequired")
{
	std::string txt = "[0]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Literal);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pArrAccessor.gotValue);
	REQUIRE(pArrAccessor.staticIndex == 0);
}

TEST_CASE("ParserArrayAccessorLiteralRequiredFailed")
{
	std::string txt = "[]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Literal);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserArrayAccessorLiteralFailed")
{
	std::string txt = "[somevar]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Literal);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserArrayAccessorStatement")
{
	std::string txt = "[somevar]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Statement);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(std::static_pointer_cast<gscript::ParserVar>(pArrAccessor.statement.components.at(0))->name == "somevar");
}

TEST_CASE("ParserArrayAccessorStatementRequiredFailed")
{
	std::string txt = "[]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Statement);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserArrayAccessorOptional")
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

TEST_CASE("ParserArrayAccessorNoIndex")
{
	std::string txt1 = "[]";
	std::string txt2 = "[123]";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Literal);

	auto result1 = pArrAccessor.parse(txt1);
	REQUIRE(result1.isOk());

	auto result2 = pArrAccessor.parse(txt2);
	REQUIRE(!result2.isOk());
}

TEST_CASE("ParserArrayAccessorFailureNoEnclosure")
{
	std::string txt = "[";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Literal);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserArrayAccessorFailureEmptyString")
{
	std::string txt = "";

	gscript::ParserArrayAccessor pArrAccessor(gscript::ParserArrayAccessor::IndexType::Required | gscript::ParserArrayAccessor::IndexType::Literal);
	auto result = pArrAccessor.parse(txt);

	REQUIRE(!result.isOk());
}