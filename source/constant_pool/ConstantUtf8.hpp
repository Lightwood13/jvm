#ifndef CONSTANT_UTF8_HPP
#define CONSTANT_UTF8_HPP

#include "ConstantBase.hpp"
#include <string_view>

class ConstantUtf8 : public ConstantBase
{
public:
	ConstantUtf8(const std::string_view val) : contents(val) {}
	ConstantUtf8(std::string&& val) : contents(val) {}

	ConstantTag get_tag() const override { return ConstantTag::CONSTANT_Utf8; }
	std::string_view get_contents() const { return contents; }

private:
	const std::string contents;
};

#endif // CONSTANT_UTF8_HPP