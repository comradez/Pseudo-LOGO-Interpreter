//TODO comments


#pragma once

#ifndef LOGO__XWB__BASIC_TYPE_DETAIL
#define LOGO__XWB__BASIC_TYPE_DETAIL


#include <iomanip>
#include <ios>
#include <ostream>
#include <type_traits>
#include <utility>
#include <variant>

#include "basic_type.hpp"


// public

template<typename visitor_t,typename ...BasicType_t,typename>
	inline auto BasicType::visit(visitor_t &&visitor,BasicType_t &&...args){
		using variant_t=BasicType::__BasicType;
		return std::visit(
			visitor,
			std::forward<variant_t>((variant_t&&)args)...
		);
	}

template<typename ...arg_ts>
	inline BasicType::BasicType(arg_ts &&...args)
		:__BasicType(
			std::forward<to_basictype_t<arg_ts>>(
				(to_basictype_t<arg_ts>&&)args
			)...
		)
	{static_assert(std::is_base_of_v<__BasicType,BasicType>);}

inline BasicType::operator boolean()const{
	return visit(
		[](auto &&val)->boolean{return val;},
		*this
	);
}

inline BasicType::operator character()const{
	return visit(
		[](auto &&val)->character{return val;},
		*this
	);
}

inline BasicType::operator fraction()const{
	return visit(
		[](auto &&val)->fraction{return val;},
		*this
	);
}

inline BasicType::operator integer()const{
	return visit(
		[](auto &&val)->integer{return val;},
		*this
	);
}

inline std::ostream& operator <<(std::ostream &os,const BasicType &val){
	os<<std::setiosflags(std::ios_base::boolalpha);
	BasicType::visit(
		overloaded{
			[&os](auto &&_val)->void{os<<_val;},
			[&os](character _val)->void{
				switch(_val){ //escape list is based on cppreference
					case 'a': os<<"'\a'"; break;
					case 'b': os<<"'\b'"; break;
					case 'f': os<<"'\f'"; break;
					case 'n': os<<"'\n'"; break;
					case 'r': os<<"'\r'"; break;
					case 't': os<<"'\t'"; break;
					case 'v': os<<"'\v'"; break;
					default: os<<'\''<<_val<<'\'';
				}
			}
		},
		val
	);
	os<<std::resetiosflags(std::ios_base::boolalpha);
	return os;
}

template<typename type>
	inline std::enable_if_t<
		is_basictype_v<type>,void
	> BasicType::convert(){
		visit(
			[this](auto &&val){*this=(type)val;},
			*this
		);
	}


#endif //LOGO__XWB__BASIC_TYPE_DETAIL