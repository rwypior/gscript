#include "gscript/factory/OperatorFunctionFactory.hpp"
#include "gscript/runtime/scriptValue.hpp"
#include "gscript/runtime/callable.hpp"
#include "gscript/runtime/varRead.hpp"
#include "gscript/runtime/variable.hpp"
#include "gscript/runtime/operator.hpp"
#include "gscript/runtime/class.hpp"
#include "gscript/type.hpp"
#include "gscript/compileException.hpp"

#include <sstream>

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

		// Operators

		std::unique_ptr<gscript::ScriptValue> operator+(int a, std::unique_ptr<gscript::ScriptClassInstance>& b)
		{
			return nullptr;
		}

		std::unique_ptr<gscript::ScriptValue> operator-(int a, std::unique_ptr<gscript::ScriptClassInstance>& b)
		{
			return nullptr;
		}

		std::unique_ptr<gscript::ScriptValue> operator*(int a, std::unique_ptr<gscript::ScriptClassInstance>& b)
		{
			return nullptr;
		}

		std::unique_ptr<gscript::ScriptValue> operator/(int a, std::unique_ptr<gscript::ScriptClassInstance>& b)
		{
			return nullptr;
		}

		std::unique_ptr<gscript::ScriptValue> operator==(int a, std::unique_ptr<gscript::ScriptClassInstance>& b)
		{
			return nullptr;
		}

		std::unique_ptr<gscript::ScriptValue> operator!=(int a, std::unique_ptr<gscript::ScriptClassInstance>& b)
		{
			return nullptr;
		}

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
				if constexpr (isUniquePtr<decltype(val)>::value)
					return val;
				else
					return std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
			}

			template<typename T_LEFT, typename T_RIGHT>
			std::unique_ptr<ScriptValue> AddTo(ScriptScopeBase& scope, ScriptCallable *left, ScriptCallable *right)
			{
				// TODO - we're getting ScriptVariable instead of ScriptReferenceVariable here,
				// it's probably the reason why the variable isn't changing, check this!!!
				auto val = static_cast<T_LEFT*>(left->run(scope)->data())->getValue() + static_cast<T_RIGHT*>(right->run(scope)->data())->getValue();
				auto res = std::make_unique<typename OperatorReturnType<decltype(val)>::type>(val);
				ScriptVarRead* vr = static_cast<ScriptVarReferenceRead*>(left);
				auto var = vr->get();
				var->setValue(res->clone());
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
				ScriptVarRead* vr = static_cast<ScriptVarRead*>(left);
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
				ScriptVarRead* vr = static_cast<ScriptVarRead*>(left);
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
				ScriptVarRead* vr = static_cast<ScriptVarRead*>(left);
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
				//auto val = --static_cast<T_RIGHT*>(right->run(scope)->data())->getValue();
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
				//auto val = static_cast<T_LEFT*>(data)->getValue()--;
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
		OperatorFunction getFunctionPOD(OperatorType func)
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

		template<typename L, typename R>
		OperatorFunction getFunction(std::shared_ptr<ScriptCallable> left, std::shared_ptr<ScriptCallable> right, OperatorType func)
		{
			if constexpr (std::is_same<L, ScriptClassValue>::value)
			{
				auto& cls = std::static_pointer_cast<ScriptClassType>(left->getType())->getClass();
				auto params = PARAMS_T{ { left->getType() }, { right->getType() } };
				auto oper = cls.findOperator(
					func, params
				);
				if (!oper)
				{
					throw CompileException((
							std::stringstream() << "Cannot find an operator \"" << getOperatorFunctionName(func) << "\" " <<
							parametersToString(params) << " in class \"" << cls.getName() << "\""
						).str()
					);
				}
				return [&oper](ScriptScopeBase& scope, ScriptCallable* left, ScriptCallable* right) {
					CALLABLE_PARAMS_T cparams;
					cparams.push_back(left->run(scope));
					cparams.push_back(right->run(scope));
					return oper->run(scope, std::move(cparams));
				};
			}
			else if constexpr (std::is_same<R, ScriptClassValue>::value)
			{
				return nullptr;
			}
			else
			{
				return getFunctionPOD<L, R>(func);
			}

			return nullptr;
		}

		template<typename T>
		OperatorFunction getFunction(std::shared_ptr<ScriptCallable> left, std::shared_ptr<ScriptCallable> right, OperatorType func)
		{
			ValueType typeRight = right ? right->getType()->getTypeDescriptor() : ValueType::Void;

			switch (typeRight)
			{
			case ValueType::Bool: return getFunction<T, ScriptBoolValue>(std::move(left), std::move(right), func);
			case ValueType::Char: return getFunction<T, ScriptCharValue>(std::move(left), std::move(right), func);
			case ValueType::Byte: return getFunction<T, ScriptByteValue>(std::move(left), std::move(right), func);
			case ValueType::Int: return getFunction <T, ScriptIntValue>(std::move(left), std::move(right), func);
			case ValueType::UnsignedInt: return getFunction<T, ScriptUnsignedIntValue>(std::move(left), std::move(right), func);
			case ValueType::Float: return getFunction<T, ScriptFloatValue>(std::move(left), std::move(right), func);
			case ValueType::Double: return getFunction<T, ScriptDoubleValue>(std::move(left), std::move(right), func);
			case ValueType::Void: return getFunction<T, ScriptNullValue>(std::move(left), std::move(right), func);
			case ValueType::Class: return getFunction<T, ScriptClassValue>(std::move(left), std::move(right), func);
			}

			return nullptr;
		}

		OperatorFunction getFunction(std::shared_ptr<ScriptCallable> left, std::shared_ptr<ScriptCallable> right, OperatorType func)
		{
			ValueType typeLeft = left ? left->getType()->getTypeDescriptor() : ValueType::Void;
			ValueType typeRight = right ? right->getType()->getTypeDescriptor() : ValueType::Void;

			switch (typeLeft)
			{
			case ValueType::Bool: return getFunction<ScriptBoolValue>(std::move(left), std::move(right), func);
			case ValueType::Char: return getFunction<ScriptCharValue>(std::move(left), std::move(right), func);
			case ValueType::Byte: return getFunction<ScriptByteValue>(std::move(left), std::move(right), func);
			case ValueType::Int: return getFunction<ScriptIntValue>(std::move(left), std::move(right), func);
			case ValueType::UnsignedInt: return getFunction<ScriptUnsignedIntValue>(std::move(left), std::move(right), func);
			case ValueType::Float: return getFunction<ScriptFloatValue>(std::move(left), std::move(right), func);
			case ValueType::Double: return getFunction<ScriptDoubleValue>(std::move(left), std::move(right), func);
			case ValueType::Void: return getFunction<ScriptNullValue>(std::move(left), std::move(right), func);
			case ValueType::Class: return getFunction<ScriptClassValue>(std::move(left), std::move(right), func);
			}

			return nullptr;
		}
	}
}