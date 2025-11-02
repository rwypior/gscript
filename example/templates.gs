// This example shows an example of usage of template usage
// A template is a type which substitutes a template type
// with a real type during compilation type, in order to
// create a generic type as needed
// 
// NOTE - To be implemented

#import <console>

template<typename T>
void somefunc(T param)
{
	Console::println("somefunc:");
	Console::println(tostring(param));
}

class Main : EntryPoint
{
	int run(int argc, string[] argv)
	{
		somefunc<int>(1337);

		return 0;
	}
}