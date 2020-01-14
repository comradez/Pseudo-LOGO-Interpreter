//TODO comments


#pragma once

#ifndef LOGO__XWB__LITERAL
#define LOGO__XWB__LITERAL


#include <string>

#include "basic_type.hpp"


boolean str_to_b(const std::string &str,int cur);

character str_to_c(const std::string &str,int cur);

fraction str_to_f(const std::string &str,int cur);

integer str_to_i(const std::string &str,int cur);

integer todigit(character c);


#include "detail/literal.ipp"


#endif //LOGO__XWB__LITERAL