#include "gscript/parser/pWord.hpp"
#include "gscript/StringUtils.hpp"

#include <string>
#include <sstream>

namespace gscript
{
	bool ParserWord::parsePredStrict(StringIteratorRange::ITERATOR_T it, StringIteratorRange::ITERATOR_T end, const std::string& buffer, const std::string&)
	{
		if (it + 1 == end)
			return true;
		char next = *(it + 1);
		if (!std::isalnum(next) && next != '_')
			return true;
		return false;
	}

	bool ParserWord::parsePredExact(StringIteratorRange::ITERATOR_T it, StringIteratorRange::ITERATOR_T end, const std::string& buffer, const std::string& word)
	{
		if (buffer == word)
			return true;

		return parsePredStrict(it, end, buffer, word);
	}

	bool ParserWord::parsePredNonWhitespace(StringIteratorRange::ITERATOR_T begin, StringIteratorRange::ITERATOR_T it, StringIteratorRange::ITERATOR_T end)
	{
		return !std::isspace(*it);
	}

	bool ParserWord::parsePredSpecial(StringIteratorRange::ITERATOR_T begin, StringIteratorRange::ITERATOR_T it, StringIteratorRange::ITERATOR_T end)
	{
		char c = *it;
		return std::isspace(c) || c == '(' || c == '{';
	}

	ParseResult ParserWord::parse(StringIteratorRange itrange, const std::string &word, std::shared_ptr<ParserEntity>&& subResult, std::function<parsePred> pred)
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
				if (pred(it, itrange.end, buffer, word))
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

	ParseResult ParserWord::parseExact(StringIteratorRange itrange, const std::string& word, std::shared_ptr<ParserEntity>&& subResult)
	{
		return parse(itrange, word, std::move(subResult), parsePredExact);
	}

	ParseResult ParserWord::parseUntil(StringIteratorRange itrange, const std::string &word, std::shared_ptr<ParserEntity>&& subResult, const std::string &allowed)
	{
		int length = word.length();

		StringIteratorRange::ITERATOR_T it = itrange.begin;
		size_t newlines = skipWhitespaces(it, itrange.end);

		if (it == itrange.end)
			return ParseResult(ParseResult::Status::Invalid, { itrange.shifted(newlines), (std::stringstream() << "Expected \"" << word << "\", got empty statement").str() });

		if (itrange.end - it < length)
			return ParseResult(ParseResult::Status::Invalid, { itrange.shifted(newlines), (std::stringstream() << "Expected \"" << word << "\", got \"" + getCharsUntilEol(it, itrange.end) + "\"").str() });

		std::string buffer(length, '0');

		int i = 0;
		for (; it != itrange.end; ++it)
		{
			if (itrange.end - it < length)
				break;

			buffer = std::string(it, it + length);
			bool ok = buffer == word;

			if (ok)
			{
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

		return ParseResult(ParseResult::Status::Invalid, { itrange.shifted(newlines), (std::stringstream() << "Expected \"" << word << "\", got \"" << buffer << "\"").str() });
	}

	ParseResult ParserWord::parseAny(StringIteratorRange itrange, bool trimLeadingWhitespaces, std::function<parseCharPred> predAllowed, std::function<parseCharPred> predFinishing)
	{
		StringIteratorRange::ITERATOR_T it = itrange.begin;
		size_t newlines = 0;
		
		if (trimLeadingWhitespaces)
		{
			newlines = skipWhitespaces(it, itrange.end);
		}

		StringIteratorRange::ITERATOR_T begin = it;

		if (it == itrange.end)
			return ParseResult(ParseResult::Status::Invalid, { itrange.shifted(newlines), (std::stringstream() << "Expected any character string, got empty statement").str() });

		for (; it != itrange.end; ++it)
		{
			if (it == itrange.end)
				break;

			char chr = *it;

			if (predFinishing(itrange.begin, it, itrange.end))
				break;

			if (!predAllowed(itrange.begin, it, itrange.end))
				return ParseResult(ParseResult::Status::Invalid, { itrange.shifted(newlines), (std::stringstream() << "Encountered disallowed character \"" << chr << "\"").str()});
		}

		if (begin == it)
			return ParseResult(ParseResult::Status::Invalid, { itrange.shifted(newlines), (std::stringstream() << "Expected any character string, got empty statement").str()});

		return ParseResult(ParseResult::Status::Ok, StringIteratorRange(begin, it, itrange.getFile(), itrange.getLine() + newlines));
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