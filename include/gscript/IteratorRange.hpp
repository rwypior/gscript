#ifndef _h_gscript_iteratorrange
#define _h_gscript_iteratorrange

#include <string>

namespace gscript
{
	template<typename T>
	class IteratorRange
	{
	public:
		typedef typename T::const_iterator ITERATOR_T;

		ITERATOR_T begin;
		ITERATOR_T end;

		IteratorRange()
		{
		}

		IteratorRange(ITERATOR_T begin, ITERATOR_T end)
			: begin(begin)
			, end(end)
		{
		}

		const std::string getWord()
		{
			return std::string(this->begin, this->end);
		}

		const unsigned int getLength() const
		{
			return end - begin;
		}
	};

	class StringIteratorRange : public IteratorRange<std::string>
	{
	// Constructor from const char* not allowed - it's not supposed to hold copy of string

	public:
		StringIteratorRange()
		{
		}

		StringIteratorRange(ITERATOR_T begin, ITERATOR_T end, const std::string& file = "", size_t line = 1/*, size_t newlines = 0*/)
			: IteratorRange<std::string>(begin, end)
			, file(file)
			, line(line)
			//, newlines(newlines)
		{
		}

		StringIteratorRange(const std::string& str, const std::string& file = "", size_t line = 1/*, size_t newlines = 0*/)
			: StringIteratorRange(str.begin(), str.end(), file, line/*, newlines*/)
		{
		}

		const std::string& getFile() const
		{
			return this->file;
		}

		size_t getLine() const
		{
			return this->line;
		}

		StringIteratorRange at(size_t line) const
		{
			return StringIteratorRange(this->begin, this->end, this->file, line);
		}

		StringIteratorRange at(const std::string& file, size_t line) const
		{
			return StringIteratorRange(this->begin, this->end, file, line);
		}

		StringIteratorRange shifted(size_t lines) const
		{
			return StringIteratorRange(this->begin, this->end, this->file, this->line + lines);
		}

		StringIteratorRange shifted(const std::string& file, size_t lines) const
		{
			return StringIteratorRange(this->begin, this->end, file, this->line + lines);
		}

	protected:
		std::string file = "";
		size_t line = 1;
	};
}

#endif