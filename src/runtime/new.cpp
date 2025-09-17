#include "runtime/new.hpp"
#include "runtime/class.hpp"
#include "runtime/classInstance.hpp"
#include "compileException.hpp"

namespace gscript
{
	ScriptNew::ScriptNew(ScriptClass &sclass, std::vector<std::unique_ptr<ScriptStatement>>&& params)
		//:ScriptFuncCall(sclass, sclass.getConstructor(), params),
		: ScriptFuncCall(sclass, sclass.getConstructor(), std::move(params))
		, sclass(sclass)
	{
		if (sclass.isAbstract())
			throw CompileException(std::string("Cannot instatiate abstract class ") + sclass.getName());
	}

	std::unique_ptr<ScriptValue> ScriptNew::run(const CALLABLE_PARAMS_T &c)
	{
		auto classInstance = std::make_unique<ScriptClassInstance>(this->sclass);
		this->sclass.initialize(*classInstance);

		auto val = std::make_unique<ScriptClassValue>(std::move(classInstance));

		/*if (this->func && this->func->get())
		{*/
		this->setInstance(ScriptReferenceValue::create(val.get()));
		ScriptFuncCall::run(c);
		//}

		return val;
	}
}