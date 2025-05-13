#include "statement.hpp"

#include "pVar.hpp"
#include "pLiteral.hpp"
#include "pFuncCall.hpp"
#include "pOperator.hpp"
#include "pNew.hpp"
#include "pArrayInitializer.hpp"
#include "pArrayAccessor.hpp"
#include "pStatement.hpp"

#include "varRead.hpp"
#include "funcCall.hpp"
#include "script.hpp"
#include "literal.hpp"
#include "operator.hpp"
#include "new.hpp"
#include "variable.hpp"
#include "type.hpp"
#include "function.hpp"
#include "arrayInitializer.hpp"
#include "namespace.hpp"
#include "globalNamespace.hpp"
#include "class.hpp"

#include "compileException.hpp"

#include "debug.hpp"

#include <iostream>
#include <cassert>

namespace gscript
{
	ScriptStatement::ScriptStatement(ScriptScope &scope)
		:ScriptCallable(scope)
	{
	}

	ScriptStatement::ScriptStatement(ScriptScope &scope, const ParserStatement &pstatement)
		: ScriptCallable(scope)
	{
		std::vector<ScriptCallable*> temp;
		temp.reserve(pstatement.components.size());
		int currentIndex = 0;

		for (
			ParserStatement::CONTAINER_T::const_iterator it = pstatement.components.begin();
			it != pstatement.components.end();
			++it
			)
		{
			if (std::shared_ptr<ParserVar> varread = std::dynamic_pointer_cast<ParserVar>(*it))
			{
				if (ParserArrayAccessor *arr = varread->arrayAccessor)
					temp.push_back(new ScriptArrayReadResolv(this->scope, new ScriptStatement(this->scope, arr->statement), varread->name));
				else
					temp.push_back(new ScriptVarReadResolv(this->scope, varread->name));
			}
			else if (std::shared_ptr<ParserLiteral> lit = std::dynamic_pointer_cast<ParserLiteral>(*it))
			{
				temp.push_back(new ScriptLiteral(this->scope, *lit));
			}
			else if (std::shared_ptr<ParserStatement> stmt = std::dynamic_pointer_cast<ParserStatement>(*it))
			{
				temp.push_back(new ScriptStatement(this->scope, *stmt));
			}
			else if (std::shared_ptr<ParserFuncCall> fcall = std::dynamic_pointer_cast<ParserFuncCall>(*it))
			{
				FunctionParameterContainer params(this->scope, fcall->arglist);

				if (std::shared_ptr<ParserNew> pnew = std::dynamic_pointer_cast<ParserNew>(*it))
				{
					ScriptNamespace *ns = this->scope.getGlobalNamespace();
					ScriptClass *sclass = ns->findClass(pnew->constructorName);

					if (!sclass)
						throw CompileException(std::string("Unable to find class \"" + pnew->constructorName + "\""));

					temp.push_back(new ScriptNew(*sclass, params.getParams()));
				}
				else
				{
					ScriptScope *usedScope = &this->scope;
					std::string usedName = fcall->name;

					if (fcall->name.isScoped())
					{
						EntityPath::scope_iterator scopeIt = fcall->name.createScopeIterator();

						ScriptNamespace *ns = this->scope.getGlobalNamespace();
						for (; scopeIt != fcall->name.getString().end(); ++scopeIt)
						{
							if (scopeIt.last())
							{
								usedName = *scopeIt;
							}
							else
							{
								ScriptNamespace *foundns = ns->findClass(*scopeIt);

								if (!foundns)
								{
									foundns = ns->findNamespace(*scopeIt);

									if (!foundns)
										throw CompileException(std::string("Class \"" + *scopeIt + "\" was not found"));
								}

								usedScope = foundns;
							}
						}
					}

					ScriptFuncCallResolv *funcCall = new ScriptFuncCallResolv(*usedScope, params.getParams(), usedName, params.getParamTypes(), fcall->name.isScoped());
					temp.push_back(funcCall);
				}
			}
			else if (std::shared_ptr<ParserArrayInitializer> arr = std::dynamic_pointer_cast<ParserArrayInitializer>(*it))
			{
				temp.push_back(new ScriptArrayInitializer(this->scope, *arr));
			}
			else if (std::shared_ptr<ParserOperator> oper = std::dynamic_pointer_cast<ParserOperator>(*it))
			{
				temp.push_back(oper->createOperator(this->scope));
			}
			else
				throw CompileException("Unknown entity type");
		}

		int countProcessed = this->resolveOperations(temp.rbegin(), temp.rend(), this->callable);

		this->assignReferences(this->callable, &this->scope, false);

		if (ScriptOperator *oper = dynamic_cast<ScriptOperator*>(this->callable))
			this->setupOperator(oper);
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

	void ScriptStatement::setupOperator(ScriptOperator *oper)
	{
		if (oper->needFactory())
			oper->assignOperatorFunction();

		if (ScriptOperator *left = dynamic_cast<ScriptOperator*>(oper->left))
			this->setupOperator(left);

		if (ScriptOperator *right = dynamic_cast<ScriptOperator*>(oper->right))
			this->setupOperator(right);

		oper->setup();
	}

	void ScriptStatement::assignReferences(ScriptCallable *&entry, ScriptScope *scope, bool member)
	{
		if (ScriptFuncCallResolv *fcall = dynamic_cast<ScriptFuncCallResolv*>(entry))
		{
			entry = fcall->resolve(scope);
			ScriptFuncCall *fc = static_cast<ScriptFuncCall*>(entry);

			if (ScriptMethod *method = dynamic_cast<ScriptMethod*>(fc->func))
			{
				if (!method->isAccessible(this->scope, method->accessModifier))
					throw CompileException("Access denied to \"" + method->getName() + "\" method");
			}
		}

		else if (ScriptArrayReadResolv *vread = dynamic_cast<ScriptArrayReadResolv*>(entry))
		{
			entry = vread->resolve(*scope, member);
		}

		else if (ScriptVarReadResolv *vread = dynamic_cast<ScriptVarReadResolv*>(entry))
		{
			entry = vread->resolve(*scope, member);
		}

		else if (ScriptOperator *oper = dynamic_cast<ScriptOperator*>(entry))
		{
			this->assignReferences(oper->left, scope, false);

			ScriptScope *resolvedScope = scope;

			bool memberAccess = false;
			if (ScriptOperatorMemberAccessor *operAccessor = dynamic_cast<ScriptOperatorMemberAccessor*>(oper))
			{
				memberAccess = true;

				if (ScriptVarRead *leftVar = dynamic_cast<ScriptVarRead*>(oper->left))
				{
					if (const ScriptClassType *leftClass = dynamic_cast<const ScriptClassType*>(leftVar->var->orig()->getType()))
						resolvedScope = &leftClass->sclass;
				}
				else if (ScriptFuncCall *leftFunc = dynamic_cast<ScriptFuncCall*>(oper->left))
				{
					if (leftFunc->func->get()->getType()->getTypeDescriptor() == VALUE_TYPE_T::VT_CLASS)
					{
						const ScriptClassType *leftClassType = static_cast<const ScriptClassType*>(leftFunc->func->get()->getType());
						resolvedScope = &leftClassType->sclass;
					}
				}
				else if (ScriptOperatorMemberAccessor *leftMemb = dynamic_cast<ScriptOperatorMemberAccessor*>(oper->left))
				{
					if (const ScriptVarRead *leftMembVar = dynamic_cast<const ScriptVarRead*>(leftMemb->right))
					{
						resolvedScope = &static_cast<const ScriptClassType*>(leftMembVar->getType())->sclass;
					}
					else if (const ScriptFuncCall *leftMembFunc = dynamic_cast<const ScriptFuncCall*>(leftMemb->right))
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
		std::vector<ScriptCallable*>::reverse_iterator begin,
		std::vector<ScriptCallable*>::reverse_iterator end,
		ScriptCallable *&result,
		unsigned int depth
	)
	{
		if (begin == end)
		{
			result = new ScriptLiteral(this->scope, Script::SCR_NULL);
			return 0;
		}

		typedef std::vector<ScriptCallable*>::reverse_iterator ITER_T;

		bool firstOperator = true;

		ITER_T operatorIt;
		ScriptOperator *oper = NULL;

		for (operatorIt = begin; operatorIt != end; ++operatorIt)
		{
			if (ScriptOperator *o = dynamic_cast<ScriptOperator*>(*operatorIt))
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

		ScriptOperator *firstOper = oper;
		ScriptOperator *prevOper = oper;
		ScriptOperator *resultOper = NULL;

		int prec = oper->getPrecedence();
		int prevPrec = prec;

		ScriptCallable *subResult = NULL;
		
		if (static_cast<int>(oper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_LEFT))
			oper->left = *(operatorIt + 1);

		if (static_cast<int>(oper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_RIGHT))
			oper->right = *(operatorIt - 1);

		while (true)
		{
			oper = NULL;

			while (!oper && operatorIt + 1 != end)
			{
				oper = dynamic_cast<ScriptOperator*>(*(++operatorIt));
			}

			if (oper)
			{
				int currentPrec = oper->getPrecedence();

				if (currentPrec == prevPrec)
				{
					//if (static_cast<int>(oper->linkage) & ~static_cast<int>(OPERATOR_LINK_T::OL_LEFT) & static_cast<int>(OPERATOR_LINK_T::OL_RIGHT))
					if (subResult)
					{
						//if (static_cast<int>(oper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_LEFT))
						if (static_cast<int>(oper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_RIGHT))
							oper->right = subResult;
						subResult = NULL;
					}
					else
					{
						//if (static_cast<int>(oper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_LEFT))
						if (static_cast<int>(oper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_RIGHT))
							oper->right = *(operatorIt - 1);
					}

					//if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_RIGHT))
					if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_LEFT))
						prevOper->left = oper;

					prevOper = oper;
					prevPrec = oper->getPrecedence();
				}
				else if (currentPrec > prevPrec)
				{
					if (depth > 0)
					{
						//if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_RIGHT))
						if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_LEFT))
							prevOper->left = *(operatorIt - 1);

						result = firstOper->linkage == OPERATOR_LINK_T::OL_BOTH ? firstOper : oper;

						return operatorIt - begin;
					}
					else
					{
						//if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_LEFT))
						//if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_RIGHT))
							//oper->right = firstOper;

						if (static_cast<int>(oper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_RIGHT))
							oper->right = prevOper;

						//if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_RIGHT))
						if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_LEFT))
							prevOper->left = *(operatorIt - 1);

						resultOper = oper;

						prevOper = oper;
						prevPrec = oper->getPrecedence();
					}
				}
				else if (currentPrec < prevPrec)
				{
					ScriptCallable *sr = nullptr;
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
					//if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_RIGHT))
					if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_LEFT))
						prevOper->left = subResult;

					subResult = NULL;
				}
				else if (prevOper != *operatorIt)
				{
					//if (static_cast<int>(prevOper->linkage) & static_cast<int>(OPERATOR_LINK_T::OL_RIGHT))
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