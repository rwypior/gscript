#ifndef _h_script_lib
#define _h_script_lib

#if defined (__GNUC__) 
	#define SCRIPT_API __attribute__ ((__visibility__("default")))
#elif defined (WIN32)
	#ifdef BUILD_SCRIPT_DLL
		#define SCRIPT_API __declspec(dllexport)
	#else
		#ifndef BUILD_NOLIB
			#define SCRIPT_API __declspec(dllimport)
		#else
			#define SCRIPT_API
		#endif
	#endif
#endif

#endif