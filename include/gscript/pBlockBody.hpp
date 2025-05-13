#ifndef _h_parser_blockbody
#define _h_parser_blockbody

#include "pEntity.hpp"

#include <memory>

namespace gscript
{
	class ParserBlockBody : public ParserEntity
	{
	protected:
		unsigned int maxEntries = 0;

	public:
		std::vector<std::shared_ptr<ParserEntity>> statements;

		ParserBlockBody(unsigned int maxEntries = 0);
		void setMaxEntries(unsigned int maxEntries);

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif