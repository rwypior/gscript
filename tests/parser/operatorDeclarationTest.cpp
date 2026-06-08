#include "common.h"
#include "gscript/parser/pOperatorDeclaration.hpp"
#include "gscript/parser/pStatement.hpp"
#include "gscript/parser/pReturn.hpp"
#include "gscript/parser/pLiteral.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("Parser::OperatorDeclaration::Simple")
{
	for (const auto& entry : gscript::getOperatorMap())
	{
		std::string txt = "operator " + entry.first;

		gscript::ParserOperatorDeclaration pOper;
		auto result = pOper.parse(txt);

		INFO("Operator = " + txt);
		REQUIRE(result.isOk());
		REQUIRE(pOper.operatorType == entry.second);
	}
}

TEST_CASE("Parser::OperatorDeclaration::With special characters")
{
	{
		std::string txt = "operator+=(";

		gscript::ParserOperatorDeclaration pOper;
		auto result = pOper.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pOper.operatorType == gscript::OperatorType::AddTo);
	}

	{
		std::string txt = "operator+={";

		gscript::ParserOperatorDeclaration pOper;
		auto result = pOper.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pOper.operatorType == gscript::OperatorType::AddTo);
	}
}

TEST_CASE("Parser::OperatorDeclaration::With spaces")
{
	std::string txt = "    operator   +=(    ";

	gscript::ParserOperatorDeclaration pOper;
	auto result = pOper.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pOper.operatorType == gscript::OperatorType::AddTo);
}

TEST_CASE("Parser::OperatorDeclaration::With full brackets")
{
	std::string txt = "operator.++()";

	gscript::ParserOperatorDeclaration pOper;
	auto result = pOper.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pOper.operatorType == gscript::OperatorType::PostIncrement);
}