//TODO comments


#pragma once

#ifndef LOGO__XWB__INTERFACE
#define LOGO__XWB__INTERFACE


#include <string>
#include <vector>

#include "basic_type.hpp"


namespace communicate{

	namespace ZCY{

		BasicType _and(const BasicType &val1,const BasicType &val2);

		BasicType _not(const BasicType &val);

		BasicType _or(const BasicType &val1,const BasicType &val2);

		BasicType _xor(const BasicType &val1,const BasicType &val2);

		BasicType add(const BasicType &val1,const BasicType &val2);

		void arc(const BasicType &d,const BasicType &rad,const BasicType &angle);

		void arg(const BasicType &angle);

		void cloak();

		void color(const BasicType &r,const BasicType &g,const BasicType &b);

		BasicType div(const BasicType &val1,const BasicType &val2);

		BasicType eq(const BasicType &val1,const BasicType &val2);

		BasicType geq(const BasicType &val1,const BasicType &val2);

		BasicType gt(const BasicType &val1,const BasicType &val2);

		BasicType leq(const BasicType &val1,const BasicType &val2);

		BasicType lt(const BasicType &val1,const BasicType &val2);

		BasicType mod(const BasicType &val1,const BasicType &val2);

		BasicType mul(const BasicType &val1,const BasicType &val2);

		void move(const BasicType &step);

		BasicType neq(const BasicType &val1,const BasicType &val2);

		void paint();

		void pos(const BasicType &x,const BasicType &y);

		void set_background(integer r,integer g,integer b);

		void set_position(integer x,integer y);

		void set_size(integer w,integer h);

		BasicType sub(const BasicType &val1,const BasicType &val2);

		void thick(const BasicType &value);

		void tree(
			const BasicType &n,
			const BasicType &len,
			const BasicType &angle,
			const BasicType &delta,
			const BasicType &depth
		);

		void turn(const BasicType &angle);

	} //namespace ZCY

} //namespace communicate


std::string filename();

std::string filepath();

std::vector<std::string> get_line(int line_number);

void halt();

int read(const std::string &file);

void throw_exception(std::string info);


#endif //LOGO__XWB__INTERFACE