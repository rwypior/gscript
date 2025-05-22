#ifndef _h_gscript_debug
#define _h_gscript_debug

#ifdef DEBUG
    #define DBG(text) std::cout << text << std::endl;
#else
    #define DBG(text)
#endif

#endif