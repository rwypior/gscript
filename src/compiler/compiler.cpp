#include "compiler/compiler.hpp"

#include "parser/pStatement.hpp"
#include "parser/pCallArglist.hpp"
#include "parser/pBlockBody.hpp"
#include "parser/pNamespace.hpp"
#include "parser/pFunction.hpp"
#include "parser/pLiteral.hpp"
#include "parser/pVar.hpp"
#include "parser/pFuncCall.hpp"
#include "parser/pNew.hpp"
#include "parser/pArrayInitializer.hpp"
#include "parser/pOperator.hpp"
#include "parser/pArrayAccessor.hpp"
#include "parser/pReturn.hpp"
#include "parser/pIf.hpp"
#include "parser/pWhile.hpp"
#include "parser/pFor.hpp"
#include "parser/pReturn.hpp"

#include "runtime/statement.hpp"
#include "runtime/funcParam.hpp"
#include "runtime/runnable.hpp"
#include "runtime/namespace.hpp"
#include "runtime/class.hpp"
#include "runtime/varDeclaration.hpp"
#include "runtime/function.hpp"
#include "runtime/literal.hpp"
#include "runtime/varRead.hpp"
#include "runtime/new.hpp"
#include "runtime/arrayInitializer.hpp"
#include "runtime/operator.hpp"
#include "runtime/if.hpp"
#include "runtime/while.hpp"
#include "runtime/for.hpp"
#include "runtime/return.hpp"

#include "runtimeException.hpp"
#include "compileException.hpp"

#include <unordered_map>
#include <functional>
#include <memory>

namespace gscript
{
	std::unique_ptr<FunctionParameterContainer> Compiler::compileParameterContainer(ScriptScope* scope, const ParserCallArglist& arglist)
	{
		std::vector<std::unique_ptr<ScriptStatement>> statements;
		statements.reserve(arglist.parameters.size());
		for (auto& param : arglist.parameters)
		{
			statements.emplace_back(this->compileStatement(scope, *param));
		}
		return std::make_unique<FunctionParameterContainer>(std::move(statements));
	}

	std::unique_ptr<ScriptNamespace> Compiler::compileNamespace(ScriptScope* scope, const ParserNamespace& entry)
	{
		auto ns = std::make_unique<ScriptNamespace>(scope, entry.name);

		for (auto& cls : entry.classes)
		{
			//ns->registerClassPrototype(cls);
			ns->registerClass(this->compileClass(ns.get(), cls));
		}

		//ns->resolveClasses(); // TODO

		for (auto& fnc : entry.functions)
		{
			ns->registerFunction(this->compileFunction(scope, fnc));
			//ns->registerFunctionPrototype(fnc);
		}

		ns->resolveFunctions();
		ns->resolveClassMembers();

		return ns;
	}

	std::unique_ptr<ScriptClass> Compiler::compileClass(ScriptNamespace* scope, const ParserClass& pclass)
	{
		const std::string& base = pclass.base;

		// TODO - clients of this method must check if class already exists
		//if (ScriptClass* existing = parentScope->findClass(pclass.name))
		//	return existing;

		ScriptClass* baseClass = nullptr;

		if (base.length() > 0)
		{
			baseClass = scope->findClass(base);
		}

		auto cl = std::make_unique<ScriptClass>(*scope, pclass.name, baseClass);
		//parentScope->registerClass(cl); // TODO - register where needed
		
		//parentScope->classes.push_back(cl);
		//cl->parentScope = parentScope;

		//for (std::vector<ParserFieldDeclaration>::const_iterator it = pclass.fields.begin(); it != pclass.fields.end(); ++it)
		for (auto& pf : pclass.fields)
		{
			ScriptVariable& svar = scope->registerVariable(std::make_unique<ScriptVariable>(pf.name, ScriptType::create(pf.type, *cl), nullptr, cl->getVariables().size()));
			//ScriptVariable& svar = cl->registerVariable(pf.name, ScriptType::create(pf.type, *cl), NULL);
			//ScriptVarDeclaration* svd = new ScriptVarDeclaration(*cl, svar, ScriptStatement(*cl, it->value));
			ScriptVarDeclaration* svd = new ScriptVarDeclaration(*cl, svar, this->compileStatement(cl.get(), pf.value));
			cl->addVarDeclaration(svd);

			//cl->registerVariable(it->name, ScriptType::create(it->type), NULL);
		}

		for (auto& pmethod : pclass.methods)
		{
			// TODO
			//auto method = this->compileMethod(scope, *pmethod);
			//cl->registerFunction(std::move(method));
		}

		return cl;
	}

