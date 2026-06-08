#ifndef _h_gscript_runtimeexception
#define _h_gscript_runtimeexception

#include <exception>
#include <string>

namespace gscript
{
	class RuntimeException : public std::exception
	{
	public:
		RuntimeException(const std::string msg)
			: std::exception()
			, msg(std::string("Runtime error: ") + msg)
		{
		}

		virtual const char* what() const noexcept
		{
			return this->msg.c_str();
		}

	protected:
		std::string msg;
	};
}

#endif