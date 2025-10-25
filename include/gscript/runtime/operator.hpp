#ifndef _h_gscript_operator
#define _h_gscript_operator

#include "variable.hpp"
#include "statement.hpp"
#include "scriptValue.hpp"
#include "scope.hpp"
#include "factory/OperatorFunctionFactory.hpp"

#include <memory>

namespace gscript
{
	class ScriptVarRead;

	class ScriptOperator : public ScriptCallable
	{
	public:
		static std::unordered_map<OperatorType, std::string> opmap;

		OperatorLinkage linkage;
		std::shared_ptr<ScriptCallable> left = nullptr;
		std::shared_ptr<ScriptCallable> right = nullptr;

		ScriptOperator(const ScriptOperator& b);
		ScriptOperator(ScriptScope &scope, OperatorLinkage linkage = OperatorLinkage::Single);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override = 0;

		virtual const std::shared_ptr<ScriptType> getType() const override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override = 0;
		virtual const int getPrecedence() const = 0;
		virtual const OperatorType getOperatorType() const = 0;
		virtual bool needFactory() const;
		virtual void setup() {};

		void assignOperatorFunction();

		static std::string translateOperator(OperatorType t);

	protected:
		OperatorFunctionFactory::OperatorFunction operatorFunction = nullptr;
	};

	class ScriptOperatorMemberAccessor : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
		virtual bool needFactory() const override;
	};

	class ScriptOperatorNegate : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorAssign : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		ScriptOperatorAssign(const ScriptOperatorAssign& b);

		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
		virtual bool needFactory() const override;
		virtual void setup() override;

	protected:
		ScriptVarRead *varRead = nullptr;
	};

	class ScriptOperatorEquals : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorGreaterThan : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorLessThan : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorGreaterThanOrEqual : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorLessThanOrEqual : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorAdd : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorAddTo : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorSubtract : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorSubtractFrom : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorMultiply : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorMultiplyBy : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorDivide : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorDivideBy : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorIncrement : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorDecrement : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
	};

	class ScriptOperatorConditionalA : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
		virtual bool needFactory() const override;
	};

	class ScriptOperatorConditionalB : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
		virtual bool needFactory() const override;
		virtual void setup() override;
	};

	class ScriptOperatorConditionalNull : public ScriptOperator
	{
	using ScriptOperator::ScriptOperator;

	public:
		SCRIPT_API virtual std::unique_ptr<ScriptCallable> clone() override;

		virtual std::unique_ptr<ScriptValue> run(ScriptScopeBase& scope, const CALLABLE_PARAMS_T &c = CALLABLE_PARAMS_T()) override;
		virtual const int getPrecedence() const override;
		virtual const OperatorType getOperatorType() const;
		virtual bool needFactory() const override;
	};
}

#endif