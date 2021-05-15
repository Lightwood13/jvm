#include "parser.hpp"

#include <istream>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>

#include "../util/InStream.hpp"
#include "../constant_pool/ConstantBase.hpp"
#include "../constant_pool/ConstantUtf8.hpp"

std::unique_ptr<ConstantUtf8> parse_constant_utf8(InStream& stream);

std::unique_ptr<ConstantBase> parse_constant(InStream& stream)
{
	ConstantTag tag = static_cast<ConstantTag>(stream.get_u1());

	switch (tag)
	{
	case ConstantTag::CONSTANT_Utf8:
		return parse_constant_utf8(stream);
	default:
		throw std::logic_error(std::string("Unsupported constant type: ") + constant_tag_to_string(tag));
	}
}

std::unique_ptr<ConstantUtf8> parse_constant_utf8(InStream& stream)
{
	uint16_t length = stream.get_u2();
	std::string res(length, 0);
	stream.read(res.data(), length);
	return std::make_unique<ConstantUtf8>(std::move(res));
}