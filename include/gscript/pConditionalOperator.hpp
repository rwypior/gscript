#ifndef _h_parser_conditional_operator
#define _h_parser_conditional_operator

#include "pEntity.hpp"
#include "pForArglist.hpp"
#include "pBlock.hpp"
#include "pElse.hpp"

namespace gscript
{
	class ParserConditionalOperator : public ParserEntity
	{
	public:
		static const char *KW_COND;
		static const char *KW_ELSE;

		ParserConditionalOperator();

		ParserStatement condition;
		ParserStatement *ontrue = nullptr;
		ParserStatement onfalse;

		virtual ParseResult parse(StringIteratorRange itrange) override;
	};
}

#endif