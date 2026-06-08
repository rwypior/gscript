#include "common.h"
#include "gscript/script.hpp"
#include "gscript/extension/ConsoleExtension.hpp"
#include "gscript/StringUtils.hpp"

#include <catch2/catch_all.hpp>

#include <ostream>

TEST_CASE("Integration::ClassTest::Basic")
{
	std::string txt =
		R"GSCRIPT(
#import <console>

class MyClass
{
	MyClass()
	{
		Console::println("Hello from MyClass");
	}

	void echo()
	{
		Console::println("Echo");
	}
}

class Main : EntryPoint
{
	int run(int argc, string[] argv)
	{
		MyClass mc = new MyClass();
		mc.echo();
		
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
	REQUIRE(gscript::trim_copy(out.str()) == "Hello from MyClass\nEcho");
}

TEST_CASE("Integration::ClassTest::Reading fields")
{
	std::string txt =
		R"GSCRIPT(
#import <console>

class MyClass
{
	int getvar()
	{
		return this.myvar;
	}

	int myvar = 123;
}

class Main : EntryPoint
{
	int run(int argc, string[] argv)
	{
		MyClass mc = new MyClass();
		int myvar = mc.getvar();
		Console::println(tostring(myvar));
		
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
	REQUIRE(gscript::trim_copy(out.str()) == "123");
}