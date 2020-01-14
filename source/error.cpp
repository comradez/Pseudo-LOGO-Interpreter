//TODO comments


#include <string>
#include <vector>

#include "error.hpp"
#include "interface.hpp"
#include "parser.hpp"


using std::string;
using std::to_string;
using std::vector;


namespace{
	//declaration

	string declare_error(int cur);


	//definition

	string declare_error(int cur){
		string info
			="[error] "
				+filename()
				+'-'
				+to_string(cur)
				+": ";
		return info;
	}

} //namespace anonymous


//api functions

void command_outside_codeblock(const string &cmd,const string &block,int cur){
	string info
		=declare_error(cur)
			+'\''
			+cmd
			+"' outside "
			+block
			+'.';
	throw_exception(info);
}

void divided_by_zero(){
	string info
		=declare_error(current_line)
			+"divisor '"
			+get_line(current_line)[2] //DIV [val1] [val2] [var]
			+"' is zero.";
	throw_exception(info);
}

void else_without_if(int cur){
	string info
		=declare_error(cur)
			+"'ELSE' without a previous 'IF'.";
	throw_exception(info);
}

void expected_begin(const string &begin_type,int cur){
	string info
		=declare_error(cur)
			+"'END "
			+begin_type
			+"' without a previous '"
			+begin_type
			+"'.";
	throw_exception(info);
}

void expected_end(const string &end_type,int cur){
	string info
		=declare_error(cur)
			+"expected 'END "
			+end_type
			+"' after token '"
			+end_type
			+"'.";
	throw_exception(info);
}

void function_not_defined(const string &name,int cur){
	string info
		=declare_error(cur)
			+"function '"
			+name
			+"' was not defined in this scope.";
	throw_exception(info);
}

void function_redefinition(const string &name,int cur){
	string info
		=declare_error(cur)
			+"redefinition of function '"
			+name
			+"'.";
	throw_exception(info);
}

void invalid_constant(const string &val,const string &type,int cur){
	string info
		=declare_error(cur)
			+"invalid "
			+type
			+" constant '"
			+val
			+"'.";
	throw_exception(info);
}

void invalid_function_name(const string &name,int cur){
	string info
		=declare_error(cur)
			+"function name '"
			+name
			+"' is invalid.";
	throw_exception(info);
}

void invalid_header(const vector<string> &expected,int cur){
	auto usage=[&expected]()->string{
		string ret;
		for(auto &&iter:expected)
			ret+=iter;
		return ret;
	};
	string info
		=declare_error(cur)
			+"header line #"
			+to_string(cur)
			+" should start with '"
			+usage()
			+"'.";
	throw_exception(info);
}

void invalid_literaltype_value(const string &val,int cur){
	string info
		=declare_error(cur)
			+"invalid literaltype value '"
			+val
			+"'.";
	throw_exception(info);
}

void invalid_number_of_arguments(int cur,bool args_lt_paras){
	string func=get_line(cur)[1];
	func=func.substr(0,func.find('(')-1);
	string info
		=declare_error(cur)
			+"too "
			+(args_lt_paras ? "few":"many")
			+" to function '"
			+func
			+"'.";
	throw_exception(info);
}

void invalid_variable_name(const string &name,int cur){
	string info
		=declare_error(cur)
			+"variable name '"
			+name
			+"' is invalid.";
	throw_exception(info);
}

void invalid_words_cnt(
	const string &first_word,
	const vector<string> &expected,
	int cur
){
	auto usage=[&expected]()->string{
		string ret;
		for(auto &&iter:expected)
			ret+=iter;
		return ret;
	};
	string info
		=declare_error(cur)
			+"expected "
			+to_string(expected.size())
			+" token(s) in statement '"
			+first_word
			+"...'. usage: "
			+usage();
	throw_exception(info);
}

void no_header(){
	string info
		="[error] "
			+filename()
			+": "
			+"expected '@SIZE...', \
				'@BACKGROUND...', \
				'@POSITION...' \
				in the first three lines.";
	throw_exception(info);
}

void operate_with_fractions(const string &cmd){
	string info
		=declare_error(current_line)
			+"invalid fraction type operand(s) to operator '"
			+cmd
			+"'.";
	throw_exception(info);
}

void syntax_error(const string &first_word,int cur){
	string info
		=declare_error(cur)
			+"'"
			+first_word
			+"...' has a syntax error.";
	throw_exception(info);
}

void variable_not_declared(const string &name,int cur){
	string info
		=declare_error(cur)
			+"variable '"
			+name
			+"' was not declared in this scope.";
	throw_exception(info);
}

void variable_redeclaration(const string &name,int cur){
	string info
		=declare_error(cur)
			+"redeclaration of variable '"
			+name
			+"'.";
	throw_exception(info);
}