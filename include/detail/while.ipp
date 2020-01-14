//TODO comments


#pragma once

#ifndef LOGO__XWB__WHILE_DETAIL
#define LOGO__XWB__WHILE_DETAIL


#include <string>
#include <utility>
#include <vector>

#include "basic_type.hpp"
#include "loop.hpp"
#include "parser.hpp"
#include "while.hpp"
#include "variable.hpp"


// public

inline While::While(int _start_line,int _end_line)
	:Loop(_start_line,_end_line){}


// private

inline const std::pair<
	std::string,
	void (While::*)(const std::vector<std::string>&,int&)
> While::keywords[]={
	{"WHILE",&While::deal_while},
	{"END",&While::deal_end}
};

inline void While::deal_special(int &cur){
	std::vector<std::string> sentence=get_line(cur);
	for(auto &&iter:keywords){
		if(sentence.front()==iter.first){
			(this->*iter.second)(sentence,cur);
			break;
		}
	}
}

inline bool While::is_special(int cur){
	std::vector<std::string> sentence=get_line(cur);
	if(sentence.empty()) return false; //null statement
	for(auto &&iter:keywords){
		if(sentence.front()==iter.first)
			return true;
	}
	return false;
}

inline void While::deal_end(const std::vector<std::string> &sentence,int &cur){
	if(sentence.back()!="WHILE")
		expected_end("WHILE",cur);
	cur=start_line;
}

inline void While::deal_while(const std::vector<std::string> &sentence,int &cur){
	if(cur!=start_line){
		parse(cur);
		return;
	}
	clear_current_scope();
	BasicType val=str_to_val(sentence[1],start_line);
	if((boolean)val) cur=start_line+1;
	else cur=end_line+1;
}


#endif //LOGO__XWB__WHILE_DETAIL