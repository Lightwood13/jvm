#include <gtest/gtest.h>

#include "../source/parser/parser.cpp"

#include <sstream>
#include <vector>
#include <cstddef>

using std::byte;

class HelperStream : public InStream
{
private:
	const std::string s;
	std::istringstream stream;
public:
	HelperStream(std::vector<byte>& s) : s(reinterpret_cast<const char*>(s.data()), s.size()), stream(this->s), InStream(stream)
	{}
};

TEST(Parser, ParseUTF8)
{
	std::vector<byte> v;
	v.push_back(static_cast<byte>(ConstantTag::CONSTANT_Utf8));
	v.push_back(static_cast<byte>(0));
	v.push_back(static_cast<byte>(3));
	v.push_back(static_cast<byte>('a'));
	v.push_back(static_cast<byte>('b'));
	v.push_back(static_cast<byte>('c'));
	HelperStream in(v);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Utf8);
	std::unique_ptr<ConstantUtf8> cutf8(static_cast<ConstantUtf8*>(c.release()));
	ASSERT_EQ(cutf8->get_contents(), "abc");
}

TEST(Parser, ParseInteger)
{
	std::vector<byte> v;
	v.push_back(static_cast<byte>(ConstantTag::CONSTANT_Integer));
	v.push_back(static_cast<byte>(0));
	v.push_back(static_cast<byte>(0));
	v.push_back(static_cast<byte>(0));
	v.push_back(static_cast<byte>(42));
	HelperStream in(v);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Integer);
	std::unique_ptr<ConstantInteger> cint(static_cast<ConstantInteger*>(c.release()));
	ASSERT_EQ(cint->get_contents(), 42);

	v.clear();
	v.push_back(static_cast<byte>(ConstantTag::CONSTANT_Integer));
	v.push_back(static_cast<byte>(0xff));
	v.push_back(static_cast<byte>(0xff));
	v.push_back(static_cast<byte>(0xff));
	v.push_back(static_cast<byte>(0xfe));
	HelperStream in2(v);
	c = parse_constant(in2);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Integer);
	cint = std::unique_ptr<ConstantInteger>(static_cast<ConstantInteger*>(c.release()));
	ASSERT_EQ(cint->get_contents(), -2);
}