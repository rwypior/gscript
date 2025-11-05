#include "gscript/runtime/arrayInitializer.hpp"
#include "gscript/compileException.hpp"
#include "gscript/util.hpp"

namespace gscript
{
	ScriptArrayInitializer::ScriptArrayInitializer(const ScriptArrayInitializer& b)
		: type(b.type->clone())
		, statements(cloneVector(b.statements))
	{
	}

	ScriptArrayInitializer::ScriptArrayInitializer(std::vector<std::unique_ptr<ScriptStatement>>&& statements)
		: statements(std::move(statements))
		, type(statements.empty() ? ScriptType::createVoid() : statements.front()->getType())
	{
	}

	std::unique_ptr<ScriptCallable> ScriptArrayInitializer::clone()
	{
		return std::make_unique<ScriptArrayInitializer>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptArrayInitializer::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		std::vector<std::unique_ptr<ScriptValue>> vec;
		vec.reserve(this->statements.size());

		for (auto& stmt : this->statements)
		{
			vec.push_back(stmt->run(scope));
		}

		return std::make_unique<ScriptArrayValue>(this->getType(), std::move(vec));
	}

	const std::shared_ptr<ScriptType> ScriptArrayInitializer::getType() const
	{
		return this->type;
	}
}