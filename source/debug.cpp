//TODO comments


#include <fstream>
#include <iomanip>
#include <optional>
#include <string>
#include <vector>

#include "basic_type.hpp"
#include "debug.hpp"
#include "interface.hpp"
#include "variable.hpp"


using std::nullopt;
using std::ofstream;
using std::optional;
using std::setw;
using std::string;
using std::vector;


namespace{

	int linenumber_width;

	ofstream dout;


	//declaration

	void print_linenumber(int cur);


	//definition

	void print_linenumber(int cur){
		dout<<setw(linenumber_width)<<cur<<": ";
	}

} //namespace anonymous

namespace debug{

	void end(){if(linenumber_width) dout.close();}

	void init(int _linenumber_width){
		linenumber_width=_linenumber_width;
		dout.open(filepath()+"/debug.log");
	}

	void print(const vector<string> &sentence,int cur){
		print_linenumber(cur);
		for(auto &&word:sentence)
			dout<<word<<' ';
		dout<<'\n';
	}

	void watch(const std::string &name,int cur){
		print_linenumber(cur);
		optional<BasicType> val=::watch(name);
		if(val)
			dout<<'['<<name<<"] = "<<*val<<'\n';
		else dout<<"[watch error]: '"<<name<<"' is not a valid variable.\n";
	}

} //namespace debug