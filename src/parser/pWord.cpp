#include "parser/pWord.hpp"
#include "StringUtils.hpp"

#include <string>
#include <sstream>

namespace gscript
{
	extern const std::string ParserWord::WORD_ANY = "";

	ParseResult ParserWord::parse(StringIteratorRange itrange, const std::string &word, std::shared_ptr<ParserEntity>&& subResult/*, bool allowSpaces*/)
	{
		int length = word.length();

		StringIteratorRange::ITERATOR_T it = itrange.begin;
		size_t newlines = skipWhitespaces(it, itrange.end);

		if (it == itrange.end)
			return ParseResult(ParseResult::Status::Invalid, { itrange.shifted(newlines), (std::stringstream() << "Expected \"" << word << "\", got empty statement").str()});

		if (itrange.end - it < length)
			return ParseResult(ParseResult::Status::Invalid, { itrange.shifted(newlines), (std::stringstream() << "Expected \"" << word << "\", got \"" + getCharsUntilEol(it, itrange.end) + "\"").str()});

		std::string buffer(length, '0');

		if (std::isdigit(*it))
			return ParseResult(ParseResult::Status::Invalid, { itrange.shifted(newlines), "Expected alphanumeric word, got number" });

		int i = 0;
		for (; it != itrange.end; ++it)
		{
			char chr = *it;
			buffer[i++] = chr;

			if (i == length)
			{
				/*if (allowSpaces && buffer == word)
					break;*/

				if (it + 1 == itrange.end)
					break;
				char next = *(it + 1);
				if (!std::isalnum(next) && next != '_')
					break;

				return ParseResult(ParseResult::Status::Invalid, { itrange.shifted(newlines), "Expected \"" + word + "\"" });
			}
		}

		if (buffer == word)
		{
			StringIteratorRange::ITERATOR_T foundBegin = it + 1 - length;

			return ParseResult(ParseResult::Status::Ok, StringIteratorRange(foundBegin, it + 1), std::move(subResult));
		}

		return ParseResult(ParseResult::Status::Invalid, {itrange.shifted(newlines), (std::stringstream() << "Expected \"" << word << "\", got \"" << buffer << "\"").str() });
	}

	ParseResult ParserWord::parseUntil(StringIteratorRange itrange, const std::string &word, std::shared_ptr<ParserEntity>&& subResult, const std::string &allowed)
	{
		int length = word.length();

		if (itrange.end - itrange.begin < length + 1)
			return ParseResult(ParseResult::Status::Invalid);

		std::string buffer(length, '0');

		StringIteratorRange::ITERATOR_T it = itrange.begin;

		size_t newlines = 0;
		int i = 0;
		for (; it != itrange.end; ++it)
		{
			buffer = std::string(it, it + length);
			bool ok = buffer == word;

			if (ok)
			{
				//return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(itrange.begin, it + 1), std::move(subResult));
				return ParseResult(ParseResult::Status::Ok, StringIteratorRange(itrange.begin, it, itrange.getFile(), itrange.getLine() + newlines), std::move(subResult));
			}
			else if (!allowed.empty())
			{
				char chr = *it;
				newlines += isNewLine(chr);

				if (allowed.find(chr) == std::string::npos)
					return ParseResult(ParseResult::Status::Invalid, {itrange.shifted(newlines), "Expected one of \"" + allowed + "\", got \"" + chr + "\""});
			}
		}

		return ParseResult(ParseResult::Status::Invalid, {itrange.shifted(newlines), "Expected \"" + word + "\", got \"" + buffer + "\"" });
	}

	void ParserWord::copy(char *destination, StringIteratorRange itrange)
	{
		int i = 0;
		for (StringIteratorRange::ITERATOR_T it = itrange.begin; it != itrange.end; ++it, ++i)
		{
			destination[i] = *it;
		}
	}
}