//TODO comments


#pragma once

#ifndef LOGO__XWB__PARSER
#define LOGO__XWB__PARSER


#include <map>
#include <string>
#include <utility>


extern int current_line;

extern std::map<std::string,std::pair<int,int>> functions; //start_line,end_line

extern std::map<int,std::pair<int,int>> ifelses; //start_line,end_line,else_line

extern std::map<int,int> whiles; //start_line,end_line

extern std::map<int,int> repeats; //start_line,end_line


std::string function_name(const std::string &str,int cur);

bool is_function_name(const std::string &name);

void parse(int &cur);


#endif //LOGO__XWB__PARSER