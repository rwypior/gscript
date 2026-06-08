#include "gscript/defs.hpp"
#include "gscript/parser/pOperatorDeclaration.hpp"
#include "gscript/parser/pComment.hpp"
#include "gscript/parser/pWord.hpp"

#include <map>
#include <sstream>

namespace gscript
{
	ParseResult ParserOperatorDeclaration::parse(StringIteratorRange itrange)
	{
		auto end = itrange.begin;

		end = parseComment(end, itrange.end);

		ParseResult keywordResult = ParserWord::parse(StringIteratorRange(end, itrange.end), keywordOperator);
		if (!keywordResult.isOk())
			return keywordResult;
		end = keywordResult.result.end;

		ParseResult operatorres = ParserWord::parseAny(StringIteratorRange(end, itrange.end), true, gscript::ParserWord::parsePredNonWhitespace, 
			[](StringIteratorRange::ITERATOR_T begin, StringIteratorRange::ITERATOR_T it, StringIteratorRange::ITERATOR_T end) 
		{
			// Only break on opening bracket if it's not call operator, or call operator has been closed
			char c = *it;
			if (end != begin && it != std::prev(end, 1))
			{
				char next = *std::next(it);
				if (c == '(' && next == ')')
					return false;
			}

			return ParserWord::parsePredSpecial(begin, it, end);
		});
		if (!operatorres.isOk())
			return operatorres.as(ParseResult::Status::Fatal);
		end = operatorres.result.end;

		std::string operatorWord = operatorres.getWord();

		if (operatorWord.size() >= 3 && std::string(std::prev(operatorWord.end(), 2), operatorWord.end()) == "()")
		{
			end = std::prev(operatorres.result.end, 2);
			operatorWord = std::string(operatorres.result.begin, end);
		}

		this->operatorType = translateOperator(operatorWord);
		if (this->operatorType == OperatorType::Invalid)
			return ParseResult(ParseResult::Status::Fatal, operatorres.details.withMessage((std::stringstream() << "Invalid operator \"" << operatorWord << "\"").str()));

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(itrange.begin, end));
	}

	OperatorType ParserOperatorDeclaration::translateOperator(const std::string& oper)
	{
		const static auto& typemap = getOperatorMap();

		const auto it = typemap.find(oper);
		if (it != typemap.end())
			return it->second;
		return OperatorType::Invalid;
	}
}