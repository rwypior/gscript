gscript
=======

Gscript (name TBD) is a scripting language with designed for extending existing applications
with fast and easy to use scripting functionality.

The project is currently under development and many features are to be implemented. Contributions
are welcome.

Installation
------------

This project is based on CMake building system, and the basic installation procedure applies.
In order to build from the source code, refer to the following commands:

```
git clone https://github.com/rwypior/gscript.git
cd gscript
mkdir build && cd build
cmake ..
sudo cmake --install .
```

Then in your project's CMakeLists.txt find and link the library as:

```
find_package(gscript CONFIG)
target_link_libraries(your_target PRIVATE gscript::gscript)
```

And include any required headers in your source code.

```
#include <gscript/script.hpp>
```

Source code structure
---------------------

The source code is divided into the following structure:

```
- gscript
|- client: the command-line executable used to execute given script files
|- cmake: auxiliary CMake scripts
|- doc: additional documentation
|- example: example script snippets
|- include: C++ header files
|- src: C++ source files
|- tests: C++ testing files
```

Both `include` and `src` directories are split into logical units, each serving it's own
purpose:

- parser
Contains the code related to parsing raw string data into objects which are later used to create
runtime statements by the compiler
- runtime
Contains the code which will be executed after the compilation
- compiler
Contains the code that turns data structures created by the _parser_ module into _runtime_ modules
- extension
Contains code that will create [extensions](doc/EXTENSIONS.md) which provide additional, optional
features to the resulting script executable
- framework
Similar to the _extension_ module, provide mandatory extensions to the language, which provide
basic namespaces, classes or functions offered by the language

Usage
-----

In order to use the script as an embedded extension to an existing project, the following steps may be
used as a good starting point:

```
#include "script.hpp" // Include the primary include file

int main(int argc, char** argv)
{
	// First, create the script object. The path given here is used for two reasons:
	// - Source path for compilation
	// - Base path for script files included in specified script file
	gscript::Script scr("path-to-the-script-file.gs");

	// Optional - load default extensions. The default extensions, are all extensions
	// provided in the extension module
	scr.loadDefaultExtensions();

	try
	{
		// Compile the script
		scr.compile();
	}
	catch (const std::exception& ex)
	{
		// Handle any exceptions which may occur
		// This may include parsing exceptions and compilation exceptions
	}

	// Either, run the script the "standalone" way - this will search for "entrypoint"
	// class and run it's "run" method.
	auto returncode = scr.run(argc, argv);

	// Or execute any part of the script as desired. This example will look for "myfunc"
	// function with no arguments, run it, and get it's return value.
	auto result = scr.getMainScope()->findFunction("myfunc", {})->fastrun(*scr.getMainScope());
}
```

Data flow in and out of the script is primarily handled by the `ScriptValue` class and it's
variations. The base class `ScriptValue` provides a set of utility functions used to access
the data. Providing the data _to_ the script, required the manual creation of `ScriptValue`
subclasses. Refer to [data-handling](doc/DATA-HANDLING.md) documentation for more information.

Testing
-------

The project testing strategy is based on Catch2 framework, built as a separete executable, which
sources are supplied in the `tests` directory. Tests are built with the main project by default,
and can be run by simply executing the `gscriptTest` executable.

Tests are split into logic units, similar to the way source code is split. Tests with name prefixed
with _parser_ word are testing parsing functionality, _runtime_ are testing runtime functionality,
_compiler_ are testing runtime functionality created by parser module, and _integration_ are testing
more generic functionalities, such as fibonacci sequence utilizing many areas of the project.

Syntax example
--------------

```
#import <console>

int fib(int n)
{
    if (n <= 1)
        return n;

	int a = fib(n - 1);
	int b = fib(n - 2);

    return a + b;
}

class Main : EntryPoint
{
	int run(int argc, string[] argv)
	{
		Console::println(tostring(fib(10)));
		
		return 0;
	}
}
```

The above code calculates the Fibonacci sequence sequence by recursive method and prints the output
the the standard output.

More docs
---------

[Data handling](doc/DATA-HANDLING.md)
[Extensions](doc/EXTENSIONS.md)
[Syntax](doc/SYNTAX.md)