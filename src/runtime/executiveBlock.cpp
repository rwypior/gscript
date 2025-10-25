#include "runtime/executiveBlock.hpp"
#include "runtime/return.hpp"
#include "runtime/if.hpp"
#include "util.hpp"

namespace gscript
{
	ScriptExecutiveBlock::ScriptExecutiveBlock(std::vector<std::unique_ptr<ScriptCallable>>&& statements)
		: statements(std::move(statements))
	{
	}

	ScriptExecutiveBlock::ScriptExecutiveBlock(ScriptExecutiveBlock&& exeblock) noexcept
		: statements(std::move(exeblock.statements))
	{
	}

	ScriptExecutiveBlock::ScriptExecutiveBlock(const ScriptExecutiveBlock& exeblock)
		: statements(cloneVector(exeblock.statements))
	{
	}

	ScriptExecutiveBlock::~ScriptExecutiveBlock()
	{
	}

	std::unique_ptr<ScriptValue> ScriptExecutiveBlock::execute(ScriptScopeBase& scope)
	{
		for (auto& stmt : this->statements)
		{
			auto ret = stmt->run(scope);
			if (ret && ret->isReturnedData())
			{
				return ret;
			}
		}

		return ScriptType::null();
	}

	void ScriptExecutiveBlock::merge(std::vector<std::unique_ptr<ScriptCallable>>&& statements)
	{
		this->statements.insert(this->statements.end(), std::make_move_iterator(statements.begin()), std::make_move_iterator(statements.end()));
	}

	void ScriptExecutiveBlock::merge(ScriptExecutiveBlock&& block)
	{
		this->merge(std::move(block.statements));
	}

	void ScriptExecutiveBlock::merge(std::unique_ptr<ScriptExecutiveBlock>&& block)
	{
		this->merge(std::move(*block));
	}

	std::vector<std::unique_ptr<ScriptCallable>>& ScriptExecutiveBlock::getStatements()
	{
		return this->statements;
	}
}