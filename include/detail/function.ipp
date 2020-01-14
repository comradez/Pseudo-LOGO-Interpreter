//TODO comments


#pragma once

#ifndef LOGO__XWB__FUNCTION_DETAIL
#define LOGO__XWB__FUNCTION_DETAIL


#include <string>
#include <utility>
#include <vector>

#include "basic_type.hpp"
#include "codeblock.hpp"
#include "function.hpp"
#include "interface.hpp"
#include "variable.hpp"


//public

inline Function::Function(
	int _start_line,
	int _end_line,
	int _caller_line,
	BasicType *_ret_ptr
):Codeblock(_start_line,_end_line),caller_line(_caller_line),ret_ptr(_ret_ptr){}

inline void Function::deal_return(const std::vector<std::string> &sentence,int &cur){
	if(sentence[1]!="VOID"){
		//when ret_ptr==NULL, this is needless
		//but str_to_val can validate the syntax
		BasicType val=str_to_val(sentence[1],cur);
		if(ret_ptr!=NULL) *ret_ptr=val;
	}
	cur=end_line;
}


//private

inline const std::pair<
	std::string,
	void (Function::*)(const std::vector<std::string>&,int&)
> Function::keywords[]={
	{"END",&Function::deal_end},
	{"FUNC",&Function::deal_func},
};

inline void Function::deal_special(int &cur){
	std::vector<std::string> sentence=get_line(cur);
	for(auto &&iter:keywords){
		if(sentence.front()==iter.first){
			(this->*iter.second)(sentence,cur);
			break;
		}
	}
}

inline bool Function::is_special(int cur){
	std::vector<std::string> sentence=get_line(cur);
	if(sentence.empty()) return false; //null statement
	for(auto &&iter:keywords){
		if(sentence.front()==iter.first)
			return true;
	}
	return false;
}

inline void Function::deal_end(const std::vector<std::string> &sentence,int &cur){
	if(sentence.back()!="FUNC")
		expected_end("FUNC",cur);
	end_line=-1;
	cur=caller_line+1;
}

inline void Function::deal_func(const std::vector<std::string> &sentence,int &cur){
	//expand parameter pack string"(a,b,c)" to vector{a,b,c}
	auto expand //TODO check
		=[](int cur,const char cmd[])->std::vector<std::string>{
			std::string parameters=get_line(cur)[1]; //FUNC/CALL [name](...) ***
			size_t lp=parameters.find('(')+1;
			size_t rp=parameters.find(')')-1;
			parameters=parameters.substr(lp,rp-lp+1);
			std::vector<std::string> ret;
			for(size_t i=0,j,len=parameters.length();i<len;i=j+1){
				j=parameters.find(',',i); //TODO 检查其它用string.find()的地方看参数顺序错没
				if(j==std::string::npos) j=len;
				size_t k=j-1;
				while(i<=k && parameters[i]==' ') ++i;
				while(k>=i && parameters[k]==' ') --k;
				if(i>k) syntax_error(cmd,cur);
				std::string val=parameters.substr(i,k-i+1);
				ret.emplace_back(std::move(val));
			}
			return ret;
		}; //lambda expand
	//initialize parameters
	std::vector<std::string> parameters=expand(start_line,"FUNC");
	std::vector<std::string> arguments=expand(caller_line,"CALL");
	if(arguments.size()!=parameters.size())
		invalid_number_of_arguments(
			caller_line,
			arguments.size()<parameters.size()
		);
	for(size_t i=0;i<parameters.size();++i){
		BasicType val=str_to_val(arguments[i],caller_line);
		allocate_variable(parameters[i],val,start_line);
	}
	cur=start_line+1;
} //function deal_func


#endif //LOGO__XWB__FUNCTION_DETAIL