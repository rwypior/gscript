#ifndef _h_gscript_parser_blockbody
#define _h_gscript_parser_blockbody

#include "pEntity.hpp"
#include "IteratorRange.hpp"

#include <memory>
#include <vector>

namespace gscript
{
	class ParserBlockBody : public ParserEntity
	{
	public:
		std::vector<std::shared_ptr<ParserEntity>> statements;

		ParserBlockBody(unsigned int maxEntries = 0);
		void setMaxEntries(unsigned int maxEntries);

		virtual ParseResult parse(StringIteratorRange itrange) override;

	protected:
		unsigned int maxEntries = 0;
	};
}

#endif