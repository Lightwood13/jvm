#include "parser.hpp"

#include <istream>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include "../util/util.hpp"
#include "../util/InStream.hpp"
#include "../constant_pool/ConstantBase.hpp"
#include "../constant_pool/ConstantUtf8.hpp"
#include "../constant_pool/ConstantInteger.hpp"
#include "../constant_pool/ConstantFloat.hpp"
#include "../constant_pool/ConstantLong.hpp"
#include "../constant_pool/ConstantDouble.hpp"
#include "../constant_pool/other_constants.hpp"                                                                           \

template <class ConstantClass>
std::unique_ptr<ConstantClass> parse_constant_one_index(InStream& stream)
{
	return std::make_unique<ConstantClass>(stream.get_u2());
}

template <class ConstantClass>
std::unique_ptr<ConstantClass> parse_constant_two_indices(InStream& stream)
{
	uint16_t idx1 = stream.get_u2();
	uint16_t idx2 = stream.get_u2();
	return std::make_unique<ConstantClass>(idx1, idx2);
}

std::unique_ptr<ConstantUtf8> parse_constant_utf8(InStream& stream);
std::unique_ptr<ConstantInteger> parse_constant_integer(InStream& stream);
std::unique_ptr<ConstantFloat> parse_constant_float(InStream& stream);
std::unique_ptr<ConstantLong> parse_constant_long(InStream& stream);
std::unique_ptr<ConstantDouble> parse_constant_double(InStream& stream);

std::unique_ptr<ConstantBase> parse_constant(InStream& stream)
{
	ConstantTag tag = static_cast<ConstantTag>(stream.get_u1());

	switch (tag)
	{
	case ConstantTag::CONSTANT_Utf8:
		return parse_constant_utf8(stream);
	case ConstantTag::CONSTANT_Integer:
		return parse_constant_integer(stream);
	case ConstantTag::CONSTANT_Float:
		return parse_constant_float(stream);
	case ConstantTag::CONSTANT_Long:
		return parse_constant_long(stream);
	case ConstantTag::CONSTANT_Double:
		return parse_constant_double(stream);
	case ConstantTag::CONSTANT_Class:
		return parse_constant_one_index<ConstantClass>(stream);
	case ConstantTag::CONSTANT_String:
		return parse_constant_one_index<ConstantString>(stream);
	case ConstantTag::CONSTANT_Fieldref:
		return parse_constant_two_indices<ConstantFieldref>(stream);
	case ConstantTag::CONSTANT_Methodref:
		return parse_constant_two_indices<ConstantMethodref>(stream);
	case ConstantTag::CONSTANT_InterfaceMethodref:
		return parse_constant_two_indices<ConstantInterfaceMethodref>(stream);
	case ConstantTag::CONSTANT_NameAndType:
		return parse_constant_two_indices<ConstantNameAndType>(stream);
	case ConstantTag::CONSTANT_MethodType:
		return parse_constant_one_index<ConstantMethodType>(stream);
	case ConstantTag::CONSTANT_Dynamic:
		return parse_constant_two_indices<ConstantDynamic>(stream);
	case ConstantTag::CONSTANT_InvokeDynamic:
		return parse_constant_two_indices<ConstantInvokeDynamic>(stream);
	case ConstantTag::CONSTANT_Module:
		return parse_constant_one_index<ConstantModule>(stream);
	case ConstantTag::CONSTANT_Package:
		return parse_constant_one_index<ConstantPackage>(stream);
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

std::unique_ptr<ConstantInteger> parse_constant_integer(InStream& stream)
{
	return std::make_unique<ConstantInteger>(stream.get_u4());
}

std::unique_ptr<ConstantFloat> parse_constant_float(InStream& stream)
{
	std::array<uint8_t, 4> data;
	data[0] = stream.get_u1();
	data[1] = stream.get_u1();
	data[2] = stream.get_u1();
	data[3] = stream.get_u1();
	return std::make_unique<ConstantFloat>(bytes_to_float(data));
}

std::unique_ptr<ConstantLong> parse_constant_long(InStream& stream)
{
	int64_t res = (static_cast<uint64_t>(stream.get_u4()) << 32)
		| stream.get_u4();
	return std::make_unique<ConstantLong>(res);
}

std::unique_ptr<ConstantDouble> parse_constant_double(InStream& stream)
{
	std::array<uint8_t, 8> data;
	data[0] = stream.get_u1();
	data[1] = stream.get_u1();
	data[2] = stream.get_u1();
	data[3] = stream.get_u1();
	data[4] = stream.get_u1();
	data[5] = stream.get_u1();
	data[6] = stream.get_u1();
	data[7] = stream.get_u1();
	return std::make_unique<ConstantDouble>(bytes_to_double(data));
}