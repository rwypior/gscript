// This example shows the way in which scope behaves in the context
// variables. Current scope can access variables declared in that scope
// and the parent scope, but outer scope cannot access variables in
// child scopes - in this case, the variable 'x' can be accessed in any
// scope inside 'run' method, but 'y' variable can only be accessed in
// the inner 'if' scope.

#import <console>

class Main : EntryPoint
{
	int run(int argc, string[] argv)
	{
		int x = 42;

		Console::println(tostring(x));

		if (true)
		{
			int y = 1337;
			Console::println(tostring(x));
			Console::println(tostring(y));
		}

		Console::println(tostring(y)); // 'y' is out of scope, this line throws compilation error

		return 0;
	}
}