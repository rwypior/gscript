#ifndef _h_runtimeexception
#define _h_runtimeexception

#include <exception>
#include <string>

namespace gscript
{
	class RuntimeException : public std::exception
	{
	protected:
		std::string msg;

		virtual const char *what() const throw()
		{
			return this->msg.c_str();
		}

	public:
		RuntimeException(const std::string msg)
			:std::exception(),
			msg(std::string("Runtime error: ") + msg)
		{
		}
	};
}

#endif