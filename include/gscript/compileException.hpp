#ifndef _h_gscript_compileexception
#define _h_gscript_compileexception

#include <exception>
#include <string>

namespace gscript
{
	class CompileException : public std::exception
	{
	public:
		CompileException(const std::string msg)
			: std::exception()
			, msg(std::string("Compilation error: ") + msg)
		{
		}

	protected:
		std::string msg;

		virtual const char *what() const throw()
		{
			return this->msg.c_str();
		}
	};
}

#endif