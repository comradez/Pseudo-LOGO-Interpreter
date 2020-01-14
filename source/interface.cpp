//TODO comments


#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

#include "debug.hpp"
#include "error.hpp"
#include "interface.hpp"
#include "type_categories.hpp"
#include "zcy.h"


using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;


namespace{

	string file,dir;

} //namespace anonymous


namespace communicate{

	namespace LMY{

		//declaration

		void send_message(string info);


		//definition

		void send_message(string info){
			ofstream fout(filepath()+"/error.log");
			fout<<info;
			fout.close();
		}

	} //namespace LMY

	namespace ZCY{

		//api functions

#define operate_except_fraction(opt_lambda,opt_str)\
	overloaded{\
		opt_lambda,\
		[](fraction val1,auto &&val2)->BasicType{\
			operate_with_fractions(opt_str);\
		},\
		[](auto &&val1,fraction val2)->BasicType{\
			operate_with_fractions(opt_str);\
		},\
		[](fraction val1,fraction val2)->BasicType{\
			operate_with_fractions(opt_str);\
		}\
	}

		BasicType _and(const BasicType &val1,const BasicType &val2){
			auto opt=[](auto &&val1,auto &&val2)->BasicType{
				return ::_and(val1,val2);
			};
			return BasicType::visit(
				operate_except_fraction(opt,"AND"),
				val1,val2
			);
		}

		BasicType _not(const BasicType &val){
			return BasicType::visit(
				overloaded{
					[](auto &&val)->BasicType{
						return ::_not(val);
					},
					[](fraction val)->BasicType{
						operate_with_fractions("NOT");
					}
				},
				val
			);
		}

		BasicType _or(const BasicType &val1,const BasicType &val2){
			auto opt=[](auto &&val1,auto &&val2)->BasicType{
				return ::_or(val1,val2);
			};
			return BasicType::visit(
				operate_except_fraction(opt,"OR"),
				val1,val2
			);
		}

		BasicType _xor(const BasicType &val1,const BasicType &val2){
			auto opt=[](auto &&val1,auto &&val2)->BasicType{
				return ::_xor(val1,val2);
			};
			return BasicType::visit(
				operate_except_fraction(opt,"XOR"),
				val1,val2
			);
		}

		BasicType add(const BasicType &val1,const BasicType &val2){
			return BasicType::visit(
				[](auto &&val1,auto &&val2)->BasicType{
					return ::add(val1,val2);
				},
				val1,val2
			);
		}

		void arc(const BasicType &d,const BasicType &rad,const BasicType &angle){
			BasicType::visit(
				[](auto &&d,auto &&rad,auto &&angle)->void{
					::arc(d,rad,angle);
				},
				d,rad,angle
			);
		}

		void arg(const BasicType &angle){
			BasicType::visit(
				[](auto &&angle)->void{
					::arg(angle);
				},
				angle
			);
		}

		void cloak(){::cloak();}

		void color(const BasicType &r,const BasicType &g,const BasicType &b){
			BasicType::visit(
				[](auto &&r,auto &&g,auto &&b)->void{
					::color(r,g,b);
				},
				r,g,b
			);
		}

		BasicType div(const BasicType &val1,const BasicType &val2){
			return BasicType::visit(
				[](auto &&val1,auto &&val2)->BasicType{
					if(val2==0) divided_by_zero();
					return ::divide(val1,val2);
				},
				val1,val2
			);
		}

		BasicType eq(const BasicType &val1,const BasicType &val2){
			return BasicType::visit(
				[](auto &&val1,auto &&val2)->boolean{
					return ::eq(val1,val2);
				},
				val1,val2
			);
		}

		BasicType geq(const BasicType &val1,const BasicType &val2){
			return BasicType::visit(
				[](auto &&val1,auto &&val2)->boolean{
					return ::geq(val1,val2);
				},
				val1,val2
			);
		}

		BasicType gt(const BasicType &val1,const BasicType &val2){
			return BasicType::visit(
				[](auto &&val1,auto &&val2)->boolean{
					return ::gt(val1,val2);
				},
				val1,val2
			);
		}

		BasicType leq(const BasicType &val1,const BasicType &val2){
			return BasicType::visit(
				[](auto &&val1,auto &&val2)->boolean{
					return ::leq(val1,val2);
				},
				val1,val2
			);
		}

		BasicType lt(const BasicType &val1,const BasicType &val2){
			return BasicType::visit(
				[](auto &&val1,auto &&val2)->boolean{
					return ::lt(val1,val2);
				},
				val1,val2
			);
		}

		BasicType mod(const BasicType &val1,const BasicType &val2){
			auto opt=[](auto &&val1,auto &&val2)->BasicType{
				if(val2==0) divided_by_zero();
				return ::mod(val1,val2);
			};
			return BasicType::visit(
				operate_except_fraction(opt,"MOD"),
				val1,val2
			);
		}

		BasicType mul(const BasicType &val1,const BasicType &val2){
			return BasicType::visit(
				[](auto &&val1,auto &&val2)->BasicType{
					return ::multi(val1,val2);
				},
				val1,val2
			);
		}

		void move(const BasicType &step){
			BasicType::visit(
				[](auto &&step)->void{::move(step);},
				step
			);
		}

		BasicType neq(const BasicType &val1,const BasicType &val2){
			return BasicType::visit(
				[](auto &&val1,auto &&val2)->boolean{
					return ::neq(val1,val2);
				},
				val1,val2
			);
		}

		void paint(){::paint();}

		void pos(const BasicType &x,const BasicType &y){
			BasicType::visit(
				[](auto &&x,auto &&y)->void{::pos(x,y);},
				x,y
			);
		}

		void set_background(integer r,integer g,integer b){
			//TODO
		}

		void set_position(integer x,integer y){
			//TODO
		}

		void set_size(integer w,integer h){
			//TODO
		}

		BasicType sub(const BasicType &val1,const BasicType &val2){
			return BasicType::visit(
				[](auto &&val1,auto &&val2)->BasicType{
					return ::minus(val1,val2);
				},
				val1,val2
			);
		}

		void thick(const BasicType &value){
			BasicType::visit(
				[](auto &&value)->void{::thick(value);},
				value
			);
		}

		void tree(
			const BasicType &n,
			const BasicType &len,
			const BasicType &angle,
			const BasicType &delta,
			const BasicType &depth
		){
			BasicType::visit(
				[](auto &&n,auto &&len,auto &&angle,auto &&delta,auto &&depth)->void{
					::tree(n,len,angle,delta,depth);
				},
				n,len,angle,delta,depth
			);
		}

		void turn(const BasicType &angle){
			BasicType::visit(
				[](auto &&angle)->void{::turn(angle);},
				angle
			);
		}

#undef operate_except_fraction

	} //namespace ZCY

} //namespace communicate


//api functions

string filename(){return file;}

string filepath(){return dir;}

vector<string> get_line(int line_number){
	return split_space(line_number);
}

void halt(){
	check();
	communicate::LMY::send_message("halt without error.");
	debug::end();
}

int read(const string &_file){
	if(_file.empty()) return -1;
	if(ifstream test(_file);test.is_open())
		test.close();
	else return -1;
	file=_file;
	size_t pos=file.rfind('/');
	if(pos!=string::npos){
		dir=file.substr(0,pos);
		file=file.substr(pos+1);
	}
	else dir=".";
	return init(_file.c_str());
}

void throw_exception(string info){
	communicate::LMY::send_message(info);
	debug::end();
	exit(0);
}