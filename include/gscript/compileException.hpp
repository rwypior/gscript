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
			, msg(msg)
		{
		}

		virtual const char *what() const
		{
			return this->msg.c_str();
		}

	protected:
		std::string msg;
	};
}

#endif