#include "new.hpp"
#include "class.hpp"
#include "classInstance.hpp"

#include "compileException.hpp"

namespace gscript
{
	ScriptNew::ScriptNew(ScriptClass &sclass, const std::vector<ScriptStatement> &params)
		//:ScriptFuncCall(sclass, sclass.getConstructor(), params),
		:ScriptFuncCall(sclass, new DirectEntityLink<ScriptFunction*>(sclass.getConstructor()), params),
		sclass(sclass)
	{
		if (sclass.isAbstract())
			throw CompileException(std::string("Cannot instatiate abstract class ") + sclass.getName());
	}

	ScriptValue *ScriptNew::run(const CALLABLE_PARAMS_T &c)
	{
		ScriptClassInstance *classInstance = new ScriptClassInstance(this->sclass);
		ScriptClassValue *val = new ScriptClassValue(classInstance);

		this->sclass.initialize(*classInstance);

		if (this->func && this->func->get())
		{
			this->setInstance(val);
			ScriptFuncCall::run(c);
		}

		return val;
	}
}