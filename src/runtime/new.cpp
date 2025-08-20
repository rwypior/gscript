#include "runtime/new.hpp"
#include "runtime/class.hpp"
#include "runtime/classInstance.hpp"
#include "compileException.hpp"

namespace gscript
{
	ScriptNew::ScriptNew(ScriptClass &sclass, std::vector<std::unique_ptr<ScriptStatement>>&& params)
		//:ScriptFuncCall(sclass, sclass.getConstructor(), params),
		: ScriptFuncCall(sclass, new DirectEntityLink<ScriptFunction*>(sclass.getConstructor()), std::move(params))
		, sclass(sclass)
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