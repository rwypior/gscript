#include "runtime/new.hpp"
#include "runtime/class.hpp"
#include "runtime/classInstance.hpp"
#include "compileException.hpp"

namespace gscript
{
	ScriptNew::ScriptNew(const ScriptNew& b)
		: ScriptFuncCall(b)
		, sclass(b.sclass)
	{
	}

	ScriptNew::ScriptNew(ScriptClass &sclass, std::vector<std::unique_ptr<ScriptStatement>>&& params)
		//:ScriptFuncCall(sclass, sclass.getConstructor(), params),
		: ScriptFuncCall(sclass, sclass.getConstructor(), std::move(params))
		, sclass(sclass)
	{
		if (sclass.isAbstract())
			throw CompileException(std::string("Cannot instatiate abstract class ") + sclass.getName());
	}

	std::unique_ptr<ScriptCallable> ScriptNew::clone()
	{
		return std::make_unique<ScriptNew>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptNew::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		auto classInstance = std::make_unique<ScriptClassInstance>(this->sclass);
		this->sclass.initialize(*classInstance);

		auto val = std::make_unique<ScriptClassValue>(std::move(classInstance));

		/*if (this->func && this->func->get())
		{*/
		this->setInstance(ScriptReferenceValue::create(val.get()));
		ScriptFuncCall::run(scope, c);
		//}

		return val;
	}
}