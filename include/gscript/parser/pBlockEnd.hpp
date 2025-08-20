#ifndef _h_gscript_parser_blockend
#define _h_gscript_parser_blockend

#include "pBlockStart.hpp"

namespace gscript
{
	class ParserBlockEnd : public ParserBlockStart
	{
	public:
		ParserBlockEnd()
			: ParserBlockStart('}')
		{
		}
	};
}

#endif