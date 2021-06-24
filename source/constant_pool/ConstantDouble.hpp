#ifndef CONSTANT_DOUBLE_HPP
#define CONSTANT_DOUBLE_HPP

#include "ConstantBase.hpp"

class ConstantDouble : public ConstantBase
{
public:
	ConstantDouble(const double val) : contents(val) {}

	ConstantTag get_tag() const override { return ConstantTag::CONSTANT_Double; }
	double get_contents() const { return contents; }

private:
	const double contents;
};

#endif // CONSTANT_DOUBLE_HPP