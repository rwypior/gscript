#include "runtime/statement.hpp"
#include "runtime/varRead.hpp"
#include "runtime/funcCall.hpp"
#include "runtime/literal.hpp"
#include "runtime/operator.hpp"
#include "runtime/new.hpp"
#include "runtime/variable.hpp"
#include "runtime/arrayInitializer.hpp"
#include "runtime/namespace.hpp"
#include "runtime/globalNamespace.hpp"
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
	ScriptStatement::ScriptStatement(ScriptScope &scope)
		: ScriptCallable(scope)
	{
	}

	ScriptStatement::ScriptStatement(ScriptScope& scope, std::vector<std::unique_ptr<ScriptCallable>>&& callables)
		: ScriptCallable(scope)
	{
		std::vector<std::shared_ptr<ScriptCallable>> sharedCallables;
		sharedCallables.resize(callables.size());

		std::transform(callables.begin(), callables.end(), sharedCallables.begin(), [](std::unique_ptr<ScriptCallable>& c) { return std::move(c); });

		this->resolveOperations(sharedCallables.rbegin(), sharedCallables.rend(), this->callable);

		this->assignReferences(this->callable, &this->scope, false);

		if (auto oper = std::dynamic_pointer_cast<ScriptOperator>(this->callable))
			this->setupOperator(oper);
	}

	ScriptStatement::ScriptStatement(ScriptStatement&& stmt)
		: ScriptCallable(stmt.scope)
		, callable(std::move(stmt.callable))
	{
	}

	ScriptValue *ScriptStatement::run(const CALLABLE_PARAMS_T &c)
	{
		ScriptValue *result = this->callable->run();
		return result;
	}

	const ScriptType *ScriptStatement::getType() const
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

	void ScriptStatement::assignReferences(std::shared_ptr<ScriptCallable>& entry, ScriptScope *scope, bool member)
	{
		if (auto fcall = dynamic_cast<ScriptFuncCallResolver*>(entry.get()))
		{
			entry = fcall->resolve(scope);
			auto fc = std::static_pointer_cast<ScriptFuncCall>(entry);

			if (ScriptMethod *method = dynamic_cast<ScriptMethod*>(fc->getFunc()))
			{
				if (!method->isAccessible(this->scope, method->accessModifier))
					throw CompileException("Access denied to \"" + method->getName() + "\" method");
			}
		}

		else if (auto vread = dynamic_cast<ScriptArrayReadResolver*>(entry.get()))
		{
			entry = vread->resolve(*scope, member);
		}

		else if (auto vread = dynamic_cast<ScriptVarReadResolver*>(entry.get()))
		{
			entry = vread->resolve(*scope, member);
		}

		else if (auto oper = dynamic_cast<ScriptOperator*>(entry.get()))
		{
			this->assignReferences(oper->left, scope, false);

			ScriptScope *resolvedScope = scope;

			bool memberAccess = false;
			if (ScriptOperatorMemberAccessor *operAccessor = dynamic_cast<ScriptOperatorMemberAccessor*>(oper))
			{
				memberAccess = true;

				if (auto leftVar = std::dynamic_pointer_cast<ScriptVarRead>(oper->left))
				{
					if (const ScriptClassType *leftClass = dynamic_cast<const ScriptClassType*>(leftVar->var->orig()->getType()))
						resolvedScope = &leftClass->sclass;
				}
				else if (auto leftFunc = std::dynamic_pointer_cast<ScriptFuncCall>(oper->left))
				{
					if (leftFunc->getFunc()->get()->getType()->getTypeDescriptor() == VALUE_TYPE_T::VT_CLASS)
					{
						const ScriptClassType *leftClassType = static_cast<const ScriptClassType*>(leftFunc->getFunc()->get()->getType());
						resolvedScope = &leftClassType->sclass;
					}
				}
				else if (auto leftMemb = std::dynamic_pointer_cast<ScriptOperatorMemberAccessor>(oper->left))
				{
					if (const auto leftMembVar = std::dynamic_pointer_cast<const ScriptVarRead>(leftMemb->right))
					{
						resolvedScope = &static_cast<const ScriptClassType*>(leftMembVar->getType())->sclass;
					}
					else if (const auto leftMembFunc = std::dynamic_pointer_cast<const ScriptFuncCall>(leftMemb->right))
					{
						resolvedScope = &static_cast<const ScriptClassType*>(leftMembFunc->getType())->sclass;
					}
					else
						throw CompileException("Casting member accessor on incompatible entity");
				}
				else
					throw CompileException("Casting member accessor on incompatible entity");
			}

			this->assignReferences(oper->right, resolvedScope, memberAccess);
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
			result = std::make_shared<ScriptLiteral>(this->scope, nullptr);
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
		
		if (static_cast<int>(oper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_LEFT))
			oper->left = *(operatorIt + 1);

		if (static_cast<int>(oper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_RIGHT))
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
						if (static_cast<int>(oper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_RIGHT))
							oper->right = subResult;
						subResult = NULL;
					}
					else
					{
						if (static_cast<int>(oper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_RIGHT))
							oper->right = *(operatorIt - 1);
					}

					if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_LEFT))
						prevOper->left = oper;

					prevOper = oper;
					prevPrec = oper->getPrecedence();
				}
				else if (currentPrec > prevPrec)
				{
					if (depth > 0)
					{
						if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_LEFT))
							prevOper->left = *(operatorIt - 1);

						result = firstOper->linkage == OPERATOR_LINK_T::OL_BOTH ? firstOper : oper;

						return operatorIt - begin;
					}
					else
					{
						if (static_cast<int>(oper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_RIGHT))
							oper->right = prevOper;

						if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_LEFT))
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
					if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_LEFT))
						prevOper->left = subResult;

					subResult = NULL;
				}
				else if (prevOper != *operatorIt)
				{
					if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_LEFT))
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