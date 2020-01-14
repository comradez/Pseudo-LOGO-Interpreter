//TODO comments

#pragma once

#ifndef LOGO__XWB__LOOP
#define LOGO__XWB__LOOP


#include <string>
#include <vector>

#include "codeblock.hpp"


class Loop:public Codeblock{
	public:
		Loop(int _start_line,int _end_line);

		void deal_break(const std::vector<std::string> &sentence,int &cur);
		void deal_continue(const std::vector<std::string> &sentence,int &cur);
	//public end
};


#include "detail/loop.ipp"


#endif //LOGO__XWB__LOOP