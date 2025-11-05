#include "gscript/factory/OperatorFunctionFactory.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/operator.hpp"
#include "gscript/type.hpp"
#include "gscript/compileException.hpp"

namespace gscript
{
	namespace OperatorFunctionFactory
	{
		// Type converters

		template<typename>
		struct OperatorReturnType;

		template<>
		struct OperatorReturnType<bool>
		{
			using type = ScriptBoolValue;
		};

		template<>
		struct OperatorReturnType<char>
		{
			using type = ScriptCharValue;
		};

		template<>
		struct OperatorReturnType<unsigned char>
		{
			using type = ScriptByteValue;
		};

		template<>
		struct OperatorReturnType<int>
		{
			using type = ScriptIntValue;
		};

		template<>
		struct OperatorReturnType<unsigned int>
		{
			using type = ScriptUnsignedIntValue;
		};

		template<>
		struct OperatorReturnType<float>
		{
			using type = ScriptFloatValue;
		};

		template<>
		struct OperatorReturnType<double>
		{
			using type = ScriptDoubleValue;
		};

		template<>
		struct OperatorReturnType<std::string>
		{
			using type = ScriptStringValue;
		};
		
		template<>
		struct OperatorReturnType<ScriptNull>
		{
			using type = ScriptNullValue;
		};

		// Functions

