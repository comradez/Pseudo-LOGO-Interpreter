//TODO comments


#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "debug.hpp"
#include "error.hpp"
#include "interface.hpp"
#include "literal.hpp"
#include "parser.hpp"


using std::ceil;
using std::cerr;
using std::log10;
using std::make_pair;
using std::map;
using std::pair;
using std::set;
using std::stack;
using std::string;
using std::vector;


namespace{

	constexpr int HEADER_LINE=4;

	//declaration

	bool basic_syntax_check(int end_line);

	void find_functions(int end_line);

	void find_ifelses(int end_line);

	void find_loops(map<int,int> &loops,const string &keyword,int end_line);

	int initialize(int end_line);

	void validate_words_cnt(
		const vector<string> &sentence,
		const vector<string> &expected,
		int cur
	);


	//definiton

	bool basic_syntax_check(int end_line){ //TODO modify
		//TODO invalid characters
		const set<string> var_op(
			{
				"DEF",
				"SET"
			}
		); //set var_op
		const set<string> cast_op(
			{
				"BOOLEAN",
				"CHARACTER",
				"FRACTION",
				"INTEGER",
			}
		); //set cast_op
		const set<string> unary_op({"NOT"});
		const set<string> binary_op(
			{
				"ADD",
				"AND",
				"DIV",
				"EQ",
				"GEQ",
				"GT",
				"LEQ",
				"LT",
				"MOD",
				"MUL",
				"NEQ",
				"OR",
				"SUB",
				"XOR"
			}
		); //set binary_op
		const set<string> ctrls(
			{
				"IF",
				"REPEAT",
				"WHILE"
			}
		); //set ctrls
		bool debug_flag=false;
		for(int i=HEADER_LINE+1;i<=end_line;++i){
			vector<string> sentence=get_line(i);
			if(sentence.empty() || sentence.front()=="#") continue;
			if(sentence.front()=="$"){
				sentence.erase(sentence.begin());
				debug_flag=true;
			}
			const string &opt=sentence.front();
			if(var_op.count(opt))
				validate_words_cnt(sentence,{opt,"[var]","[val]"},i);
			else if(cast_op.count(opt))
				validate_words_cnt(sentence,{opt,"[var]"},i);
			else if(unary_op.count(opt))
				validate_words_cnt(sentence,{opt,"[val]","[var]"},i);
			else if(binary_op.count(opt))
				validate_words_cnt(sentence,{opt,"[val1]","[val2]","[var]"},i);
			else if(ctrls.count(opt))
				validate_words_cnt(sentence,{opt,"[val]"},i);
			else if(opt=="FUNC")
				validate_words_cnt(sentence,{opt,"[Func](arg1,arg2,...)"},i);
			else if(opt=="BREAK")
				validate_words_cnt(sentence,{opt},i);
			else if(opt=="ELSE")
				validate_words_cnt(sentence,{opt},i);
			else if(opt=="END")
				validate_words_cnt(sentence,{opt,"[FUNC/IF/REPEAT/WHILE]"},i);
			else if(opt=="POS")
				validate_words_cnt(sentence,{opt,"[val1]","[val2]"},i);
			else if(opt=="ARG")
				validate_words_cnt(sentence,{opt,"[angle]"},i);
			else if(opt=="ARC")
				validate_words_cnt(sentence,{opt,"[dir]","[rad]","[angle]"},i);
			else if(opt=="TREE")
				validate_words_cnt(
					sentence,
					{opt,"[n]","[len]","[angle]","[delta]","[depth]"},
					i
				);
			else if(opt=="THICK")
				validate_words_cnt(sentence,{opt,"[value]"},i);
			else if(opt=="CALL")
				validate_words_cnt(
					sentence,
					{opt,"[Func](arg1,arg2,...)","[var]/VOID"},
					i
				);
			else if(opt=="RETURN")
				validate_words_cnt(sentence,{opt,"[val]/VOID"},i);
			else if(opt=="CONTINUE")
				validate_words_cnt(sentence,{opt},i);
			//logo commands
			else if(opt=="CLOAK")
				validate_words_cnt(sentence,{opt},i);
			else if(opt=="COLOR")
				validate_words_cnt(sentence,{opt,"[r]","[g]","[b]"},i);
			else if(opt=="MOVE")
				validate_words_cnt(sentence,{opt,"[step]"},i);
			else if(opt=="PAINT")
				validate_words_cnt(sentence,{opt},i);
			else if(opt=="TURN")
				validate_words_cnt(sentence,{opt,"[angle]"},i);
			else if(opt=="$WATCH"){
				validate_words_cnt(sentence,{opt,"[var]"},i);
				debug_flag=true;
			}
			else syntax_error(opt,i);
		} //for(...i<=end_line...)
		return debug_flag;
	} //function basic_syntax_check

	void find_functions(int end_line){
		int begin=-1;
		for(int i=1;i<=end_line;++i){
			vector<string> sentence=get_line(i);
			if(sentence.empty()) continue;
			if(sentence.front()=="FUNC"){
				if(begin!=-1) expected_end("FUNC",begin);
				validate_words_cnt(
					sentence,
					{"FUNC","[name](arg1,arg2,...)"},
					i
				);
				begin=i;
			}
			else if(sentence.front()=="END" && sentence.back()=="FUNC"){
				if(begin==-1) expected_begin("FUNC",i);
				string name=function_name(get_line(begin)[1],begin);
				if(!is_function_name(name)) invalid_function_name(name,begin);
				if(functions.count(name)) function_redefinition(name,begin);
				functions.emplace(name,make_pair(begin,i));
				begin=-1;
			}
		} //for(...i<=end_line...)
		if(begin!=-1) expected_end("FUNC",begin);
	} //function find_funcs

	void find_ifelses(int end_line){
		stack<pair<int,int>> stk; //start_line,else_line
		for(int i=1;i<=end_line;++i){
			vector<string> sentence=get_line(i);
			if(sentence.empty()) continue;
			if(sentence.front()=="IF") stk.emplace(i,-1);
			else if(sentence.front()=="ELSE"){
				if(stk.empty()) else_without_if(i);
				stk.top().second=i;
			}
			else if(sentence.front()=="END" && sentence.back()=="IF"){
				if(stk.empty()) expected_begin("IF",i);
				ifelses.emplace(stk.top().first,make_pair(i,stk.top().second));
				stk.pop();
			}
		} //for(...i<=end_line...)
		if(!stk.empty()) expected_end("IF",stk.top().first);
	} //function find_ifelses

	void find_loops(map<int,int> &loops,const string &keyword,int end_line){
		stack<int> stk;
		for(int i=1;i<=end_line;++i){
			vector<string> sentence=get_line(i);
			if(sentence.empty()) continue;
			if(sentence.front()==keyword) stk.push(i);
			else if(sentence.front()=="END" && sentence.back()==keyword){
				if(stk.empty()) expected_begin(keyword,i);
				loops.emplace(stk.top(),i);
				stk.pop();
			}
		} //for(...i<=end_line...)
		if(!stk.empty()) expected_end(keyword,stk.top());
	} //function find_loops

	int initialize(int end_line){
		if(end_line<HEADER_LINE) no_header();
		vector<string> sentence;
		auto syntax_check=[&sentence](
			const vector<string> &expected,
			int cur
		)->void{
			if(sentence.empty() || sentence.front()!=expected.front())
				invalid_header(expected,cur);
			validate_words_cnt(sentence,expected,cur);
		};

		//header line #1
		sentence=get_line(1);
		syntax_check({"@SIZE","[w]","[h]"},1);
		communicate::ZCY::set_size(
			str_to_i(sentence[1],1),
			str_to_i(sentence[2],1)
		);

		//header line #2
		sentence=get_line(2);
		syntax_check({"@BACKGROUND","[r]","[g]","[b]"},2);
		communicate::ZCY::set_background(
			str_to_i(sentence[1],2),
			str_to_i(sentence[2],2),
			str_to_i(sentence[3],2)
		);

		//header line #3
		sentence=get_line(3);
		syntax_check({"@POSITION","[x]","[y]"},3);
		communicate::ZCY::set_position(
			str_to_i(sentence[1],3),
			str_to_i(sentence[2],3) //TODO validate position
		);

		//TODO header line #4

		return HEADER_LINE+1;
	} //function initialize

	void validate_words_cnt(
		const vector<string> &sentence,
		const vector<string> &expected,
		int cur
	){
		if(sentence.size()!=expected.size())
			invalid_words_cnt(sentence.front(),expected,cur);
	} //function validate_words_cnt

} //namespace anonymous


//main function

int main(int argc,char *argv[]){
#ifdef DEBUG
	int end_line=read("test.logo");
#else
	int end_line=read(argv[1]);
	if(end_line<0){
		cerr<<"no such file.\n";
		return 0;
	}
#endif
	if(basic_syntax_check(end_line))
		debug::init(ceil(log10(end_line+1)));
	find_functions(end_line);
	find_loops(whiles,"WHILE",end_line);
	find_loops(repeats,"REPEAT",end_line);
	find_ifelses(end_line);

	//current_line=initialize(end_line);
	current_line=HEADER_LINE+1;
	while(current_line<=end_line)
		parse(current_line);
	halt();
	return 0;
} //function main