	std::unique_ptr<ScriptFunction> Compiler::compileFunction(ScriptScope* scope, const ParserFunction& pfunc)
	{
		PARAMS_T params;
		for (auto& pp : pfunc.arglist.parameters)
		{
			params.emplace_back(ScriptType::create(pp.type, *scope), pp.name);
		}

		// TODO - statements in executable block need scope, and this scope
		// will be a function that is yet to be created

		auto newfunc = std::make_unique<ScriptFunction>(*scope, pfunc.name, ScriptType::create(pfunc.returnTypeName, *scope), params);
		newfunc->merge(std::move(this->compileExecutiveBlock(newfunc.get(), pfunc.body)));

		return newfunc;
	}

	std::unique_ptr<ScriptMethod> Compiler::compileMethod(ScriptScope* scope, const ParserMethod& pfunc)
	{
		return nullptr;
	}

	std::unique_ptr<ScriptValue> Compiler::compileValue(const ParserLiteral& pLiteral)
	{
		switch (pLiteral.type)
		{
		case VALUE_TYPE_T::VT_INT:
			return std::make_unique<ScriptIntValue>(std::stoi(pLiteral.value));
		case VALUE_TYPE_T::VT_STRING:
			return std::make_unique<ScriptStringValue>(pLiteral.value);
		case VALUE_TYPE_T::VT_FLOAT:
			return std::make_unique<ScriptFloatValue>(std::stof(pLiteral.value));
		case VALUE_TYPE_T::VT_DOUBLE:
			return std::make_unique<ScriptDoubleValue>(std::stod(pLiteral.value));
		case VALUE_TYPE_T::VT_CHAR:
			return std::make_unique<ScriptCharValue>(pLiteral.value[0]);
		}

		throw RuntimeException(std::string("Unknown type ID ") + std::to_string(static_cast<size_t>(pLiteral.type)));
	}

	std::unique_ptr<ScriptStatement> Compiler::compileStatement(ScriptScope* scope, const ParserStatement& pstatement)
	{
		std::vector<std::unique_ptr<ScriptCallable>> substatements;
		substatements.reserve(pstatement.components.size());
		int currentIndex = 0;

		for (auto& comp : pstatement.components)
		{
			if (auto varread = std::dynamic_pointer_cast<ParserVar>(comp))
			{
				substatements.push_back(this->compileVarRead(scope, *varread));
			}
			else if (auto lit = std::dynamic_pointer_cast<ParserLiteral>(comp))
			{
				substatements.push_back(std::make_unique<ScriptLiteral>(*scope, this->compileValue(*lit)));
			}
			else if (auto stmt = std::dynamic_pointer_cast<ParserStatement>(comp))
			{
				substatements.push_back(this->compileStatement(scope, *stmt));
			}
			else if (auto fcall = std::dynamic_pointer_cast<ParserNew>(comp))
			{
				substatements.push_back(this->compileNewCall(scope, *fcall));
			}
			else if (auto fcall = std::dynamic_pointer_cast<ParserFuncCall>(comp))
			{
				substatements.push_back(this->compileFuncCall(scope, *fcall));
			}
			else if (auto arr = std::dynamic_pointer_cast<ParserArrayInitializer>(comp))
			{
				substatements.push_back(this->compileArrayInitializer(scope, *arr));
			}
			else if (auto oper = std::dynamic_pointer_cast<ParserOperator>(comp))
			{
				substatements.push_back(this->compileOperator(scope, *oper));
			}
			else
				throw CompileException("Unknown entity type");
		}

		return std::make_unique<ScriptStatement>(*scope, std::move(substatements));
	}

	// TODO
	// Runnable classes must later be set up - so that they can find references to other variables, method
	// etc. - everything that depends on other stuff
	// So - at some later point - call setup() function on everything that's applicable

	std::unique_ptr<ScriptExecutiveBlock> Compiler::compileExecutiveBlock(ScriptScope* scope, const ParserBlockBody& pblock)
	{
		std::vector<std::shared_ptr<ScriptCallable>> statements;

		for (auto &ps : pblock.statements)
		{
			if (auto statement = std::dynamic_pointer_cast<ParserVarDeclaration>(ps))
				statements.push_back(this->compileVarDeclaration(scope, *statement));
			else if (auto statement = std::dynamic_pointer_cast<ParserStatement>(ps))
				statements.push_back(this->compileStatement(scope, *statement));
			else if (auto statement = std::dynamic_pointer_cast<ParserReturn>(ps))
				statements.push_back(this->compileReturn(scope, *statement));
			else if (auto statement = std::dynamic_pointer_cast<ParserIf>(ps))
				statements.push_back(this->compileIf(scope, *statement));
			else if (auto statement = std::dynamic_pointer_cast<ParserWhile>(ps))
				statements.push_back(this->compileWhile(scope, *statement));
			else if (auto statement = std::dynamic_pointer_cast<ParserFor>(ps))
				statements.push_back(this->compileFor(scope, *statement));
		}

		return std::make_unique<ScriptExecutiveBlock>(std::move(statements));
	}

