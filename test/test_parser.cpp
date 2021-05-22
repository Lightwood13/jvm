#include <gtest/gtest.h>

#include "../source/parser/parser.cpp"

#include <sstream>


class HelperStream : public InStream
{
private:
	std::istringstream stream;
public:
	HelperStream(const std::string& s) : stream(s), InStream(stream)
	{}
};

TEST(Parser, ParseUTF8)
{
	std::string s;
	s += static_cast<uint8_t>(ConstantTag::CONSTANT_Utf8);
	s += (char)0;
	s += (char)3;
	s += "abc";
	HelperStream in(s);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Utf8);
	std::unique_ptr<ConstantUtf8> cutf8(dynamic_cast<ConstantUtf8*>(c.release()));
	ASSERT_EQ(cutf8->get_contents(), "abc");
}