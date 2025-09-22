#include "compiler/compiler.hpp"

#include "parser/pStatement.hpp"
#include "parser/pCallArglist.hpp"
#include "parser/pBlockBody.hpp"
#include "parser/pNamespace.hpp"
#include "parser/pFunction.hpp"
#include "parser/pMethod.hpp"
#include "parser/pLiteral.hpp"
#include "parser/pVar.hpp"
#include "parser/pFuncCall.hpp"
#include "parser/pNew.hpp"
#include "parser/pArrayInitializer.hpp"
#include "parser/pOperator.hpp"
#include "parser/pArrayAccessor.hpp"
#include "parser/pReturn.hpp"
#include "parser/pIf.hpp"
#include "parser/pElse.hpp"
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
#include "logger.hpp"

#include "runtimeException.hpp"
#include "compileException.hpp"

#include <unordered_map>
#include <functional>
#include <memory>

namespace gscript
{
	std::unique_ptr<FunctionParameterContainer> Compiler::compileParameterContainer(ScriptScope* scope, const ParserCallArglist& arglist)
	{
		gs_log("Compiling FunctionParameterContainer");

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
		gs_log("Compiling ScriptNamespace " << entry.name);

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
		gs_log("Compiling ScriptClass " << pclass.name);

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

		for (auto& pf : pclass.fields)
		{
			auto field = std::make_unique<ScriptFieldDeclaration>(*cl, pf.name, ScriptType::create(pf.type, *cl), this->compileStatement(cl.get(), pf.value));
			cl->addFieldDeclaration(std::move(field));
		}

		for (auto& pmethod : pclass.methods)
		{
			auto method = this->compileMethod(scope, pmethod);
			cl->registerFunction(std::move(method));
		}

		return cl;
	}

	std::unique_ptr<ScriptFunction> Compiler::compileFunction(ScriptScope* scope, const ParserFunction& pfunc)
	{
		gs_log("Compiling ScriptFunction " << pfunc.name);

		PARAMS_T params;
		for (auto& pp : pfunc.arglist.parameters)
		{
			params.emplace_back(ScriptType::create(pp.type, *scope), pp.name);
		}

		auto newfunc = std::make_unique<ScriptFunction>(*scope, pfunc.name, ScriptType::create(pfunc.returnTypeName, *scope), params);
		newfunc->merge(std::move(this->compileExecutiveBlock(newfunc.get(), pfunc.body)));

		return newfunc;
	}

	std::unique_ptr<ScriptMethod> Compiler::compileMethod(ScriptScope* scope, const ParserMethod& pfunc)
	{
		gs_log("Compiling ScriptMethod " << pfunc.name);

		PARAMS_T params;
		for (auto& pp : pfunc.arglist.parameters)
		{
			params.emplace_back(ScriptType::create(pp.type, *scope), pp.name);
		}

		auto newfunc = std::make_unique<ScriptMethod>(*scope, pfunc.name, ScriptType::create(pfunc.returnTypeName, *scope), params, pfunc.accessSpecifier.getModifier());
		newfunc->merge(std::move(this->compileExecutiveBlock(newfunc.get(), pfunc.body)));

		return newfunc;
	}

	bool parseBool(const std::string& value)
	{
		if (value == "true")
			return true;
		else if (value == "false")
			return false;

		return std::stoi(value);
	}

	std::unique_ptr<ScriptValue> Compiler::compileValue(const ParserLiteral& pLiteral)
	{
		gs_log("Compiling ScriptValue " << pLiteral.value);

		switch (pLiteral.type)
		{
		case ValueType::Bool:
			return std::make_unique<ScriptBoolValue>(parseBool(pLiteral.value));
		case ValueType::Char:
			return std::make_unique<ScriptCharValue>(pLiteral.value[0]);
		case ValueType::Byte:
			return std::make_unique<ScriptByteValue>(static_cast<unsigned char>(std::stoul(pLiteral.value)));
		case ValueType::Int:
			return std::make_unique<ScriptIntValue>(std::stoi(pLiteral.value));
		case ValueType::UnsignedInt:
			return std::make_unique<ScriptUnsignedIntValue>(static_cast<unsigned int>(std::stoul(pLiteral.value)));
		case ValueType::Float:
			return std::make_unique<ScriptFloatValue>(std::stof(pLiteral.value));
		case ValueType::Double:
			return std::make_unique<ScriptDoubleValue>(std::stod(pLiteral.value));
		case ValueType::String:
			return std::make_unique<ScriptStringValue>(pLiteral.value);
		}

		throw RuntimeException(std::string("Unknown type ID ") + std::to_string(static_cast<size_t>(pLiteral.type)));
	}

