//TODO comments


#include <cctype>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "basic_type.hpp"
#include "error.hpp"
#include "expression.h"
#include "literal.hpp"
#include "variable.hpp"


using std::map;
using std::nullopt;
using std::optional;
using std::string;
using std::vector;


namespace{

	vector<map<string,BasicType>> variable(1);


	//declaration

	BasicType* find(const string &name);

	bool is_variable_name(const string &name);


	//definition

	BasicType* find(const string &name){
		for(auto &&iter=variable.rbegin();iter!=variable.rend();++iter){
			auto ptr=iter->find(name);
			if(ptr!=iter->end())
				return &ptr->second;
		}
		return NULL;
	}

	bool is_variable_name(const string &name){
		for(auto &&iter:name){
			if(!islower(iter)) return false;
		}
		return true;
	}

} //namespace anonymous


//api functions

void allocate_scope(){variable.emplace_back();}

void allocate_variable(const string &name,BasicType val,int cur){
	if(!is_variable_name(name))
		invalid_variable_name(name,cur);
	map<string,BasicType> &current_scope=variable.back();
	if(current_scope.count(name))
		variable_redeclaration(name,cur);
	current_scope.emplace(name,std::move(val));
}

void clear_current_scope(){variable.back().clear();}

void free_scope(){variable.pop_back();}

BasicType& get(const string &name,int cur){
	BasicType *ptr=find(name);
	if(ptr==NULL) variable_not_declared(name,cur);
	return *ptr;
}

BasicType str_to_val(const string &str,int cur){
	if(is_variable_name(str))
		return get(str,cur);
	if(str.find('\'')!=string::npos)
		return BasicType(str_to_c(str,cur));
	if(str.find('.')!=string::npos)
		return BasicType(str_to_f(str,cur));
	if([&str]()->bool{
			for(auto &&iter:str){
				if(iter!='-' && !isdigit(iter))
					return false;
			}
			return true;
		}()
	) return BasicType(str_to_i(str,cur));
	if(str=="TRUE" || str=="FALSE")
		return BasicType(str_to_b(str,cur));
	if(str.front()=='{')
		return BasicType(expression::calc(str,cur));
	invalid_literaltype_value(str,cur);
}

optional<BasicType> watch(const string &name){
	BasicType *ptr=find(name);
	if(ptr==NULL) return nullopt;
	else return *ptr;
}