//TODO comments


#pragma once

#ifndef LOGO__XWB__IF_ELSE_DETAIL
#define LOGO__XWB__IF_ELSE_DETAIL


#include <string>
#include <utility>
#include <vector>

#include "basic_type.hpp"
#include "codeblock.hpp"
#include "if_else.hpp"
#include "interface.hpp"
#include "parser.hpp"
#include "variable.hpp"


//public

inline IfElse::IfElse(int _start_line,int _end_line,int _else_line)
	:Codeblock(_start_line,_end_line),else_line(_else_line){}


//private

inline const std::pair<
	std::string,
	void (IfElse::*)(const std::vector<std::string>&,int&)
> IfElse::keywords[]={
	{"ELSE",&IfElse::deal_else},
	{"END",&IfElse::deal_end},
	{"IF",&IfElse::deal_if}
};

inline void IfElse::deal_special(int &cur){
	std::vector<std::string> sentence=get_line(cur);
	for(auto &&iter:keywords){
		if(sentence.front()==iter.first){
			(this->*iter.second)(sentence,cur);
			break;
		}
	}
}

inline bool IfElse::is_special(int cur){
	std::vector<std::string> sentence=get_line(cur);
	if(sentence.empty()) return false; //null statement
	for(auto &&iter:keywords){
		if(sentence.front()==iter.first)
			return true;
	}
	return false;
}

inline void IfElse::deal_else(const std::vector<std::string> &sentence,int &cur){
	cur= else_line==-1 ? end_line:else_line+1;
}

inline void IfElse::deal_end(const std::vector<std::string> &sentence,int &cur){
	if(sentence.back()!="IF")
		expected_end("IF",cur);
	cur=end_line+1;
}

inline void IfElse::deal_if(const std::vector<std::string> &sentence,int &cur){
	if(cur!=start_line){
		parse(cur);
		return;
	}
	BasicType val=str_to_val(sentence[1],start_line);
	if((boolean)val){
		else_line=-1;
		cur=start_line+1;
	}
	else cur= else_line==-1 ? end_line:else_line;
}


#endif //LOGO__XWB__IF_ELSE_DETAIL