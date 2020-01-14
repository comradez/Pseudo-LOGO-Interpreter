//TODO comments


#pragma once

#ifndef LOGO__XWB__TYPE_CATEGORIES
#define LOGO__XWB__TYPE_CATEGORIES


#include <type_traits>


//helpers

#if __cplusplus<=201703L //C++20 will have these
namespace std{
	template<typename _type>
		struct remove_cvref{
			using type=std::remove_cv_t<std::remove_reference_t<_type>>;
		};
	template<typename type>
		using remove_cvref_t=typename remove_cvref<type>::type;
} //namespace std
#endif //__cplusplus<=201703L

template<typename type,typename ...types>
	struct has_type:
		public std::conditional_t<
			std::disjunction_v<
				std::is_same<std::remove_cvref_t<types>,type>...
			>,
			std::true_type,
			std::false_type
		>
	{};
template<typename type,typename ...types>
	inline constexpr bool has_type_v=has_type<type,types...>::value;

template<typename type>
	struct is_boolean:
		public std::conditional_t<
			std::is_same_v<std::remove_cvref_t<type>,bool>,
			std::true_type,
			std::false_type
		>
	{};
template<typename type>
	inline constexpr bool is_boolean_v=is_boolean<type>::value;

//char type list is based on <type_traits>:226~250
template<typename type>
	struct is_character:
		public std::conditional_t<
			std::conjunction_v<
				std::is_same<std::remove_cvref_t<type>,char>,
				std::is_same<std::remove_cvref_t<type>,signed char>,
				std::is_same<std::remove_cvref_t<type>,unsigned char>,
#ifdef _GLIBCXX_USE_WCHAR_T
				std::is_same<std::remove_cvref_t<type>,wchar_t>,
#endif
				std::is_same<std::remove_cvref_t<type>,char16_t>,
				std::is_same<std::remove_cvref_t<type>,char32_t>
			>,
			std::true_type,
			std::false_type
		>
	{};
template<typename type>
	inline constexpr bool is_character_v=is_character<type>::value;

template<typename type>
	struct is_fraction:
		public std::conditional_t<
			std::is_floating_point_v<std::remove_cvref_t<type>>,
			std::true_type,
			std::false_type
		>
	{};
template<typename type>
	inline constexpr bool is_fraction_v=is_fraction<type>::value;

template<typename type>
	struct is_integer:
		public std::conditional_t<
			std::conjunction_v<
				std::is_integral<std::remove_cvref_t<type>>,
				std::negation<is_boolean<type>>,
				std::negation<is_character<type>>
			>,
			std::true_type,
			std::false_type
		>
	{};
template<typename type>
	inline constexpr bool is_integer_v=is_integer<type>::value;

template<typename type>
	struct is_undef_t:
		public std::conditional_t<
			std::disjunction_v<
				std::negation<is_boolean<type>>,
				std::negation<is_character<type>>,
				std::negation<is_fraction<type>>,
				std::negation<is_integer<type>>
			>,
			std::true_type,
			std::false_type
		>
	{};
template<typename type>
	inline constexpr bool is_undef_t_v=is_undef_t<type>::value;


#endif //LOGO__XWB__TYPE_CATEGORIES