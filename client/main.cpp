#ifndef BUILD_SCRIPT_DLL

#include <iostream>
#include <fstream>
#include <limits>

#include <chrono>

#include "script.hpp"

typedef std::chrono::high_resolution_clock Clock;

int main(int argc, char **argv)
{
	if (argc <= 1)
	{
		std::cout << "Please specify path to the script" << std::endl;
		return 1;
	}

	gscript::Script scr(argv[1]);

	scr.loadDefaultExtensions();

	auto begin = Clock::now();
	scr.compile();
	double period = std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - begin).count() * 0.000001;

	std::cout << "Compile finished in " << period << "s" << std::endl;

	int returncode = 0;
	try
	{
		begin = Clock::now();
		returncode = scr.run(argc, argv);
		period = std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - begin).count() * 0.000001;

		std::cout << "Run finished with code " << returncode << " in " << period << "s" << std::endl;
	}
	catch (const std::exception &ex)
	{
		std::cout << "An exception has been thrown: " << ex.what() << std::endl;
		std::cout << "Program aborted." << std::endl;
	}

	std::cin.ignore();
	std::cin.get();

	return returncode;
}

#endif