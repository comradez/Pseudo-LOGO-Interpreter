//TODO comments


#pragma once

#ifndef LOGO__XWB__CODEBLOCK
#define LOGO__XWB__CODEBLOCK


class Codeblock{
	public:
		Codeblock(int _start_line,int _end_line);
		~Codeblock();

		void execute(int &cur);
	// public end

	protected:
		int start_line;
		int end_line;
		int stk_idx;
	// protected end

	private:
		virtual void deal_special(int &cur)=0;
		virtual bool is_special(int cur)=0;
	// private end

}; //class CodeBlock


#include "detail/codeblock.ipp"


#endif //LOGO__XWB__CODEBLOCK