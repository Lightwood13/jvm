#ifndef CONSTANT_FLOAT_HPP
#define CONSTANT_FLOAT_HPP

#include "ConstantBase.hpp"

class ConstantFloat : public ConstantBase
{
public:
	ConstantFloat(const float val) : contents(val) {}

	ConstantTag get_tag() const override { return ConstantTag::CONSTANT_Float; }
	float get_contents() const { return contents; }

private:
	const float contents;
};

#endif // CONSTANT_FLOAT_HPP