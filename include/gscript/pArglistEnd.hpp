#ifndef _h_parser_arglist_end
#define _h_parser_arglist_end

#include "pArglistStart.hpp"

namespace gscript
{
	class ParserArglistEnd : public ParserArglistStart
	{
	public:
		static const char C_ARGLIST_END;

		ParserArglistEnd(char c = ParserArglistEnd::C_ARGLIST_END)
			:ParserArglistStart(c)
		{
		}
	};
}

#endif