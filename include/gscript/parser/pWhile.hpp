#ifndef _h_gscript_parser_while
#define _h_gscript_parser_while

#include "pEntity.hpp"
#include "pCallArglist.hpp"
#include "pBlock.hpp"
#include "pElse.hpp"
#include "IteratorRange.hpp"

namespace gscript
{
	class ParserWhile : public ParserEntity
	{
	public:
		static const char *KW_WHILE;

		ParserWhile();
		ParserWhile(const ParserWhile&) = delete;
		ParserWhile(ParserWhile&& b) noexcept
			: body(std::move(b.body))
			, arglist(std::move(b.arglist))
		{
		}

		ParserBlock body;
		ParserCallArglist arglist;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif