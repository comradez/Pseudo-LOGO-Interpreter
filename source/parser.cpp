//TODO comment


#include <cctype>
#include <map>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "debug.hpp"
#include "error.hpp"
#include "function.hpp"
#include "if_else.hpp"
#include "interface.hpp"
#include "parser.hpp"
#include "repeat.hpp"
#include "variable.hpp"
#include "while.hpp"


using std::map;
using std::pair;
using std::stack;
using std::string;
using std::vector;


namespace{

	stack<Function*> function_stk;

	stack<Loop*> loop_stk;


	//declaration

	void binary_op(
		const vector<string> &sentence,
		int &cur,
		BasicType operate(const BasicType&,const BasicType&)
	);

	void deal_add(const vector<string> &sentence,int &cur);

	void deal_and(const vector<string> &sentence,int &cur);

	void deal_arc(const vector<string> &sentence,int &cur);

	void deal_arg(const vector<string> &sentence,int &cur);

	void deal_boolean(const vector<string> &sentence,int &cur);

	void deal_break(const vector<string> &sentence,int &cur);

	void deal_call(const vector<string> &sentence,int &cur);

	void deal_character(const vector<string> &sentence,int &cur);

	void deal_cloak(const vector<string> &sentence,int &cur);

	void deal_color(const vector<string> &sentence,int &cur);

	void deal_continue(const vector<string> &sentence,int &cur);

	void deal_def(const vector<string> &sentence,int &cur);

	void deal_div(const vector<string> &sentence,int &cur);

	void deal_eq(const vector<string> &sentence,int &cur);

	void deal_fraction(const vector<string> &sentence,int &cur);

	void deal_func(const vector<string> &sentence,int &cur);

	void deal_geq(const vector<string> &sentence,int &cur);

	void deal_gt(const vector<string> &sentence,int &cur);

	void deal_ifelse(const vector<string> &sentence,int &cur);

	void deal_integer(const vector<string> &sentence,int &cur);

	void deal_leq(const vector<string> &sentence,int &cur);

	void deal_lt(const vector<string> &sentence,int &cur);

	void deal_mod(const vector<string> &sentence,int &cur);

	void deal_move(const vector<string> &sentence,int &cur);

	void deal_mul(const vector<string> &sentence,int &cur);

	void deal_neq(const vector<string> &sentence,int &cur);

	void deal_not(const vector<string> &sentence,int &cur);

	void deal_or(const vector<string> &sentence,int &cur);

	void deal_paint(const vector<string> &sentence,int &cur);

	void deal_pos(const vector<string> &sentence,int &cur);

	void deal_repeat(const vector<string> &sentence,int &cur);

	void deal_return(const vector<string> &sentence,int &cur);

	void deal_set(const vector<string> &sentence,int &cur);

	void deal_sub(const vector<string> &sentence,int &cur);

	void deal_thick(const vector<string> &sentence,int &cur);

	void deal_tree(const vector<string> &sentence,int &cur);

	void deal_turn(const vector<string> &sentence,int &cur);

	void deal_while(const vector<string> &sentence,int &cur);

	void deal_xor(const vector<string> &sentence,int &cur);


	//definition

	void binary_op(
		const vector<string> &sentence,
		int &cur,
		BasicType operate(const BasicType&,const BasicType&)
	){
		//OPT [val1] [val2] [var]
		BasicType &var=get(sentence[3],cur);
		BasicType val1=str_to_val(sentence[1],cur);
		BasicType val2=str_to_val(sentence[2],cur);
		var=operate(val1,val2);
	}

	void deal_add(const vector<string> &sentence,int &cur){
		//ADD [val1] [val2] [var]
		binary_op(sentence,cur,communicate::ZCY::add);
		++cur;
	}

	void deal_and(const vector<string> &sentence,int &cur){
		//AND [val1] [val2] [var]
		binary_op(sentence,cur,communicate::ZCY::_and);
		++cur;
	}

	void deal_arc(const vector<string> &sentence,int &cur){
		//ARC [d] [rad] [angle]
		BasicType d=str_to_val(sentence[1],cur);
		BasicType rad=str_to_val(sentence[2],cur);
		BasicType angle=str_to_val(sentence[3],cur);
		communicate::ZCY::arc(d,rad,angle);
		++cur;
	}

	void deal_arg(const vector<string> &sentence,int &cur){
		//ARG [angle]
		BasicType var=str_to_val(sentence[1],cur);
		communicate::ZCY::arg(var);
		++cur;
	}

	void deal_boolean(const vector<string> &sentence,int &cur){
		//BOOLEAN [var]
		BasicType &var=get(sentence[1],cur);
		var.convert<boolean>();
		++cur;
	}

	void deal_break(const vector<string> &sentence,int &cur){
		//BREAK
		if(loop_stk.empty())
			command_outside_codeblock("BREAK","loop",cur);
		loop_stk.top()->deal_break(sentence,cur);
	}

	void deal_call(const vector<string> &sentence,int &cur){
		//CALL [Name](arg1,arg2,...) [var]
		string name=function_name(sentence[1],cur);
		auto &&iter=functions.find(name);
		if(iter==functions.end() || iter->second.first>=cur)
			function_not_defined(name,cur);
		BasicType *ret_ptr=NULL;
		if(sentence[2]!="VOID")
			ret_ptr=&get(sentence[2],cur);
		Function function(iter->second.first,iter->second.second,cur,ret_ptr);
		function_stk.push(&function);
		function.execute(cur);
		function_stk.pop();
	}

	void deal_character(const vector<string> &sentence,int &cur){
		//CHARACTER [var]
		BasicType &var=get(sentence[1],cur);
		var.convert<character>();
		++cur;
	}

	void deal_cloak(const vector<string> &sentence,int &cur){
		//CLOAK
		communicate::ZCY::cloak();
		++cur;
	}

	void deal_color(const vector<string> &sentence,int &cur){
		//COLOR [r] [g] [b]
		BasicType r=str_to_val(sentence[1],cur);
		BasicType g=str_to_val(sentence[2],cur);
		BasicType b=str_to_val(sentence[3],cur);
		communicate::ZCY::color(r,g,b);
		++cur;
	}

	void deal_continue(const vector<string> &sentence,int &cur){
		//CONTINUE
		if(loop_stk.empty())
			command_outside_codeblock("CONTINUE","loop",cur);
		loop_stk.top()->deal_continue(sentence,cur);
	}

	void deal_def(const vector<string> &sentence,int &cur){
		//DEF [name] [val]
		BasicType val=str_to_val(sentence[2],cur);
		allocate_variable(sentence[1],val,cur);
		++cur;
	}

	void deal_div(const vector<string> &sentence,int &cur){
		//DIV [val1] [val2] [var]
		binary_op(sentence,cur,communicate::ZCY::div);
		++cur;
	}

	void deal_eq(const vector<string> &sentence,int &cur){
		//EQ [val1] [val2] [var]
		binary_op(sentence,cur,communicate::ZCY::eq);
		++cur;
	}

	void deal_fraction(const vector<string> &sentence,int &cur){
		//FRACTION [var]
		BasicType &var=get(sentence[1],cur);
		var.convert<fraction>();
		++cur;
	}

	void deal_func(const vector<string> &sentence,int &cur){
		cur=functions[function_name(sentence[1],cur)].second+1;
	}

	void deal_geq(const vector<string> &sentence,int &cur){
		//GEQ [val1] [val2] [var]
		binary_op(sentence,cur,communicate::ZCY::geq);
		++cur;
	}

	void deal_gt(const vector<string> &sentence,int &cur){
		//GT [val1] [val2] [var]
		binary_op(sentence,cur,communicate::ZCY::gt);
		++cur;
	}

	void deal_ifelse(const vector<string> &sentence,int &cur){
		//IF [val]
		pair<int,int> &lines=ifelses[cur];
		IfElse ifelse(cur,lines.first,lines.second);
		ifelse.execute(cur);
	}

	void deal_integer(const vector<string> &sentence,int &cur){
		//INTEGER [var]
		BasicType &var=get(sentence[1],cur);
		var.convert<integer>();
		++cur;
	}

	void deal_leq(const vector<string> &sentence,int &cur){
		//LEQ [val1] [val2] [var]
		binary_op(sentence,cur,communicate::ZCY::leq);
		++cur;
	}

	void deal_lt(const vector<string> &sentence,int &cur){
		//LT [val1] [val2] [var]
		binary_op(sentence,cur,communicate::ZCY::lt);
		++cur;
	}

	void deal_mod(const vector<string> &sentence,int &cur){
		//MOD [val1] [val2] [var]
		binary_op(sentence,cur,communicate::ZCY::mod);
		++cur;
	}

	void deal_move(const vector<string> &sentence,int &cur){
		//MOVE [step]
		BasicType step=str_to_val(sentence[1],cur);
		communicate::ZCY::move(step);
		++cur;
	}

	void deal_mul(const vector<string> &sentence,int &cur){
		//MUL [val1] [val2] [var]
		binary_op(sentence,cur,communicate::ZCY::mul);
		++cur;
	}

	void deal_neq(const vector<string> &sentence,int &cur){
		//NEQ [val1] [val2] [var]
		binary_op(sentence,cur,communicate::ZCY::neq);
		++cur;
	}

	void deal_not(const vector<string> &sentence,int &cur){
		//NOT [val] [var]
		BasicType &var=get(sentence[2],cur);
		BasicType val=str_to_val(sentence[1],cur);
		var=communicate::ZCY::_not(val);
		++cur;
	}

	void deal_or(const vector<string> &sentence,int &cur){
		//OR [val1] [val2] [var]
		binary_op(sentence,cur,communicate::ZCY::_or);
		++cur;
	}

	void deal_paint(const vector<string> &sentence,int &cur){
		//PAINT
		communicate::ZCY::paint();
		++cur;
	}

	void deal_pos(const vector<string> &sentence,int &cur){
		//POS [x] [y]
		BasicType val1=str_to_val(sentence[1],cur);
		BasicType val2=str_to_val(sentence[2],cur);
		communicate::ZCY::pos(val1,val2);
		++cur;
	}

	void deal_repeat(const vector<string> &sentence,int &cur){
		//REPEAT [val]
		Repeat repeat(cur,repeats[cur]);
		loop_stk.push(&repeat);
		repeat.execute(cur);
		loop_stk.pop();
	}

	void deal_return(const vector<string> &sentence,int &cur){
		//RETURN [val]/VOID
		if(function_stk.empty())
			command_outside_codeblock("RETURN","function",cur);
		function_stk.top()->deal_return(sentence,cur);
	}

	void deal_set(const vector<string> &sentence,int &cur){
		//SET [var] [val]
		BasicType &var=get(sentence[1],cur);
		BasicType val=str_to_val(sentence[2],cur);
		var=val;
		++cur;
	}

	void deal_sub(const vector<string> &sentence,int &cur){
		//SUB [val1] [val2] [var]
		binary_op(sentence,cur,communicate::ZCY::sub);
		++cur;
	}

	void deal_thick(const vector<string> &sentence,int &cur){
		//THICK [value]
		BasicType var=str_to_val(sentence[1],cur);
		communicate::ZCY::thick(var);
		++cur;
	}

