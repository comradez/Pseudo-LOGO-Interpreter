//TODO comments


#pragma once

#ifndef LOGO__XWB__ERROR
#define LOGO__XWB__ERROR


#include <string>
#include <vector>


void command_outside_codeblock(
	const std::string &cmd,
	const std::string &block,
	int cur
);

void divided_by_zero();

void else_without_if(int cur);

void expected_begin(const std::string &begin_type,int cur);

void expected_end(const std::string &end_type,int cur);

void function_not_defined(const std::string &name,int cur);

void function_redefinition(const std::string &name,int cur);

void invalid_constant(const std::string &val,const std::string &type,int cur);

void invalid_function_name(const std::string &name,int cur);

void invalid_header(const std::vector<std::string> &expected,int cur);

void invalid_literaltype_value(const std::string &val,int cur);

void invalid_number_of_arguments(int cur,bool args_lt_paras);

void invalid_variable_name(const std::string &name,int cur);

void invalid_words_cnt(
	const std::string &first_word,
	const std::vector<std::string> &expected,
	int cur
);

void no_header();

void operate_with_fractions(const std::string &cmd);

void syntax_error(const std::string &first_word,int cur);

void variable_not_declared(const std::string &name,int cur);

void variable_redeclaration(const std::string &name,int cur);


#endif //LOGO__XWB__ERROR