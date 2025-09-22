#include "common.h"
#include "gscript/parser/pImportDirective.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("ParserImportDirectiveFile")
{
	std::string txt = "#import \"something\"";

	gscript::ParserImportDirective pCtrl;
	auto result = pCtrl.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pCtrl.directive == "import");
	REQUIRE(pCtrl.type == gscript::ParserImportDirective::IMPORT_TYPE_T::IT_FILE);
	REQUIRE(pCtrl.filename == "something");
}

TEST_CASE("ParserImportDirectiveFailureNoChar")
{
	std::string txt = "import \"something\"";

	gscript::ParserImportDirective pCtrl;
	auto result = pCtrl.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Expected \"#\", got \"i\"");
}

TEST_CASE("ParserImportDirectiveExtension")
{
	std::string txt = "#import <someext>";

	gscript::ParserImportDirective pCtrl;
	auto result = pCtrl.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pCtrl.directive == "import");
	REQUIRE(pCtrl.type == gscript::ParserImportDirective::IMPORT_TYPE_T::IT_EXTENSION);
	REQUIRE(pCtrl.filename == "someext");
}

TEST_CASE("ParserImportDirectiveFailureNoEnclosure")
{
	std::string txt = "#import \"something";

	gscript::ParserImportDirective pCtrl;
	auto result = pCtrl.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Missing import directive enclosure \"\"\"");
}

TEST_CASE("ParserImportDirectiveFailureNoEnclosureExtension")
{
	std::string txt = "#import <something";

	gscript::ParserImportDirective pCtrl;
	auto result = pCtrl.parse(txt);

	REQUIRE(!result.isOk());
	REQUIRE(result.details.message == "Missing import directive enclosure \">\"");
}

TEST_CASE("ParserImportDirectiveCommentLineBefore")
{
	std::string txt = 
		"// This is a comment\n"
		"#import \"something\"";

	gscript::ParserImportDirective pCtrl;
	auto result = pCtrl.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pCtrl.directive == "import");
	REQUIRE(pCtrl.type == gscript::ParserImportDirective::IMPORT_TYPE_T::IT_FILE);
	REQUIRE(pCtrl.filename == "something");
}

TEST_CASE("ParserImportDirectiveCommentBlockAfterImport")
{
	std::string txt = "#import /* This is a comment */ \"something\"";

	gscript::ParserImportDirective pCtrl;
	auto result = pCtrl.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pCtrl.directive == "import");
	REQUIRE(pCtrl.type == gscript::ParserImportDirective::IMPORT_TYPE_T::IT_FILE);
	REQUIRE(pCtrl.filename == "something");
}

TEST_CASE("ParserImportDirectiveCommentBlockAfterFile")
{
	std::string txt = "#import \"something\" /* This is a comment */";

	gscript::ParserImportDirective pCtrl;
	auto result = pCtrl.parse(txt);

	REQUIRE(result.isOk());
	REQUIRE(pCtrl.directive == "import");
	REQUIRE(pCtrl.type == gscript::ParserImportDirective::IMPORT_TYPE_T::IT_FILE);
	REQUIRE(pCtrl.filename == "something");
}