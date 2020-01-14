//TODO comments


#pragma once

#ifndef LOGO__XWB__CODEBLOCK_DETAIL
#define LOGO__XWB__CODEBLOCK_DETAIL


#include "codeblock.hpp"
#include "parser.hpp"
#include "variable.hpp"


/* public */

inline Codeblock::Codeblock(int _start_line,int _end_line)
	:start_line(_start_line),end_line(_end_line)
{allocate_scope();}

inline Codeblock::~Codeblock(){free_scope();}

inline void Codeblock::execute(int &cur){
	cur=start_line;
	while(cur<=end_line){
		if(is_special(cur)) deal_special(cur);
		else parse(cur);
	}
}


#endif //LOGO__XWB__CODEBLOCK_DETAIL