	std::unique_ptr<ScriptOperator> Compiler::compileOperator(ScriptScope* scope, const ParserOperator& pOperator)
	{
		std::unordered_map<OPERATOR_TYPE_T, std::function<std::unique_ptr<ScriptOperator>(ScriptScope&, OPERATOR_LINK_T)>> opmap = {
			{ OPERATOR_TYPE_T::OT_MEMBER_ACCESSOR, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorMemberAccessor>(s, l); }},
			{ OPERATOR_TYPE_T::OT_ADD, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorAdd>(s, l); }},
			{ OPERATOR_TYPE_T::OT_ADD_TO, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorAddTo>(s, l); }},
			{ OPERATOR_TYPE_T::OT_SUBTRACT, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorSubtract>(s, l); }},
			{ OPERATOR_TYPE_T::OT_SUBTRACT_FROM, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorSubtractFrom>(s, l); }},
			{ OPERATOR_TYPE_T::OT_MULTIPLY, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorMultiply>(s, l); }},
			{ OPERATOR_TYPE_T::OT_MULTIPLY_BY, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorMultiplyBy>(s, l); }},
			{ OPERATOR_TYPE_T::OT_DIVIDE, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorDivide>(s, l); }},
			{ OPERATOR_TYPE_T::OT_DIVIDE_BY, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorDivideBy>(s, l); }},
			{ OPERATOR_TYPE_T::OT_EQUALS, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorEquals>(s, l); }},
			{ OPERATOR_TYPE_T::OT_NOT_EQUALS, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorEquals>(s, l); }}, // TODO - missing operator for not equals?
			{ OPERATOR_TYPE_T::OT_GREATER_THAN, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorGreaterThan>(s, l); }},
			{ OPERATOR_TYPE_T::OT_GREATER_THAN_OR_EQUAL, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorGreaterThanOrEqual>(s, l); }},
			{ OPERATOR_TYPE_T::OT_LESSER_THAN, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorLessThan>(s, l); }},
			{ OPERATOR_TYPE_T::OT_LESSER_THAN_OR_EQUAL, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorLessThanOrEqual>(s, l); }},
			{ OPERATOR_TYPE_T::OT_ASSIGN, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorAssign>(s, l); }},
			{ OPERATOR_TYPE_T::OT_NEGATE, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorNegate>(s, l); }},
			{ OPERATOR_TYPE_T::OT_INCREMENT, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorIncrement>(s, l); }},
			{ OPERATOR_TYPE_T::OT_PRE_INCREMENT, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorIncrement>(s, l); }},
			{ OPERATOR_TYPE_T::OT_POST_INCREMENT, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorIncrement>(s, l); }},
			{ OPERATOR_TYPE_T::OT_DECREMENT, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorDecrement>(s, l); }},
			{ OPERATOR_TYPE_T::OT_PRE_DECREMENT, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorDecrement>(s, l); }},
			{ OPERATOR_TYPE_T::OT_POST_DECREMENT, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorDecrement>(s, l); }},
			{ OPERATOR_TYPE_T::OT_CONDITIONAL_IF, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorConditionalA>(s, l); }},
			{ OPERATOR_TYPE_T::OT_CONDITIONAL_ELSE, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorConditionalB>(s, l); }},
			{ OPERATOR_TYPE_T::OT_CONDITIONAL_NULL, [](ScriptScope& s, OPERATOR_LINK_T l) { return std::make_unique<ScriptOperatorConditionalNull>(s, l); }}
		};

		auto it = opmap.find(pOperator.getType());
		if (it == opmap.end())
			throw CompileException("Cannot find operator " + std::to_string(static_cast<int>(pOperator.getType())));

		return it->second(*scope, pOperator.getLinkage());
	}

	std::unique_ptr<ScriptVarRead> Compiler::compileVarRead(ScriptScope* scope, const ParserVar& pVar)
	{
		if (auto& arr = pVar.arrayAccessor)
			return std::make_unique<ScriptArrayReadResolver>(*scope, this->compileStatement(scope, arr->statement), pVar.name);

		return std::make_unique<ScriptVarReadResolver>(*scope, pVar.name);
	}

	std::unique_ptr<ScriptNew> Compiler::compileNewCall(ScriptScope* scope, const ParserNew& fcall)
	{
		auto params = this->compileParameterContainer(scope, fcall.arglist);

		ScriptNamespace* ns = scope->getGlobalNamespace();
		ScriptClass* sclass = ns->findClass(fcall.constructorName);

		if (!sclass)
			throw CompileException(std::string("Unable to find class \"" + fcall.constructorName + "\""));

		return std::make_unique<ScriptNew>(*sclass, std::move(params->getParams()));
	}

	std::unique_ptr<ScriptFuncCall> Compiler::compileFuncCall(ScriptScope* scope, const ParserFuncCall& fcall)
	{
		auto params = this->compileParameterContainer(scope, fcall.arglist);

		ScriptScope* usedScope = scope;
		std::string usedName = fcall.name;

		if (fcall.name.isScoped())
		{
			EntityPath::scope_iterator scopeIt = fcall.name.createScopeIterator();

			ScriptNamespace* ns = scope->getGlobalNamespace();
			for (; scopeIt != fcall.name.getString().end(); ++scopeIt)
			{
				if (scopeIt.last())
				{
					usedName = *scopeIt;
				}
				else
				{
					ScriptNamespace* foundns = ns->findClass(*scopeIt);

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

		auto types = params->getParamTypes();
		return std::make_unique<ScriptFuncCallResolver>(*usedScope, std::move(params->getParams()), usedName, types, fcall.name.isScoped());
	}

	std::unique_ptr<ScriptArrayInitializer> Compiler::compileArrayInitializer(ScriptScope* scope, const ParserArrayInitializer& initializer)
	{
		const ScriptType* prevType = nullptr;
		std::vector<std::unique_ptr<ScriptStatement>> statements;
		
		for (auto& param : initializer.arglist.parameters)
		{
			auto stmt = this->compileStatement(scope, *param);
			const ScriptType* type = stmt->getType();

			if (prevType && !prevType->equals(*type))
				throw CompileException("Mixed-type array not allowed");

			prevType = type;
			statements.push_back(std::move(stmt));
		}

		auto type = new ScriptArrayType(prevType ? prevType : ScriptType::create(VALUE_TYPE_T::VT_NULL, *scope));

		return std::make_unique<ScriptArrayInitializer>(*scope, std::move(statements));
	}

	std::unique_ptr<ScriptVarDeclaration> Compiler::compileVarDeclaration(ScriptScope* scope, const ParserVarDeclaration& pVar)
	{
		auto& v = scope->registerVariable(std::make_unique<ScriptVariable>(pVar.name, ScriptType::create(pVar.type, *scope), nullptr, scope->getVariables().size()));
		auto stmt = this->compileStatement(scope, pVar.value);
		return std::make_unique<ScriptVarDeclaration>(*scope, v, std::move(stmt));
	}

	std::unique_ptr<ScriptIf> Compiler::compileIf(ScriptScope* scope, const ParserIf& pIf)
	{
		auto condition = this->compileStatement(scope, *pIf.arglist.parameters.front());
		auto exeblock = this->compileExecutiveBlock(scope, pIf.body.body);

		auto sif = std::make_unique<ScriptIf>(*scope, std::move(condition));
		sif->merge(std::move(exeblock));

		return sif;
	}

	std::unique_ptr<ScriptWhile> Compiler::compileWhile(ScriptScope* scope, const ParserWhile& pWhile)
	{
		auto condition = this->compileStatement(scope, *pWhile.arglist.parameters.front());
		auto exeblock = this->compileExecutiveBlock(scope, pWhile.body.body);

		ScriptWhile(*scope, std::move(condition));
		auto swhile = std::make_unique<ScriptWhile>(*scope, std::move(condition));
		swhile->merge(std::move(exeblock));

		return swhile;
	}

	std::unique_ptr<ScriptFor> Compiler::compileFor(ScriptScope* scope, const ParserFor& pFor)
	{
		auto vardecl = this->compileVarDeclaration(scope, pFor.arglist.varDecl);
		auto condition = this->compileStatement(scope, pFor.arglist.condition);
		auto progress = this->compileStatement(scope, pFor.arglist.progress);
		auto exeblock = this->compileExecutiveBlock(scope, pFor.body.body);

		auto sfor = std::make_unique<ScriptFor>(*scope, std::move(vardecl), std::move(condition), std::move(progress));
		sfor->merge(std::move(exeblock));

		return sfor;
	}

	std::unique_ptr<ScriptReturn> Compiler::compileReturn(ScriptScope* scope, const ParserReturn& pReturn)
	{
		auto stmt = this->compileStatement(scope, pReturn.value);
		return std::make_unique<ScriptReturn>(*scope, std::move(stmt));
	}
}