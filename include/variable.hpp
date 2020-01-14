//TODO comments


#pragma once

#ifndef LOGO__XWB__VARIABLE
#define LOGO__XWB__VARIABLE


#include <optional>
#include <string>

#include "basic_type.hpp"


void allocate_scope();

void allocate_variable(const std::string &name,BasicType val,int cur);

void clear_current_scope();

void free_scope();

BasicType& get(const std::string &str,int cur);

BasicType str_to_val(const std::string &str,int cur);

std::optional<BasicType> watch(const std::string &name);


#endif //LOGO__XWB__VARIABLE