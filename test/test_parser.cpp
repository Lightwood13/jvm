#include <gtest/gtest.h>

#include "../source/parser/parser.cpp"

#include <sstream>
#include <vector>
#include <cstddef>
#include <cstring>

using std::byte;

class HelperStream : public InStream
{
private:
	std::istringstream stream;
public:
	HelperStream(std::vector<uint8_t>& data) : InStream(stream)
	{
		std::string s(data.size(), 0);
		std::memcpy(s.data(), data.data(), data.size());
		stream = std::istringstream(s);
	}
};

TEST(Parser, ParseUTF8)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_Utf8),
		0, 3, 'a', 'b', 'c'
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Utf8);
	std::unique_ptr<ConstantUtf8> cutf8(static_cast<ConstantUtf8*>(c.release()));
	ASSERT_EQ(cutf8->get_contents(), "abc");
}

TEST(Parser, ParseInteger)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_Integer),
		0, 0, 0, 42
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Integer);
	std::unique_ptr<ConstantInteger> cint(static_cast<ConstantInteger*>(c.release()));
	ASSERT_EQ(cint->get_contents(), 42);

	data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_Integer),
		0xff, 0xff, 0xff, 0xfe
	};
	HelperStream in2(data);
	c = parse_constant(in2);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Integer);
	cint = std::unique_ptr<ConstantInteger>(static_cast<ConstantInteger*>(c.release()));
	ASSERT_EQ(cint->get_contents(), -2);
}

TEST(Parser, ParseFloat)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_Float),
		0x3e, 0xc0, 0, 0
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Float);
	std::unique_ptr<ConstantFloat> cint(static_cast<ConstantFloat*>(c.release()));
	ASSERT_EQ(cint->get_contents(), 0.375);
}


TEST(Parser, ParseLong)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_Long),
		0, 0, 0, 1, 0, 0, 0, 2
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Long);
	std::unique_ptr<ConstantLong> cint(static_cast<ConstantLong*>(c.release()));
	ASSERT_EQ(cint->get_contents(), 4294967298);

	data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_Long),
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe
	};
	HelperStream in2(data);
	c = parse_constant(in2);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Long);
	cint = std::unique_ptr<ConstantLong>(static_cast<ConstantLong*>(c.release()));
	ASSERT_EQ(cint->get_contents(), -2);
}


TEST(Parser, ParseDouble)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_Double),
		0xbf, 0xe4, 0, 0, 0, 0, 0, 0
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Double);
	std::unique_ptr<ConstantDouble> cint(static_cast<ConstantDouble*>(c.release()));
	ASSERT_EQ(cint->get_contents(), -0.625);
}