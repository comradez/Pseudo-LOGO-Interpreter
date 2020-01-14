//TODO comments


#pragma once

#ifndef LOGO__XWB__REPEAT
#define LOGO__XWB__REPEAT


#include "loop.hpp"


class Repeat:public Loop{
	public:
		Repeat(int _start_line,int _end_line);
	//public end

	private:
		static const std::pair<
			std::string,
			void (Repeat::*)(const std::vector<std::string>&,int&)
		> keywords[];

		int rest_time;

		void deal_special(int &cur)override;
		bool is_special(int cur)override;

		void deal_end(const std::vector<std::string> &sentence,int &cur);
		void deal_repeat(const std::vector<std::string> &sentence,int &cur);
	//private end
}; //class Repeat


#include "detail/repeat.ipp"


#endif //LOGO__XWB__REPEAT