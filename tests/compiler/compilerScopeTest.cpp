#include "common.h"
#include "gscript/script.hpp"
#include "gscript/extension/ConsoleExtension.hpp"
#include "gscript/StringUtils.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE_METHOD(GscriptTest, "CompilerScopeIf")
{
	std::string txt =
		R"GSCRIPT(
#import <console>

class Main : EntryPoint
{
	int run(int argc, string[] argv)
	{
		int x = 42;
		
		if (true)
		{
			int y = 1337;
			Console::println(tostring(x));
			Console::println(tostring(y));
		}
		
		return 0;
	}
}
		)GSCRIPT";

	std::ostringstream out;

	gscript::Script script;
	script.loadDefaultExtensions();
	static_cast<gscript::ConsoleExtension*>(script.findExtension("Console"))->out = &out;

	bool compileResult = script.compile(txt);
	REQUIRE(compileResult);

	int res = script.run(0, nullptr);

	REQUIRE(res == 0);
	REQUIRE(gscript::trim_copy(out.str()) == "42\n1337");
}

TEST_CASE_METHOD(GscriptTest, "CompilerScopeIfFailure")
{
	std::string txt =
		R"GSCRIPT(
#import <console>

class Main : EntryPoint
{
	int run(int argc, string[] argv)
	{
		if (true)
		{
			int y = 1337;
		}

		Console::println(tostring(y));
		
		return 0;
	}
}
		)GSCRIPT";

	std::ostringstream out;

	gscript::Script script;
	script.loadDefaultExtensions();
	static_cast<gscript::ConsoleExtension*>(script.findExtension("Console"))->out = &out;

	REQUIRE_THROWS_AS(script.compile(txt), gscript::CompileException);
}