#include "common.h"
#include "gscript/compiler/compiler.hpp"
#include "gscript/parser/pFunction.hpp"
#include "gscript/parser/pStatement.hpp"
#include "gscript/parser/pNamespace.hpp"
#include "gscript/runtime/namespace.hpp"
#include "gscript/runtime/statement.hpp"
#include "gscript/runtime/funcCall.hpp"
#include "gscript/runtime/class.hpp"
#include "gscript/runtime/classInstance.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/logger.hpp"
#include "gscript/StringUtils.hpp"

#include <catch2/catch_all.hpp>

#include <iostream>
#include <ostream>

TEST_CASE_METHOD(GscriptTest, "CompilerExtensionPrintlnFromNamespace")
{
	std::string txt =
		"#import <console>"
		"namespace mynamespace {\n"
		"	void myfunc() {\n"
		"		Console::println(tostring(42));\n"
		"	}\n"
		"}"
		;

	gscript::Script script;
	script.loadDefaultExtensions();

	script.compile(txt);

	auto& globalNamespace = *script.getMainScope();

	auto myns = globalNamespace.findNamespace("mynamespace");
	auto myfunc = myns->getFunction("myfunc", {});
	
	std::ostringstream capture;
	auto cout_orig = std::cout.rdbuf(capture.rdbuf());

	myfunc->run(globalNamespace);

	std::cout.rdbuf(cout_orig);

	REQUIRE(gscript::trim_copy(capture.str()) == "42");
}