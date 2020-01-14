//TODO comments


#pragma once

#ifndef LOGO__XWB__DEBUG
#define LOGO__XWB__DEBUG


#include <string>
#include <vector>


namespace debug{

	void end();

	void init(int _linenumber_width);

	void print(const std::vector<std::string> &sentence,int cur);

	void watch(const std::string &name,int cur);

} //namespace debug


#endif //LOGO__XWB__DEBUG