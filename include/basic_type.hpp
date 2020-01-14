//TODO comments


#pragma once

#ifndef LOGO__XWB__BASIC_TYPE
#define LOGO__XWB__BASIC_TYPE


#include <ostream>
#include <type_traits>
#include <variant>

#include "type_categories.hpp"


using boolean=bool;
using character=char;
using fraction=double;
using integer=long long;


//helpers

template<typename type>
	struct is_basictype:
		public std::conditional_t<
			std::disjunction_v<
				std::is_same<std::remove_cvref_t<type>,boolean>,
				std::is_same<std::remove_cvref_t<type>,character>,
				std::is_same<std::remove_cvref_t<type>,fraction>,
				std::is_same<std::remove_cvref_t<type>,integer>
			>,
			std::true_type,
			std::false_type
		> {};
template<typename type>
	inline constexpr bool is_basictype_v=is_basictype<type>::value;

template<typename ...types>
	struct expr_type{
		static_assert(std::conjunction_v<is_basictype<types>...>);
		using type=std::conditional_t<
			has_type_v<fraction,types...>,fraction,
			std::conditional_t<
				has_type_v<integer,types...>,integer,
				std::conditional_t<
					has_type_v<character,types...>,character,
					boolean
				>
			>
		>;
	}; //struct expr_type
template<typename ...types>
	using expr_type_t=typename expr_type<types...>::type;

template<typename _type>
	struct to_basictype{
		using type=std::conditional_t<
			is_boolean_v<_type>,boolean,
			std::conditional_t<
				is_character_v<_type>,character,
				std::conditional_t<
					is_fraction_v<_type>,fraction,
					std::conditional_t<
						is_integer_v<_type>,integer,
						std::remove_cvref_t<_type>
					>
				>
			>
		>;
	}; //struct to_basictype
template<typename type>
	using to_basictype_t=typename to_basictype<type>::type;

template<typename ...functor_t>
	struct overloaded:public functor_t...{
		using functor_t::operator()...;
	};
template<typename ...functor_t>
	overloaded(functor_t...)->overloaded<functor_t...>;


//BasicType

class BasicType:
	public std::variant<boolean,character,fraction,integer>
{
	public:
		template<
			typename visitor_t,typename ...BasicType_t,
			typename=std::enable_if_t<
				std::conjunction_v<
					std::is_same<
						std::remove_cvref_t<BasicType_t>,
						BasicType
					>...
				>,void
			>
		> static auto visit(visitor_t &&visitor,BasicType_t &&...args);

		template<typename ...arg_ts>
			BasicType(arg_ts &&...args);

		operator boolean()const;
		operator character()const;
		operator fraction()const;
		operator integer()const;

		friend std::ostream& operator <<(std::ostream &os,const BasicType &val);

		template<typename type>
			std::enable_if_t<
				is_basictype_v<type>,
				void
			> convert();
	//public end

	private:
		using __BasicType=std::variant<boolean,character,fraction,integer>;
	//private end
}; //class BasicType


#include "detail/basic_type.ipp"


#endif //LOGO__XWB__BASIC_TYPE