#ifndef _h_gscript_parser_blockend
#define _h_gscript_parser_blockend

#include "pBlockStart.hpp"

namespace gscript
{
	class ParserBlockEnd : public ParserBlockStart
	{
	public:
		static constexpr char keycharBlockEnd = '}';

		ParserBlockEnd()
			: ParserBlockStart(keycharBlockEnd)
		{
		}
	};
}

#endif