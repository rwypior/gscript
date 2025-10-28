#include "runtime/statement.hpp"
#include "runtime/varRead.hpp"
#include "runtime/funcCall.hpp"
#include "runtime/literal.hpp"
#include "runtime/operator.hpp"
#include "runtime/new.hpp"
#include "runtime/variable.hpp"
#include "runtime/arrayInitializer.hpp"
#include "runtime/namespace.hpp"
#include "runtime/class.hpp"
#include "runtime/function.hpp"

#include "type.hpp"
#include "compileException.hpp"
#include "debug.hpp"

#include <iostream>
#include <cassert>
#include <algorithm>

namespace gscript
{
	ScriptStatement::ScriptStatement(const ScriptStatement& b)
		: callable(b.callable->clone())
	{
	}

	ScriptStatement::ScriptStatement(std::vector<std::unique_ptr<ScriptCallable>>&& callables)
	{
		std::vector<std::shared_ptr<ScriptCallable>> sharedCallables;
		sharedCallables.reserve(callables.size());

		for (auto& callable : callables)
		{
			sharedCallables.push_back(std::move(callable));
		}

		this->resolveOperations(sharedCallables.rbegin(), sharedCallables.rend(), this->callable);
	}

	ScriptStatement::ScriptStatement(ScriptStatement&& stmt) noexcept
		: callable(std::move(stmt.callable))
	{
	}

	std::unique_ptr<ScriptCallable> ScriptStatement::clone()
	{
		return std::make_unique<ScriptStatement>(*this);
	}

