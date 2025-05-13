#include "pVarDeclaration.hpp"

#include "pTypeSpecifier.hpp"
#include "pNameSpecifier.hpp"
#include "pEndStatement.hpp"
#include "pOperator.hpp"
#include "pComment.hpp"

#include <iostream>

namespace gscript
{
	ParserVarDeclaration::ParserVarDeclaration(bool sub)
		:sub(sub),
		value(true)
	{
	}

	ParseResult ParserVarDeclaration::parse(StringIteratorRange itrange)
	{
		std::string::iterator begin = itrange.begin;

		unsigned int commentLength = 0;
		COMMENT(itrange, itrange.begin, commentLength)

			ParserTypeSpecifier type = ParserTypeSpecifier();
		ParseResult typeres = type.parse(itrange);
		if (!typeres.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, COMMENT_RESULT(itrange, commentLength));
		this->type = typeres.getWord();

		ParseResult name = (ParserNameSpecifier()).parse(StringIteratorRange(typeres.result.end + 1, itrange.end));
		if (!name.isOk())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, COMMENT_RESULT(itrange, commentLength));
		this->name = name.getWord();

		std::string::iterator end = name.result.end;
		ParseResult assign = (ParserOperatorAssign()).parse(StringIteratorRange(end, itrange.end));

		if (assign.isOk())
		{
			end = assign.result.end;
			ParseResult valres = this->value.parse(StringIteratorRange(end, itrange.end));
			if (valres.isOk())
				end = valres.result.end;
		}

		if (!sub)
		{
			ParseResult endstatement = (ParserEndStatement()).parse(StringIteratorRange(end, itrange.end));
			if (!endstatement.isOk())
				return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
			end = endstatement.result.end;
		}

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(typeres.result.begin, end));
	}
}