		namespace Functions
		{

// Disable the 'unsafe mix of type' warnings
#pragma warning(push)
#pragma warning(disable: 4804 4805)

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> Add(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run(scope)->data())->getValue() + static_cast<T_RIGHT*>(right->run(scope)->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> AddTo(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run(scope)->data())->getValue() + static_cast<T_RIGHT*>(right->run(scope)->data())->getValue();
				auto res = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> Subtract(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run(scope)->data())->getValue() - static_cast<T_RIGHT*>(right->run(scope)->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> SubtractFrom(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run(scope)->data())->getValue() - static_cast<T_RIGHT*>(right->run(scope)->data())->getValue();
				auto res = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> Multiply(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run(scope)->data())->getValue() * static_cast<T_RIGHT*>(right->run(scope)->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> MultiplyBy(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run(scope)->data())->getValue() * static_cast<T_RIGHT*>(right->run(scope)->data())->getValue();
				auto res = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> Divide(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run(scope)->data())->getValue() / static_cast<T_RIGHT*>(right->run(scope)->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> DivideBy(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run(scope)->data())->getValue() / static_cast<T_RIGHT*>(right->run(scope)->data())->getValue();
				auto res = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> Equal(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run(scope)->data())->getValue() == static_cast<T_RIGHT*>(right->run(scope)->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> NotEqual(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run(scope)->data())->getValue() != static_cast<T_RIGHT*>(right->run(scope).get())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> GreaterThan(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)
			{
				auto valLeft = static_cast<T_LEFT*>(left->run(scope)->data())->getValue();
				auto valRight = static_cast<T_RIGHT*>(right->run(scope)->data())->getValue();
				auto val = valLeft > valRight;
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> GreaterOrEqualThan(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run(scope)->data())->getValue() >= static_cast<T_RIGHT*>(right->run(scope)->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> LessThan(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run(scope)->data())->getValue() < static_cast<T_RIGHT*>(right->run(scope)->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> LessOrEqualThan(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)
			{
				auto val = static_cast<T_LEFT*>(left->run(scope)->data())->getValue() <= static_cast<T_RIGHT*>(right->run(scope)->data())->getValue();
				return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_RIGHT>
			std::unique_ptr<ScriptValue> PreDecrement(ScriptScopeBase& scope, ScriptCallable*, ScriptCallable *right)
			{
				auto val = static_cast<T_RIGHT*>(right->run(scope)->data())->getValue() - 1;
				auto res = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(right);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT>
			std::unique_ptr<ScriptValue> PostDecrement(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable*)
			{
				auto data = left->run(scope)->data();
				auto val = static_cast<T_LEFT*>(data)->getValue() - 1;
				auto newVal = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				auto res = data->clone();
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(std::move(newVal));
				return res;
			}

			template<typename T_RIGHT>
			std::unique_ptr<ScriptValue> PreIncrement(ScriptScopeBase& scope, ScriptCallable*, ScriptCallable *right)
			{
				auto val = static_cast<T_RIGHT*>(right->run(scope)->data())->getValue() + 1;
				auto res = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(right);
				vr->get()->setValue(res->clone());
				return res;
			}

			template<typename T_LEFT>
			std::unique_ptr<ScriptValue> PostIncrement(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable*)
			{
				//auto lvalue = left->run(scope);
				auto data = left->run(scope)->data();
				auto val = static_cast<T_LEFT*>(data)->getValue() + 1;
				auto newVal = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				auto res = data->clone();
				ScriptVarRead *vr = static_cast<ScriptVarRead*>(left);
				vr->get()->setValue(std::move(newVal));
				return res;
			}

#pragma warning(pop)
		}

		// Factory

		template<typename L, typename R>
		OperatorFunction getFunction(OperatorType func)
		{
			switch (func)
			{
			case OperatorType::Add: return &Functions::Add<L, R>;
			case OperatorType::AddTo: return &Functions::AddTo<L, R>;
			case OperatorType::Subtract: return &Functions::Subtract<L, R>;
			case OperatorType::SubtractFrom: return &Functions::SubtractFrom<L, R>;
			case OperatorType::Multiply: return &Functions::Multiply<L, R>;
			case OperatorType::MultiplyBy: return &Functions::MultiplyBy<L, R>;
			case OperatorType::Divide: return &Functions::Divide<L, R>;
			case OperatorType::DivideBy: return &Functions::DivideBy<L, R>;

			case OperatorType::PreDecrement: return &Functions::PreDecrement<R>;
			case OperatorType::PostDecrement: return &Functions::PostDecrement<L>;
			case OperatorType::PreIncrement: return &Functions::PreIncrement<R>;
			case OperatorType::PostIncrement: return &Functions::PostIncrement<L>;

			case OperatorType::Equals: return &Functions::Equal<L, R>;
			case OperatorType::GreaterThan: return &Functions::GreaterThan<L, R>;
			case OperatorType::GreaterThanOrEqual: return &Functions::GreaterOrEqualThan<L, R>;
			case OperatorType::LesserThan: return &Functions::LessThan<L, R>;
			case OperatorType::LesserThanOrEqual: return &Functions::LessOrEqualThan<L, R>;
			}

			return nullptr;
		}

		template<typename T>
		OperatorFunction getFunction(ValueType right, OperatorType func)
		{
			switch (right)
			{
			case ValueType::Bool: return getFunction<T, ScriptBoolValue>(func);
			case ValueType::Char: return getFunction<T, ScriptCharValue>(func);
			case ValueType::Byte: return getFunction<T, ScriptByteValue>(func);
			case ValueType::Int: return getFunction <T, ScriptIntValue> (func);
			case ValueType::UnsignedInt: return getFunction<T, ScriptUnsignedIntValue>(func);
			case ValueType::Float: return getFunction<T, ScriptFloatValue>(func);
			case ValueType::Double: return getFunction<T, ScriptDoubleValue>(func);
			case ValueType::Void: return getFunction<T, ScriptNullValue>(func);
			}

			return nullptr;
		}

		OperatorFunction getFunction(ValueType left, ValueType right, OperatorType func)
		{
			switch (left)
			{
			case ValueType::Bool: return getFunction<ScriptBoolValue>(right, func);
			case ValueType::Char: return getFunction<ScriptCharValue>(right, func);
			case ValueType::Byte: return getFunction<ScriptByteValue>(right, func);
			case ValueType::Int: return getFunction<ScriptIntValue>(right, func);
			case ValueType::UnsignedInt: return getFunction<ScriptUnsignedIntValue>(right, func);
			case ValueType::Float: return getFunction<ScriptFloatValue>(right, func);
			case ValueType::Double: return getFunction<ScriptDoubleValue>(right, func);
			case ValueType::Void: return getFunction<ScriptNullValue>(right, func);
			}

			return nullptr;
		}
	}
}