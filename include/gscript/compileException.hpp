#ifndef _h_compileexception
#define _h_compileexception

#include <exception>
#include <string>

namespace gscript
{
	class CompileException : public std::exception
	{
	protected:
		std::string msg;

		virtual const char *what() const throw()
		{
			return this->msg.c_str();
		}

	public:
		CompileException(const std::string msg)
			:std::exception(),
			msg(std::string("Compilation error: ") + msg)
		{
		}
	};
}

#endif