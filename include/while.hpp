//TODO comments


#pragma once

#ifndef LOGO__XWB__WHILE
#define LOGO__XWB__WHILE


#include <string>
#include <utility>
#include <vector>

#include "loop.hpp"


class While:public Loop{
	public:
		While(int _start_line,int _end_line);
	//public end

	private:
		static const std::pair<
			std::string,
			void (While::*)(const std::vector<std::string>&,int&)
		> keywords[];

		void deal_special(int &cur)override;
		bool is_special(int cur)override;

		void deal_end(const std::vector<std::string> &sentence,int &cur);
		void deal_while(const std::vector<std::string> &sentence,int &cur);
	//private end
}; //class While


#include "detail/while.ipp"


#endif //LOGO__XWB__WHILE