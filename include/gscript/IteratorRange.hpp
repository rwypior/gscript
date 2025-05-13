#ifndef _h_iteratorrange
#define _h_iteratorrange

#include <string>

namespace gscript
{
	template<typename T>
	class IteratorRange
	{
	public:
		typedef typename T::iterator ITERATOR_T;

		ITERATOR_T begin;
		ITERATOR_T end;

		IteratorRange()
		{}

		IteratorRange(ITERATOR_T begin, ITERATOR_T end)
			: begin(begin),
			end(end)
		{}

		const std::string getWord()
		{
			return std::string(this->begin, this->end);
		}

		const unsigned int getLength() const
		{
			return end - begin;
		}
	};
}

#endif