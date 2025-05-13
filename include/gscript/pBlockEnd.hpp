#ifndef _h_parser_blockend
#define _h_parser_blockend

namespace gscript
{
	class ParserBlockEnd : public ParserBlockStart
	{
	public:
		ParserBlockEnd()
			:ParserBlockStart('}')
		{
		}
	};
}

#endif