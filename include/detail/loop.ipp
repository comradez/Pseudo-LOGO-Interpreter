//TODO comments

#pragma once

#ifndef LOGO__XWB__LOOP_DETAIL
#define LOGO__XWB__LOOP_DETAIL


#include <string>
#include <vector>

#include "codeblock.hpp"
#include "loop.hpp"


//public

Loop::Loop(int _start_line,int _end_line)
	:Codeblock(_start_line,_end_line){}

inline void Loop::deal_break(const std::vector<std::string> &sentence,int &cur){
	cur=end_line+1;
}

inline void Loop::deal_continue(const std::vector<std::string> &sentence,int &cur){
	cur=end_line;
}


#endif //LOGO__XWB__LOOP_DETAIL