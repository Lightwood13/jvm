#ifndef CONSTANT_INTEGER_HPP
#define CONSTANT_INTEGER_HPP

#include <cstdint>
#include "ConstantBase.hpp"

class ConstantInteger : public ConstantBase
{
public:
	ConstantInteger(const int32_t val) : contents(val) {}

	ConstantTag get_tag() const override { return ConstantTag::CONSTANT_Integer; }
	int32_t get_contents() const { return contents; }

private:
	const int32_t contents;
};

#endif // CONSTANT_INTEGER_HPP