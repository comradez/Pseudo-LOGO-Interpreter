//TODO comments


#pragma once

#ifndef LOGO__XWB__FUNCTION
#define LOGO__XWB__FUNCTION


#include <string>
#include <utility>
#include <vector>

#include "basic_type.hpp"
#include "codeblock.hpp"


//Function

class Function:public Codeblock{
	public:
		Function(int _start_line,int _end_line,int _caller_line,BasicType *_ret_ptr);

		void deal_return(const std::vector<std::string> &sentence,int &cur);
	//public end

	private:
		static const std::pair<
			std::string,
			void (Function::*)(const std::vector<std::string>&,int&)
		> keywords[];

		int caller_line;
		BasicType *ret_ptr;

		void deal_special(int &cur)override;
		bool is_special(int cur)override;

		void deal_end(const std::vector<std::string> &sentence,int &cur);
		void deal_func(const std::vector<std::string> &sentence,int &cur);
	//private end
}; //class Function


#include "detail/function.ipp"


#endif //LOGO__XWB__FUNCTION