//TODO comments


#pragma once

#ifndef LOGO__XWB__LITERAL_DETAIL
#define LOGO__XWB__LITERAL_DETAIL


#include <cctype>
#include <string>

#include "basic_type.hpp"
#include "error.hpp"
#include "literal.hpp"


inline boolean str_to_b(const std::string &str,int cur){
	static const std::string type("boolean");
	if(str=="TRUE") return true;
	if(str=="FALSE") return false;
	invalid_constant(str,type,cur);
}

inline character str_to_c(const std::string &str,int cur){
	static const std::string type("character");
	if(str.length()<3 || str.front()!='\'' || str.back()!='\'') //'x'
		invalid_constant(str,type,cur);
	if(str.length()==3){ //'x'
		if(str[1]=='\'' || str[1]=='\\')
			invalid_constant(str,type,cur);
		return str[1];
	}
	if(str.length()==4 && str[1]=='\\'){ //'\x'
		switch(str[1]){ //escape list is based on cppreference
			case '\'': return '\'';
			case '"': return '"';
			case '?': return '?';
			case '\\': return '\\';
			case 'a': return '\a';
			case 'b': return '\b';
			case 'f': return '\f';
			case 'n': return '\n';
			case 'r': return '\r';
			case 't': return '\t';
			case 'v': return '\v';
			default: invalid_constant(str,type,cur);
		}
	}
	invalid_constant(str,type,cur);
}

inline fraction str_to_f(const std::string &str,int cur){
	static const std::string type("fraction");
	size_t i=0; integer pm=1;
	if(str.front()=='-'){
		if(str.length()<2) //-x
			invalid_constant(str,type,cur);
		i=1,pm=-1;
	}
	fraction ret=0;
	for(;i<str.length();++i){
		if(isdigit(str[i])) ret=ret*10+todigit(str[i]);
		else if(str[i]!='.') invalid_constant(str,type,cur);
	}
	i=str.find('.');
	if(i!=std::string::npos){
		if(pm==-1 && str.length()==2) //-.
			invalid_constant(str,type,cur);
		if(str.find('.',i+1)!=std::string::npos) //xx.xx.xx
			invalid_constant(str,type,cur);
		while(++i<str.length()) ret/=10;
	}
	return pm*ret;
}

inline integer str_to_i(const std::string &str,int cur){
	static const std::string type("integer");
	size_t i=0; integer pm=1;
	if(str.front()=='-'){
		if(str.length()<2) //-x
			invalid_constant(str,type,cur);
		i=1,pm=-1;
	}
	integer ret=0;
	for(;i<str.length();++i){
		if(!isdigit(str[i])) invalid_constant(str,type,cur);
		ret=ret*10+todigit(str[i]);
	}
	return pm*ret;
}

inline integer todigit(character c){
	return isdigit(c) ? c-'0':c;
}


#endif //LOGO__XWB__LITERAL_DETAIL