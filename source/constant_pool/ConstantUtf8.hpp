#ifndef CONSTANT_UTF8_HPP
#define CONSTANT_UTF8_HPP

#include "ConstantBase.hpp"
#include <string_view>

class ConstantUtf8 : public ConstantBase
{
public:
	ConstantUtf8(std::string_view s) : contents(s) {}

	ConstantTag get_tag() const override { return ConstantTag::CONSTANT_Utf8; }
	std::string_view get_contents() { return contents; }

private:
	std::string contents;
};

#endif // CONSTANT_UTF8_HPP