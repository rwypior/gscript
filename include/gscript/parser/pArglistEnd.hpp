#ifndef _h_gscript_parser_arglist_end
#define _h_gscript_parser_arglist_end

#include "gscript/parser/pArglistStart.hpp"
#include "gscript/IteratorRange.hpp"

namespace gscript
{
	class ParserArglistEnd : public ParserArglistStart
	{
	public:
		static constexpr char keycharArglistEnd = ')';

		ParserArglistEnd(char c = ParserArglistEnd::keycharArglistEnd)
			: ParserArglistStart(c)
		{
		}
	};
}

#endif