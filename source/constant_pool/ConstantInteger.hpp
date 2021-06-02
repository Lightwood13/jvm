#ifndef CONSTANT_INTEGER_HPP
#define CONSTANT_INTEGER_HPP

#include <cstdint>
#include "ConstantBase.hpp"

class ConstantInteger : public ConstantBase
{
public:
	ConstantInteger(int32_t i) : contents(i) {}

	ConstantTag get_tag() const override { return ConstantTag::CONSTANT_Integer; }
	int32_t get_contents() { return contents; }

private:
	int32_t contents;
};

#endif // CONSTANT_INTEGER_HPP