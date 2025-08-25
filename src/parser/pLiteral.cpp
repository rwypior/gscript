#include "parser/pLiteral.hpp"

namespace gscript
{
	ParseResult ParserLiteral::parse(StringIteratorRange itrange)
	{
		bool isString = false;
		bool isChar = false;
		bool escape = false;
		bool isFloat = false;
		bool isHex = false;
		bool cleanEnd = true;
		bool isInt = false;

		std::string::const_iterator begin = itrange.end;
		size_t newlines = 0;

		for (auto it = itrange.begin; it != itrange.end; ++it)
		{
			newlines += std::isspace(*it);
			if (!std::isspace(*it))
			{
				begin = it;
				break;
			}
		}

		auto it = begin;

		if (it == itrange.end)
			return ParseResult(ParseResult::STATUS_T::S_FATAL, { itrange.shifted(newlines), "Expected literal value, got empty string" });

		if (*begin == '"')
		{
			isString = true;
			cleanEnd = false;
			++it;
		}
		else if (*begin == '\'')
		{
			isChar = true;
			cleanEnd = false;
			++it;
		}

		for (; it != itrange.end; ++it)
		{
			char chr = *it;

			if (escape)
			{
				switch (chr)
				{
				case 'n': chr = '\n'; break;
				case 't': chr = '\t'; break;
				case 'r': chr = '\r'; break;
				case 'v': chr = '\v'; break;
				case 'f': chr = '\f'; break;
				case 'a': chr = '\a'; break;
				case '\\': chr = '\\'; break;
				}

				escape = false;
			}
			else
			{
				if (isString || isChar)
				{
					if (chr == '\\')
					{
						escape = true;
						continue;
					}

					if (isString && chr == '"')
					{
						cleanEnd = true;
						++it;
						break;
					}
					else if (isChar && chr == '\'')
					{
						cleanEnd = true;
						++it;
						break;
					}
				}
				else
				{
					if (std::isalpha(chr))
						return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
				}

				if (chr == '.')
					isFloat = true;
				else if (!isString && !isChar && !std::isdigit(chr))
					break;
				else if (std::isdigit(chr))
					isInt = true;
			}

			this->value.push_back(chr);

			if (isChar && this->value.length() > 1)
				return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());
		}

		if (!cleanEnd)
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		if (!isString && this->value.empty())
			return ParseResult(ParseResult::STATUS_T::S_FATAL, StringIteratorRange());

		if (isString)
			this->type = VALUE_TYPE_T::VT_STRING;
		else if (isChar)
			this->type = VALUE_TYPE_T::VT_CHAR;
		else
		{
			if (this->value.find_first_of('.') != std::string::npos)
			{
				this->type = VALUE_TYPE_T::VT_DOUBLE;

				if (this->value.back() == 'f' || this->value.back() == 'F')
					this->type = VALUE_TYPE_T::VT_FLOAT;
			}
			else
				this->type = VALUE_TYPE_T::VT_INT;
		}

		return ParseResult(ParseResult::STATUS_T::S_OK, StringIteratorRange(begin, it));
	}
}