// This example shows an example of a built-in event type
// which accumulates event subscribers and manages it's own
// lifetime, and lifetime of it's subscribers. For example
// this event type will automatically unregister its' subscribers
// when they will go out of scope, or (in case if the subscriber
// is a member of a class object) the object is deleted.

// NOTE - To be implemented

#import <console>

void somefunc(int a, float b)
{
	Console::println("somefunc:");
	Console::println(tostring(a));
	Console::println(tostring(b));
}

void anotherfunc(int a, float b)
{
	Console::println("anotherfunc:")
		Console::println(tostring(a));
	Console::println(tostring(b));
}

class Main : EntryPoint
{
	int run(int argc, string[] argv)
	{
		event void(int a, float b) myEvent;

		myEvent += somefunc;

		myEvent(42, 13.37);

		myEvent += anotherfunc;

		myEvent(12, 3.4);
		myEvent(56, 7.8);

		return 0;
	}
}