	std::unique_ptr<ScriptValue> ScriptStatement::run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c)
	{
		return this->callable->run(scope);
	}

	void ScriptStatement::setup(ScriptScopeBase& scope)
	{
		this->assignReferences(this->callable, scope, false);
		if (auto oper = std::dynamic_pointer_cast<ScriptOperator>(this->callable))
			this->setupOperator(oper);
	}

	const std::shared_ptr<ScriptType> ScriptStatement::getType() const
	{
		return this->callable->getType();
	}

	void ScriptStatement::setupOperator(std::shared_ptr<ScriptOperator> oper)
	{
		if (oper->needFactory())
			oper->assignOperatorFunction();

		if (auto left = std::dynamic_pointer_cast<ScriptOperator>(oper->left))
			this->setupOperator(left);

		if (auto right = std::dynamic_pointer_cast<ScriptOperator>(oper->right))
			this->setupOperator(right);

		oper->setup();
	}

	void ScriptStatement::assertAccessibilityOf(const ScriptScopeBase& scope, ScriptMethod& method) const
	{
		if (!method.isAccessible(scope, method.accessModifier))
			throw CompileException("Method \"" + method.getName() + "\" is inaccessible");
	}

	void ScriptStatement::assignReferences(std::shared_ptr<ScriptCallable>& entry, ScriptScopeBase& scope, bool member)
	{
		if (auto fcall = std::dynamic_pointer_cast<ScriptCallablePrototype>(entry))
		{
			entry = fcall->build(scope);
		}

		if (auto fcall = std::dynamic_pointer_cast<ScriptFuncCall>(entry))
		{
			if (ScriptMethod* method = dynamic_cast<ScriptMethod*>(fcall->getFunc().get()))
				assertAccessibilityOf(scope, *method);
		}

		else if (auto oper = dynamic_cast<ScriptOperator*>(entry.get()))
		{
			this->assignReferences(oper->left, scope, false);

			ScriptScopeBase* resolvedScope = &scope;

			bool memberAccess = false;
			if (ScriptOperatorMemberAccessor *operAccessor = dynamic_cast<ScriptOperatorMemberAccessor*>(oper))
			{
				memberAccess = true;

				if (auto leftVar = std::dynamic_pointer_cast<ScriptVarRead>(oper->left))
				{
					// TODO - check this
					//if (const ScriptClassType *leftClass = dynamic_cast<const ScriptClassType*>(leftVar->var->orig()->getType()))
					if (const auto leftClass = std::dynamic_pointer_cast<const ScriptClassType>(leftVar->getType()))
						resolvedScope = &leftClass->getClass();
				}
				else if (auto leftFunc = std::dynamic_pointer_cast<ScriptFuncCall>(oper->left))
				{
					auto type = leftFunc->getFunc().getType();
					if (type->getTypeDescriptor() == ValueType::Class)
					{
						const auto leftClassType = std::static_pointer_cast<const ScriptClassType>(type);
						resolvedScope = &leftClassType->getClass();
					}
				}
				else if (auto leftMemb = std::dynamic_pointer_cast<ScriptOperatorMemberAccessor>(oper->left))
				{
					if (const auto leftMembVar = std::dynamic_pointer_cast<const ScriptVarRead>(leftMemb->right))
					{
						resolvedScope = &std::static_pointer_cast<const ScriptClassType>(leftMembVar->getType())->getClass();
					}
					else if (const auto leftMembFunc = std::dynamic_pointer_cast<const ScriptFuncCall>(leftMemb->right))
					{
						resolvedScope = &std::static_pointer_cast<const ScriptClassType>(leftMembFunc->getType())->getClass();
					}
					else
						throw CompileException("Casting member accessor on incompatible entity");
				}
				else
					throw CompileException("Casting member accessor on incompatible entity");
			}

			this->assignReferences(oper->right, *resolvedScope, memberAccess);
		}
	}

	int ScriptStatement::resolveOperations(
		std::vector<std::shared_ptr<ScriptCallable>>::reverse_iterator begin,
		std::vector<std::shared_ptr<ScriptCallable>>::reverse_iterator end,
		std::shared_ptr<ScriptCallable> &result,
		unsigned int depth
	)
	{
		if (begin == end)
		{
			result = std::make_shared<ScriptLiteral>(nullptr);
			return 0;
		}

		bool firstOperator = true;

		decltype(begin) operatorIt;
		std::shared_ptr<ScriptOperator> oper = nullptr;

		for (operatorIt = begin; operatorIt != end; ++operatorIt)
		{
			if (auto o = std::dynamic_pointer_cast<ScriptOperator>(*operatorIt))
			{
				oper = o;
				break;
			}

			firstOperator = false;
		}

		if (!oper)
		{
			result = *begin;
			return operatorIt - begin;
		}

		std::shared_ptr<ScriptOperator> firstOper = oper;
		std::shared_ptr<ScriptOperator> prevOper = oper;
		std::shared_ptr<ScriptOperator> resultOper = nullptr;

		int prec = oper->getPrecedence();
		int prevPrec = prec;

		std::shared_ptr<ScriptCallable> subResult = nullptr;
		
		if (static_cast<int>(oper->linkage) & static_cast<int>(OperatorLinkage::Left))
			oper->left = *(operatorIt + 1);

		if (static_cast<int>(oper->linkage) & static_cast<int>(OperatorLinkage::Right))
			oper->right = *(operatorIt - 1);

		while (true)
		{
			oper = NULL;

			while (!oper && operatorIt + 1 != end)
			{
				oper = std::dynamic_pointer_cast<ScriptOperator>(*(++operatorIt));
			}

			if (oper)
			{
				int currentPrec = oper->getPrecedence();

				if (currentPrec == prevPrec)
				{
					if (subResult)
					{
						if (static_cast<int>(oper->linkage) & static_cast<int>(OperatorLinkage::Right))
							oper->right = subResult;
						subResult = NULL;
					}
					else
					{
						if (static_cast<int>(oper->linkage) & static_cast<int>(OperatorLinkage::Right))
							oper->right = *(operatorIt - 1);
					}

					if (static_cast<int>(prevOper->linkage) & static_cast<int>(OperatorLinkage::Left))
						prevOper->left = oper;

					prevOper = oper;
					prevPrec = oper->getPrecedence();
				}
				else if (currentPrec > prevPrec)
				{
					if (depth > 0)
					{
						if (static_cast<int>(prevOper->linkage) & static_cast<int>(OperatorLinkage::Left))
							prevOper->left = *(operatorIt - 1);

						result = firstOper->linkage == OperatorLinkage::Both ? firstOper : oper;

						return operatorIt - begin;
					}
					else
					{
						if (static_cast<int>(oper->linkage) & static_cast<int>(OperatorLinkage::Right))
							oper->right = prevOper;

						if (static_cast<int>(prevOper->linkage) & static_cast<int>(OperatorLinkage::Left))
							prevOper->left = *(operatorIt - 1);

						resultOper = oper;

						prevOper = oper;
						prevPrec = oper->getPrecedence();
					}
				}
				else if (currentPrec < prevPrec)
				{
					std::shared_ptr<ScriptCallable> sr = nullptr;
					int subProcessed = this->resolveOperations(operatorIt, end, sr, depth + 1);
					if (subProcessed)
					{
						operatorIt += subProcessed - 1;
						subResult = sr;
					}
				}
			}
			else
			{
				if (subResult)
				{
					if (static_cast<int>(prevOper->linkage) & static_cast<int>(OperatorLinkage::Left))
						prevOper->left = subResult;

					subResult = NULL;
				}
				else if (prevOper != *operatorIt)
				{
					if (static_cast<int>(prevOper->linkage) & static_cast<int>(OperatorLinkage::Left))
						prevOper->left = *operatorIt;
				}

				result = resultOper ? resultOper : firstOper;

				return operatorIt - begin;
			}
		}

		assert(false);
		return -1;
	}
}