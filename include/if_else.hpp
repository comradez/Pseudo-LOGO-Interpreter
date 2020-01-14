//TODO comments


#pragma once

#ifndef LOGO__XWB__IF_ELSE
#define LOGO__XWB__IF_ELSE


#include <string>
#include <utility>
#include <vector>

#include "codeblock.hpp"


class IfElse:public Codeblock{
	public:
		IfElse(int _start_line,int _end_line,int _else_line=-1);

	private:
		static const std::pair<
			std::string,
			void (IfElse::*)(const std::vector<std::string>&,int&)
		> keywords[];

		int else_line;

		void deal_special(int &cur)override;
		bool is_special(int cur)override;

		void deal_else(const std::vector<std::string> &sentence,int &cur);
		void deal_end(const std::vector<std::string> &sentence,int &cur);
		void deal_if(const std::vector<std::string> &sentence,int &cur);
	//private end
};


#include "detail/if_else.ipp"


#endif //LOGO__XWB__IF_ELSE