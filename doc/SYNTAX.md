Syntax
======

The script syntax is heavility influenced by the syntax of the family of C languages.
This page will provide a summary of syntax and keywords this language accepts.

The compiler doesn't interpret white characters, meaning it makes no difference if
there is one space, or more spaces between a type and variable name, or if a bracket
is placed in the same line as `if`, or in the next line, etc.

All names (variables, classes, namespace etc.) are case sensitive, and a given block
cannot have duplicate symbols declared in it.

Unlike C-family, this language doesn't require forward declarations, as all function,
classes, namespace etc. are available equally at every point in the code.

Examples
========

The best way is to learn by example.
A script code examples may be found in the [examples/](/examples/) directory.
Host-side (integration) examples, may be found in unit tests - primarily 
[compiler](/tests/compiler/) and [integration](/tests/integration) tests.

namespacees
-----------

A namespace aggregates symbols (other namespaces, classes, functions) in a block specified
by the namespace under it's name. The namespace follows the following syntax:

```
namespace _name_
{
	// Nested code
}
```

classes
-------

A class provides a concept of data structures. Classes may contain fields (variables),
functions (method), or nested classes. Classes may inherit other classes, effectively
extending them with the functionality of other classes.

An access to methods and fields of the class may be controlled by one of the following
access specifiers:

- private
- protected
- public

where `private` only allows access to the member by the class itself - never outside of
the class, `protected` extends the access the inherited classes, and `public` allows
the access to any entity. The default access provider is `public`.

The class follows the following syntax:

```
class _name_ : inherited_class_1, inherited_class_2
{
	void some_function()
	{
		// ...
	}_

	int some_value = 42;_
}
```

The class may be instantiated using the `new` syntax, as follows:

```
ClassName varName = new ClassName(params);
```

functions
---------

Functions are the basic unit of code execution and is described with function signature,
which consists of return type, name and parameters. Example syntax is as follows:

```
int functionName(int a, int b)
{
	return a * b;*
}
```

methods
-------

Methods are essentially class member functions, providing additional functionalities of
access specifiers and `this` keywords representing the function's parent class.
Like all class members, the default access specifier of a method is `public`. Example
syntax is as follows:

```
protected int methodName(int x)
{
	this->someVariable = x;
}
```

Execution block
---------------

Execution block is an abstract construct of a block of code providing it's own `scope`, where
the scope is a container of statements and variables. At the end of the scope, all of it's
contents are destroyed. Given scope may access members of it's parent scopes, but **not** the
other way as shown in the following pseudocode:

```
parent scope {
	int first_variable = 42;

	child_scope {
		int another_variable = 1337;
		use another_variable; // Completely fine
		use first_variable; // Fine too
	}

	use first_variable; // Totally fine
	use another_variable; // Error - this variable is a mamber of (already destroyed) child scope
}
```

if
--

If is a conditional expression which is executed when the it's argument evaluates to
an equivalent of `true`. If may be optionally followed by `else`, which is a block executed
when the condition of the original `if` evaluates to an equivalent of `false`. `If` and `else`
may be combined to created `else if` statement. Example syntax:

```
int x = something;
if (x == 42)
{
	println("It's 42!");
}
else if (x = 1337)
{
	println("Leet!");
}
else
{
	println("Meh");
}
```

for
---

For is a loop providing an initialization step, condition step and progress step. The for
loop first starts by executing an initialization step (usually a variable declaration) and
executes it's execution block when the condition is met, followed by it's progress step.

A typical syntax of for loop may look like the following:

```
for (int i = 0; i \< 10; i++)
{
	println(tostring(i));
}
```

which will print numbers from 0 to 9.

while
-----

While is a loop similar to `for` loop but only evaluating the single condition step instead of
for's initialization, condition and progress steps. Example:

```
int i = 0;
while(i \< 10)
{
	println(tostring(i));
	i++;
}
```

variables
---------

Variable is a carrier of named data described by a type, name and optionally a default value.
Variable may be initialized by a literal value, another variable's value or a result of a 
function call. Example syntax:

```
int x = 42;
int y = myFunction();
int z = x * y;
string w = "something";
```