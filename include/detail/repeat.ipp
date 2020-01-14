//TODO comments


#pragma once

#ifndef LOGO__XWB__REPEAT_DETAIL
#define LOGO__XWB__REPEAT_DETAIL


#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "basic_type.hpp"
#include "loop.hpp"
#include "parser.hpp"
#include "repeat.hpp"
#include "variable.hpp"


// public

inline Repeat::Repeat(int _start_line,int _end_line)
	:Loop(_start_line,_end_line)
{rest_time=-1;}


// private

inline const std::pair<
	std::string,
	void (Repeat::*)(const std::vector<std::string>&,int&)
> Repeat::keywords[]={
	{"REPEAT",&Repeat::deal_repeat},
	{"END",&Repeat::deal_end}
};

inline void Repeat::deal_special(int &cur){
	std::vector<std::string> sentence=get_line(cur);
	for(auto &&iter:keywords){
		if(sentence.front()==iter.first){
			(this->*iter.second)(sentence,cur);
			break;
		}
	}
}

inline bool Repeat::is_special(int cur){
	std::vector<std::string> sentence=get_line(cur);
	if(sentence.empty()) return false; //null statement
	for(auto &&iter:keywords){
		if(sentence.front()==iter.first)
			return true;
	}
	return false;
}

inline void Repeat::deal_end(const std::vector<std::string> &sentence,int &cur){
	if(sentence.back()!="REPEAT")
		expected_end("REPEAT",cur);
	--rest_time;
	cur=start_line;
}

inline void Repeat::deal_repeat(const std::vector<std::string> &sentence,int &cur){
	//REPEAT [val]
	if(cur!=start_line){
		parse(cur);
		return;
	}
	clear_current_scope();
	if(rest_time==-1){
		BasicType val=str_to_val(sentence[1],start_line);
		rest_time=std::max(0LL,(integer)val);
	}
	if(rest_time>0) cur=start_line+1;
	else cur=end_line+1;
}


#endif //LOGO__XWB__REPEAT_DETAIL