	void deal_tree(const vector<string> &sentence,int &cur){
		//TREE [n] [len] [angle] [delta] [depth]
		BasicType n=str_to_val(sentence[1],cur);
		BasicType len=str_to_val(sentence[2],cur);
		BasicType angle=str_to_val(sentence[3],cur);
		BasicType delta=str_to_val(sentence[4],cur);
		BasicType depth=str_to_val(sentence[5],cur);
		communicate::ZCY::tree(n,len,angle,delta,depth);
		++cur;
	}

	void deal_turn(const vector<string> &sentence,int &cur){
		//TURN [angle]
		BasicType angle=str_to_val(sentence[1],cur);
		communicate::ZCY::turn(angle);
		++cur;
	}

	void deal_while(const vector<string> &sentence,int &cur){
		//LOOP [val]
		While _while(cur,whiles[cur]);
		loop_stk.push(&_while);
		_while.execute(cur);
		loop_stk.pop();
	}

	void deal_xor(const vector<string> &sentence,int &cur){
		//XOR [val1] [val2] [var]
		binary_op(sentence,cur,communicate::ZCY::_xor);
		++cur;
	}

} //namespace anonymous


//global variables

int current_line;

map<string,pair<int,int>> functions; //start_line,end_line

map<int,pair<int,int>> ifelses; //start_line,end_line,else_line

map<int,int> whiles; //start_line,end_line

map<int,int> repeats; //start_line,end_line


//api functions

bool is_function_name(const string &name){
	if(!isupper(name.front())) return false;
	for(size_t i=1;i<name.length();++i){
		if(!islower(name[i])) return false;
	}
	return true;
}

string function_name(const string &str,int cur){
	size_t pos=str.find('(');
	if(pos==string::npos) syntax_error("FUNC",cur);
	return str.substr(0,pos);
}

void parse(int &cur){
	static const pair<
		string,
		void (*)(const vector<string>&,int&)
	> keywords[]={
		//variable related
		{"BOOLEAN",deal_boolean},
		{"CHARACTER",deal_character},
		{"DEF",deal_def},
		{"FRACTION",deal_fraction},
		{"INTEGER",deal_integer},

		//operators
		{"ADD",deal_add},
		{"AND",deal_and},
		{"DIV",deal_div},
		{"EQ",deal_eq},
		{"GEQ",deal_geq},
		{"GT",deal_gt},
		{"LEQ",deal_leq},
		{"LT",deal_lt},
		{"MOD",deal_mod},
		{"MUL",deal_mul},
		{"NEQ",deal_neq},
		{"NOT",deal_not},
		{"OR",deal_or},
		{"SET",deal_set},
		{"SUB",deal_sub},
		{"XOR",deal_xor},

		//logo commands
		{"THICK",deal_thick},
		{"ARC",deal_arc},
		{"ARG",deal_arg},
		{"CLOAK",deal_cloak},
		{"COLOR",deal_color},
		{"MOVE",deal_move},
		{"PAINT",deal_paint},
		{"POS",deal_pos},
		{"TREE",deal_tree},
		{"TURN",deal_turn},

		//control sentences
		{"BREAK",deal_break},
		{"CALL",deal_call},
		{"CONTINUE",deal_continue},
		{"FUNC",deal_func},
		{"IF",deal_ifelse},
		{"WHILE",deal_while},
		{"REPEAT",deal_repeat},
		{"RETURN",deal_return}
	}; //array keywords
	vector<string> sentence=get_line(cur);
	if(sentence.empty() || sentence.front()=="#"){ //null statement or comment
		++cur;
		return;
	}
	if(sentence.front()=="$"){
		auto &&iter=sentence.erase(sentence.begin());
		if(*iter!="FUNC" && *iter!="$WATCH")
			debug::print(sentence,cur);
	}
	if(sentence.front()=="$WATCH"){
		debug::watch(sentence.back(),cur);
		++cur;
		return;
	}
	for(auto &&iter:keywords){
		if(sentence.front()==iter.first){
			iter.second(sentence,cur);
			return;
		}
	}
	syntax_error(sentence.front(),cur);
} //function parse