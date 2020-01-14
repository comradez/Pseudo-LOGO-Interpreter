#include <cmath>
#include <vector>
#include <string>
#include "basic_type.hpp"

template <typename Type1, typename Type2>
inline auto add (Type1 a, Type2 b) {
	return (expr_type_t<Type1, Type2>)(a + b);
}

template <typename Type1, typename Type2>
inline auto minus (Type1 a, Type2 b) {
	return (expr_type_t<Type1, Type2>)(a - b);
}

template <typename Type1, typename Type2>
inline auto multi (Type1 a, Type2 b) {
	return (expr_type_t<Type1, Type2>)(a * b);
}

template <typename Type1, typename Type2>
inline auto divide (Type1 a, Type2 b) {
	return (expr_type_t<Type1, Type2>)(a / b);
}

template <typename Type1, typename Type2>
inline auto mod (Type1 a, Type2 b) {
	return (expr_type_t<Type1, Type2>)(a % b);
}
template <typename Type>
inline auto mod (Type a, fraction b) {
	return std::fmod(a,b);
}
template <typename Type>
inline auto mod (fraction a, Type b) {
	return std::fmod(a,b);
}

template <typename Type1, typename Type2>
inline auto _and (Type1 a, Type2 b) {
	return (expr_type_t<Type1, Type2>)(a & b);
}
template<>
inline auto _and (boolean a, boolean b) {
	return a && b;
}

template <typename Type1, typename Type2>
inline auto _or (Type1 a, Type2 b) {
	return (expr_type_t<Type1, Type2>)(a | b);
}
template<>
inline auto _or (boolean a, boolean b) {
	return a || b;
}

template <typename Type1, typename Type2>
inline auto _xor (Type1 a, Type2 b) {
	return (expr_type_t<Type1, Type2>)(a ^ b);
}
template<>
inline auto _xor (boolean a, boolean b) {
	return a != b;
}

template <typename Type>
inline Type _not (Type a) {
	return (expr_type_t<Type>)(~a);
}
template<>
inline boolean _not (boolean a) {
	return !a;
}

template <typename Type1, typename Type2>
inline boolean eq (Type1 a, Type2 b) {
	return a == b;
}

template <typename Type1, typename Type2>
inline boolean neq (Type1 a, Type2 b) {
	return a != b;
}

template <typename Type1, typename Type2>
inline boolean leq (Type1 a, Type2 b) {
	return a <= b;
}

template <typename Type1, typename Type2>
inline boolean lt (Type1 a, Type2 b) {
	return a < b;
}

template <typename Type1, typename Type2>
inline boolean geq (Type1 a, Type2 b) {
	return a >= b;
}

template <typename Type1, typename Type2>
inline boolean gt (Type1 a, Type2 b) {
	return a > b;
}

void cloak();

void turn(double angle);

void color(int r, int g, int b);

void move(double step);

void paint();

void circle(int dir, double radius);

void arc(int dir, double radius, double angle);

void tree(int num, double trunk_length, double angle, double delta, int dep);

int init(const char file[]);

void pos(double x, double y);

void arg(double angle);

std::vector<std::string> split_space(int num);

void check();

void thick(int t);