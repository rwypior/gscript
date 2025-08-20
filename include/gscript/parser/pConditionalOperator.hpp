#ifndef _h_gscript_parser_conditional_operator
#define _h_gscript_parser_conditional_operator

#include "pEntity.hpp"
#include "pStatement.hpp"
#include "IteratorRange.hpp"

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