// This example shows a method of calculating a Fibonacci sequence
// using recursion.

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