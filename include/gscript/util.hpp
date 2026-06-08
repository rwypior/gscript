#ifndef _h_gscript_util
#define _h_gscript_util

#include "gscript/lib.hpp"
#include "gscript/defs.hpp"

#include <tuple>
#include <vector>
#include <memory>
#include <cassert>
#include <string>
#include <type_traits>

namespace gscript
{
	class ScriptStatement;
	class ScriptFieldDeclaration;
	class ScriptScope;

	namespace hash_tuple
	{
		template <typename TT>
		struct hash
		{
			size_t operator()(TT const& tt) const
			{
				return std::hash<TT>()(tt);
			}
		};

		namespace
		{
			template <class T>
			inline void hash_combine(std::size_t& seed, T const& v)
			{
				seed ^= hash_tuple::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}

			template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
			struct HashValueImpl
			{
				static void apply(size_t& seed, Tuple const& tuple)
				{
					HashValueImpl<Tuple, Index - 1>::apply(seed, tuple);
					hash_combine(seed, std::get<Index>(tuple));
				}
			};

			template <class Tuple>
			struct HashValueImpl<Tuple, 0>
			{
				static void apply(size_t& seed, Tuple const& tuple)
				{
					hash_combine(seed, std::get<0>(tuple));
				}
			};
		}

		template <typename ... TT>
		struct hash<std::tuple<TT...>>
		{
			size_t operator()(std::tuple<TT...> const& tt) const
			{
				size_t seed = 0;
				HashValueImpl<std::tuple<TT...> >::apply(seed, tt);
				return seed;
			}
		};
	}

	PARAMS_T extractParams(std::vector<std::unique_ptr<ScriptStatement>>& statements);

	template<typename To, typename From>
	std::unique_ptr<To> static_unique_pointer_cast(std::unique_ptr<From>&& from)
	{
		assert(dynamic_cast<To*>(from.get()) && "Incompatible pointers");
		return std::unique_ptr<To>(static_cast<To*>(from.release()));
	}

	template<typename T>
	std::vector<std::unique_ptr<T>> cloneVector(const std::vector<std::unique_ptr<T>>& target)
	{
		std::vector<std::unique_ptr<T>> result(target.size());
		size_t i = 0;
		for (const auto& el : target)
		{
			result[i++] = static_unique_pointer_cast<T>( el->clone() );
		}
		return result;
	}

	std::unique_ptr<ScriptStatement> createNullStatement();

	template<typename Type>
	std::unique_ptr<gscript::ScriptFieldDeclaration> createFieldDeclaration(ScriptScope& scope, const std::string& name)
	{
		return std::make_unique<gscript::ScriptFieldDeclaration>(
			scope,
			name,
			std::make_unique<Type>(),
			gscript::createNullStatement()
		);
	}

	template <typename...>
	struct isUniquePtr final : std::false_type
	{
	};

	template<class T, typename... Args>
	struct isUniquePtr<std::unique_ptr<T, Args...>> final : std::true_type 
	{
	};

	template<typename T>
	struct dereference_t
	{
		T& operator()(std::remove_reference_t<std::remove_cv_t<T>>& a)
		{
			return a;
		}
	};

	template<typename T>
	struct dereference_t<T*>
	{
		T& operator()(T* a)
		{
			return *a;
		}
	};

	template<typename T>
	struct dereference_t<std::unique_ptr<T>>
	{
		T& operator()(std::unique_ptr<T>& a)
		{
			return *a;
		}
	};

	template<typename T>
	auto dereference(T& a)
	{
		return dereference_t<T>()(a);
	}

	template<typename T>
	auto dereference(T&& a)
	{
		return dereference_t<T>()(a);
	}

	template<typename Key, typename Value>
	auto flip(const std::map<Key, Value>& collection)
	{
		std::map<Value, Key> flipped;
		for (auto& pair : collection)
		{
			flipped[pair.second] = pair.first;
		}
		return flipped;
	}

	/// Format list of parameters to human-friendly string
	std::string parametersToString(const PARAMS_T& params, const bool withNewLines = false);
}

#endif