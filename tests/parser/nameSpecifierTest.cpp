#include "common.h"
#include "gscript/parser/pNameSpecifier.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserNameSpecifierAlnum")
{
	std::string txt = "qwertyuiopasdfghjklzxcvbnm0123456789";

	gscript::ParserNameSpecifier pName;
	auto result = pName.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pName.name == txt);
}

TEST_CASE("ParserNameSpecifierEmpty")
{
	std::string txt = "";

	gscript::ParserNameSpecifier pName;
	auto result = pName.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserNameSpecifierSingleColon")
{
	std::string txt = ":";

	gscript::ParserNameSpecifier pName;
	auto result = pName.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserNameSpecifierStartsWithSpaces")
{
	std::string txt = " name";

	gscript::ParserNameSpecifier pName;
	auto result = pName.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pName.name == "name");
}

TEST_CASE("ParserNameSpecifierWithSpaces")
{
	std::string txt = "na me";

	gscript::ParserNameSpecifier pName;
	auto result = pName.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pName.name == "na"); // The rest is trimmed
}

TEST_CASE("ParserNameSpecifierStartsWithNumbers")
{
	std::string txt = "123name";

	gscript::ParserNameSpecifier pName;
	auto result = pName.parse(txt);

	REQUIRE(!result.isOk());
}

TEST_CASE("ParserNameSpecifierAllowedCharacters")
{
	// Currently only _ is allowed
	std::string txt = "some_name";

	gscript::ParserNameSpecifier pName;
	auto result = pName.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pName.name == "some_name");
}

TEST_CASE("ParserNameSpecifierDisallowedCharacters")
{
	std::string txt1 = "my@email";
	std::string txt2 = "!negate";
	std::string txt3 = "who!oops";
	std::string txt4 = "almost:namespace";
	std::string txt5 = "some(brackets)";
	std::string txt6 = "some\"quotes\"";
	std::string txt7 = "@";
	std::string txt8 = "!";
	std::string txt9 = "(";
	std::string txt10 = ")";
	std::string txt11 = "\"";

	gscript::ParserNameSpecifier pName1;
	auto result1 = pName1.parse(txt1);
	REQUIRE(result1.isOk());
	REQUIRE(pName1.name == "my");

	gscript::ParserNameSpecifier pName2;
	auto result2 = pName2.parse(txt2);
	REQUIRE(!result2.isOk());

	gscript::ParserNameSpecifier pName3;
	auto result3 = pName3.parse(txt3);
	REQUIRE(result3.isOk());
	REQUIRE(pName3.name == "who");

	gscript::ParserNameSpecifier pName4;
	auto result4 = pName4.parse(txt4);
	REQUIRE(result4.isOk());
	REQUIRE(pName4.name == "almost");

	gscript::ParserNameSpecifier pName5;
	auto result5 = pName5.parse(txt5);
	REQUIRE(result5.isOk());
	REQUIRE(pName5.name == "some");

	gscript::ParserNameSpecifier pName6;
	auto result6 = pName6.parse(txt6);
	REQUIRE(result6.isOk());
	REQUIRE(pName6.name == "some");

	REQUIRE(!gscript::ParserNameSpecifier().parse(txt7).isOk());
	REQUIRE(!gscript::ParserNameSpecifier().parse(txt8).isOk());
	REQUIRE(!gscript::ParserNameSpecifier().parse(txt9).isOk());
	REQUIRE(!gscript::ParserNameSpecifier().parse(txt10).isOk());
	REQUIRE(!gscript::ParserNameSpecifier().parse(txt11).isOk());
}

TEST_CASE("ParserNameSpecifierNamespaceAccessor")
{
	std::string txt = "namespace::name";

	gscript::ParserNameSpecifier pName;
	auto result = pName.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pName.name == "namespace::name");
}

TEST_CASE("ParserNameSpecifierNamespaceAccessorBegin")
{
	{
		std::string txt = "::something";

		gscript::ParserNameSpecifier pName;
		auto result = pName.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pName.name == "::something");
	}
	
	{
		std::string txt = ":something";

		gscript::ParserNameSpecifier pName;
		auto result = pName.parse(txt);

		REQUIRE(!result.isOk());
	}
}

TEST_CASE("ParserNameSpecifierNamespaceAccessorEnd")
{
	{
		std::string txt = "something::";

		gscript::ParserNameSpecifier pName;
		auto result = pName.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pName.name == "something::");
	}
	
	{
		// This one is OK because the colon character may be an inheritance operator
		std::string txt = "something:";

		gscript::ParserNameSpecifier pName;
		auto result = pName.parse(txt);

		REQUIRE(result.isOk());
		REQUIRE(pName.name == "something");
	}
}