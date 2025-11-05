Extensions
==========

The language provides simple mechanism of extending the it's functionality by
extensions. Extensions are simple classes registered and executed **before** the
compilation stage. The host application may introduce any amount of extensions
performing _any_ action. Typically the extensions are used to register _namespaces_,
_classes_ and _functions_, but their behavior is _not_ limited to just that.

Extensions are loaded into the script using the `#import` syntax with angle brackets,
for example:

```
#import <Console>
```

which loads the console extension.

Default extensions
==================

The script provides a set of default extensions, loaded by `gscript::Script::loadDefaultExtensions`
function, which loads all extensions provided in _(src;include)/extension_ directory.
Default extensions are listed below.

\<console\> - Console extension
-----------------

Provides basic input/output operations to the standard output accessible by functions
registered in `Console` namespace.

#### [namespace] Console

The namespace containing all console-related functions.

##### void print(string msg)

Outputs the string `msg` into the standard output.

##### void println(string msg)

Outputs the string `msg` followed by new line feed into the standard output.

##### string readln()

Reads a line from the standard output and returns it.

\<io\>
-----------------

Provides basic input/output operations to filesystem.

#### [class] File

A class representing a file.

##### [constructor] File(string path, string mode)

Opens the file located at `path` with mode specified by `mode`.
The `mode` must be one of:
- `w` - opens for write
- `r` - opens for read

##### int readall(string& output)

Reads all bytes from the file into the `output` reference parameter.
Returned value is an integer equal to number of bytes read.

##### static bool exists(string path)

Returns `true` if file located at `path` exists otherwise `false`.