#include "parser/pVarDeclaration.hpp"
#include "parser/pTypeSpecifier.hpp"
#include "parser/pNameSpecifier.hpp"
#include "parser/pEndStatement.hpp"
#include "parser/pOperator.hpp"
#include "parser/pComment.hpp"
#include "StringUtils.hpp"

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
		auto begin = itrange.begin;

		size_t newlines = skipWhitespaces(begin, itrange.end);

		unsigned int commentLength = 0;
		COMMENT(itrange, itrange.begin, commentLength);

		ParserTypeSpecifier type = ParserTypeSpecifier();
		ParseResult typeres = type.parse(StringIteratorRange(begin, itrange.end));
		if (!typeres.isOk())
			return typeres;
		this->type = typeres.getWord();

		ParseResult name = (ParserNameSpecifier()).parse(StringIteratorRange(typeres.result.end, itrange.end));
		if (!name.isOk())
			return name;
		this->name = name.getWord();

		auto end = name.result.end;
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
				return endstatement;
			end = endstatement.result.end;
		}

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(typeres.result.begin, end));
	}
}