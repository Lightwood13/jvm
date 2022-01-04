#ifndef CONSTANT_LONG_HPP
#define CONSTANT_LONG_HPP

#include <cstdint>
#include "ConstantBase.hpp"

class ConstantLong : public ConstantBase
{
public:
	ConstantLong(const int64_t val) : contents(val) {}

	ConstantTag get_tag() const override { return ConstantTag::CONSTANT_Long; }
	int64_t get_contents() const { return contents; }

private:
	const int64_t contents;
};

#endif // CONSTANT_LONG_HPP