	std::unique_ptr<ScriptStatement> Compiler::compileStatement(ScriptScope* scope, const ParserStatement& pstatement)
	{
		gs_log("Compiling ScriptStatement " << pstatement.components.size() << " components");

		std::vector<std::unique_ptr<ScriptCallable>> substatements;
		substatements.reserve(pstatement.components.size());
		int currentIndex = 0;

		ScriptScopeBase* stmtScope = nullptr;

		for (auto& comp : pstatement.components)
		{
			ScriptScopeBase* currentScope = stmtScope ? stmtScope : scope;

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

	std::unique_ptr<ScriptExecutiveBlock> Compiler::compileExecutiveBlock(ScriptScope* scope, const ParserBlockBody& pblock)
	{
		gs_log("Compiling ScriptExecutiveBlock with " << pblock.statements.size() << " statements");

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
		gs_log("Compiling ScriptOperator " << pOperator.getChar());

		std::unordered_map<OperatorType, std::function<std::unique_ptr<ScriptOperator>(ScriptScope&, OperatorLinkage)>> opmap = {
			{ OperatorType::MemberAccessor, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorMemberAccessor>(s, l); }},
			{ OperatorType::Add, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorAdd>(s, l); }},
			{ OperatorType::AddTo, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorAddTo>(s, l); }},
			{ OperatorType::Subtract, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorSubtract>(s, l); }},
			{ OperatorType::SubtractFrom, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorSubtractFrom>(s, l); }},
			{ OperatorType::Multiply, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorMultiply>(s, l); }},
			{ OperatorType::MultiplyBy, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorMultiplyBy>(s, l); }},
			{ OperatorType::Divide, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorDivide>(s, l); }},
			{ OperatorType::DivideBy, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorDivideBy>(s, l); }},
			{ OperatorType::Equals, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorEquals>(s, l); }},
			{ OperatorType::NotEquals, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorEquals>(s, l); }}, // TODO - missing operator for not equals?
			{ OperatorType::GreaterThan, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorGreaterThan>(s, l); }},
			{ OperatorType::GreaterThanOrEqual, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorGreaterThanOrEqual>(s, l); }},
			{ OperatorType::LesserThan, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorLessThan>(s, l); }},
			{ OperatorType::LesserThanOrEqual, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorLessThanOrEqual>(s, l); }},
			{ OperatorType::Assign, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorAssign>(s, l); }},
			{ OperatorType::Negate, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorNegate>(s, l); }},
			{ OperatorType::Increment, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorIncrement>(s, l); }},
			{ OperatorType::PreIncrement, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorIncrement>(s, l); }},
			{ OperatorType::PostIncrement, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorIncrement>(s, l); }},
			{ OperatorType::Decrement, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorDecrement>(s, l); }},
			{ OperatorType::PreDecrement, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorDecrement>(s, l); }},
			{ OperatorType::PostDecrement, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorDecrement>(s, l); }},
			{ OperatorType::ConditionalIf, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorConditionalA>(s, l); }},
			{ OperatorType::ConditionalElse, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorConditionalB>(s, l); }},
			{ OperatorType::ConditionalNull, [](ScriptScope& s, OperatorLinkage l) { return std::make_unique<ScriptOperatorConditionalNull>(s, l); }}
		};

		auto it = opmap.find(pOperator.getType());
		if (it == opmap.end())
			throw CompileException("Cannot find operator " + std::to_string(static_cast<int>(pOperator.getType())));

		return it->second(*scope, pOperator.getLinkage());
	}

	std::unique_ptr<ScriptCallable> Compiler::compileVarRead(ScriptScope* scope, const ParserVar& pVar)
	{
		gs_log("Compiling ScriptVarRead " << pVar.name.getString());

		if (auto& arr = pVar.arrayAccessor)
			return std::make_unique<ScriptArrayReadPrototype>(*scope, pVar.name, this->compileStatement(scope, arr->statement));

		return std::make_unique<ScriptVarReadPrototype>(*scope, pVar.name);
	}

	std::unique_ptr<ScriptNew> Compiler::compileNewCall(ScriptScope* scope, const ParserNew& fcall)
	{
		gs_log("Compiling ScriptNew " << fcall.name.getString());

		auto params = this->compileParameterContainer(scope, fcall.arglist);

		ScriptNamespace* ns = scope->getGlobalNamespace();
		ScriptClass* sclass = ns->findClass(fcall.constructorName);

		if (!sclass)
			throw CompileException(std::string("Unable to find class \"" + fcall.constructorName + "\""));

		return std::make_unique<ScriptNew>(*sclass, std::move(params->getParams()));
	}

	std::unique_ptr<ScriptCallable> Compiler::compileFuncCall(ScriptScope* scope, const ParserFuncCall& fcall)
	{
		gs_log("Compiling ScriptFuncCall " << fcall.name.getString());

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
		bool isStaticCall = fcall.name.isScoped();
		return std::make_unique<ScriptFuncCallPrototype>(*usedScope, usedName, std::move(params->getParams()), isStaticCall);
	}

	std::unique_ptr<ScriptArrayInitializer> Compiler::compileArrayInitializer(ScriptScope* scope, const ParserArrayInitializer& initializer)
	{
		gs_log("Compiling ScriptArrayInitializer with " << initializer.arglist.parameters.size() << " arguments");

		std::shared_ptr<ScriptType> prevType;
		std::vector<std::unique_ptr<ScriptStatement>> statements;
		
		for (auto& param : initializer.arglist.parameters)
		{
			auto stmt = this->compileStatement(scope, *param);
			const auto type = stmt->getType();

			if (prevType && !prevType->equals(*type))
				throw CompileException("Mixed-type array not allowed");

			prevType = type;
			statements.push_back(std::move(stmt));
		}

		auto type = new ScriptArrayType(prevType ? prevType : ScriptType::create(ValueType::Null, *scope));

		return std::make_unique<ScriptArrayInitializer>(*scope, std::move(statements));
	}

	std::unique_ptr<ScriptVarDeclaration> Compiler::compileVarDeclaration(ScriptScope* scope, const ParserVarDeclaration& pVar)
	{
		gs_log("Compiling ScriptVarDeclaration " << pVar.name);

		auto& v = scope->registerVariable(std::make_unique<ScriptVariable>(pVar.name, ScriptType::create(pVar.type, *scope), nullptr));
		auto stmt = this->compileStatement(scope, pVar.value);
		return std::make_unique<ScriptVarDeclaration>(*scope, v, std::move(stmt));
	}

	std::unique_ptr<ScriptIf> Compiler::compileIf(ScriptScope* scope, const ParserIf& pIf)
	{
		gs_log("Compiling ScriptIf");

		auto condition = this->compileStatement(scope, *pIf.arglist.parameters.front());
		auto exeblock = this->compileExecutiveBlock(scope, pIf.body.body);

		std::unique_ptr<ScriptIf> selse;
		if (!pIf.pelse.body.body.statements.empty())
			selse = this->compileElse(scope, pIf.pelse);

		auto sif = std::make_unique<ScriptIf>(*scope, std::move(condition), std::move(selse));
		sif->merge(std::move(exeblock));

		return sif;
	}

	std::unique_ptr<ScriptIf> Compiler::compileElse(ScriptScope* scope, const ParserElse& pElse)
	{
		gs_log("Compiling ScriptElse");

		// TODO - change this, so compiled 'else if' statement will have a condition
		// instead of adding a statement with single 'if'

		std::unique_ptr<ScriptStatement> condition;
		std::unique_ptr<ScriptIf> selse;
		if (pElse.pif)
		{
			condition = this->compileStatement(scope, *pElse.pif->arglist.parameters.front());
			if (!pElse.pif->pelse.body.body.statements.empty())
				selse = this->compileElse(scope, pElse.pif->pelse);
		}
		
		auto exeblock = this->compileExecutiveBlock(scope, pElse.body.body);
		auto sif = std::make_unique<ScriptIf>(*scope, std::move(condition), std::move(selse));
		sif->merge(std::move(exeblock));

		return sif;
	}

	std::unique_ptr<ScriptWhile> Compiler::compileWhile(ScriptScope* scope, const ParserWhile& pWhile)
	{
		gs_log("Compiling ScriptWhile");

		auto condition = this->compileStatement(scope, *pWhile.arglist.parameters.front());
		auto exeblock = this->compileExecutiveBlock(scope, pWhile.body.body);

		auto swhile = std::make_unique<ScriptWhile>(*scope, std::move(condition));
		swhile->merge(std::move(exeblock));

		return swhile;
	}

	std::unique_ptr<ScriptFor> Compiler::compileFor(ScriptScope* scope, const ParserFor& pFor)
	{
		gs_log("Compiling ScriptFor");

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
		gs_log("Compiling ScriptReturn with " << pReturn.value.components.size() << " components");

		auto stmt = this->compileStatement(scope, pReturn.value);
		return std::make_unique<ScriptReturn>(*scope, std::move(stmt));
	}

	// Finalize

	void Compiler::finalize(ScriptNamespace& ns)
	{
		gs_log("Compiler - finalize");

		this->finalizeScope(ns);

		for (auto& ns : ns.getNamespaces())
		{
			this->finalize(*ns);
			this->finalizeScope(*ns);
		}

		for (auto& cls : ns.getClasses())
		{
			this->finalizeScope(*cls);
		}
	}

	void Compiler::finalizeScope(ScriptScopeBase& scope)
	{
		for (auto& fnc : scope.getFunctions())
		{
			auto& stmts = fnc->getStatements();

			for (auto& stmt : fnc->getStatements())
			{
				this->finalizeCallable(stmt);
			}
		}
	}

	void Compiler::finalizeCallable(std::shared_ptr<ScriptCallable>& callable, ScriptScopeBase* scope)
	{
		if (auto proto = std::dynamic_pointer_cast<ScriptCallablePrototype>(callable))
		{
			callable = proto->build(scope);
		}
		else if (auto stmt = std::dynamic_pointer_cast<ScriptStatement>(callable))
		{
			if (auto proto = std::dynamic_pointer_cast<ScriptCallablePrototype>(stmt->callable))
			{
				stmt->callable = proto->build();
			}
			this->finalizeCallable(stmt->callable);
		}
		else if (auto ret = std::dynamic_pointer_cast<ScriptReturn>(callable))
		{
			this->finalizeCallable(ret->getStatement());
		}
		else if (auto oper = std::dynamic_pointer_cast<ScriptOperator>(callable))
		{
			this->finalizeCallable(oper->left);

			ScriptScopeBase* usedScope = scope;
			if (auto memberacc = std::dynamic_pointer_cast<ScriptOperatorMemberAccessor>(oper))
			{
				auto& target = std::static_pointer_cast<const ScriptClassType>(memberacc->left->getType())->getClass();
				usedScope = &target;
			}

			this->finalizeCallable(oper->right, usedScope);
		}
	}

	void Compiler::finalizeCallable(std::unique_ptr<ScriptCallable>& callable, ScriptScopeBase* scope)
	{
		if (auto proto = dynamic_cast<ScriptCallablePrototype*>(callable.get()))
		{
			callable = proto->build(scope);
			this->finalizeCallable(callable);
		}
		else if (auto stmt = dynamic_cast<ScriptStatement*>(callable.get()))
		{
			if (auto proto = std::dynamic_pointer_cast<ScriptCallablePrototype>(stmt->callable))
			{
				stmt->callable = proto->build();
			}
			this->finalizeCallable(stmt->callable);
		}
		else if (auto ret = dynamic_cast<ScriptReturn*>(callable.get()))
		{
			this->finalizeCallable(ret->getStatement());
		}
		else if (auto oper = dynamic_cast<ScriptOperator*>(callable.get()))
		{
			this->finalizeCallable(oper->left);

			ScriptScopeBase* usedScope = scope;
			if (auto memberacc = dynamic_cast<ScriptOperatorMemberAccessor*>(oper))
			{
				auto& target = std::static_pointer_cast<const ScriptClassType>(memberacc->left->getType())->getClass();
				usedScope = &target;
			}

			this->finalizeCallable(oper->right, usedScope);
		}
	}
}