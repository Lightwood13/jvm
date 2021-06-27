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
	std::unique_ptr<ConstantFloat> cfloat(static_cast<ConstantFloat*>(c.release()));
	ASSERT_EQ(cfloat->get_contents(), 0.375);
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
	std::unique_ptr<ConstantLong> clong(static_cast<ConstantLong*>(c.release()));
	ASSERT_EQ(clong->get_contents(), 4294967298);

	data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_Long),
		0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xfe, 0xff
	};
	HelperStream in2(data);
	c = parse_constant(in2);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Long);
	clong = std::unique_ptr<ConstantLong>(static_cast<ConstantLong*>(c.release()));
	ASSERT_EQ(clong->get_contents(), -64424509697);
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
	std::unique_ptr<ConstantDouble> cdouble(static_cast<ConstantDouble*>(c.release()));
	ASSERT_EQ(cdouble->get_contents(), -0.625);
}


TEST(Parser, ParseClass)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_Class),
		0, 42
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Class);
	std::unique_ptr<ConstantClass> cclass(static_cast<ConstantClass*>(c.release()));
	ASSERT_EQ(cclass->get_name_index(), 42);
}


TEST(Parser, ParseString)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_String),
		0, 42
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_String);
	std::unique_ptr<ConstantString> cstring(static_cast<ConstantString*>(c.release()));
	ASSERT_EQ(cstring->get_string_index(), 42);
}


TEST(Parser, ParseFieldref)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_Fieldref),
		0, 42,
		0, 10
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Fieldref);
	std::unique_ptr<ConstantFieldref> cfref(static_cast<ConstantFieldref*>(c.release()));
	ASSERT_EQ(cfref->get_class_index(), 42);
	ASSERT_EQ(cfref->get_name_and_type_index(), 10);
}



TEST(Parser, ParseMethodref)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_Methodref),
		0, 42,
		0, 10
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Methodref);
	std::unique_ptr<ConstantMethodref> cmref(static_cast<ConstantMethodref*>(c.release()));
	ASSERT_EQ(cmref->get_class_index(), 42);
	ASSERT_EQ(cmref->get_name_and_type_index(), 10);
}


TEST(Parser, ParseInterfaceMethodref)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_InterfaceMethodref),
		0, 42,
		0, 10
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_InterfaceMethodref);
	std::unique_ptr<ConstantInterfaceMethodref> cimref(static_cast<ConstantInterfaceMethodref*>(c.release()));
	ASSERT_EQ(cimref->get_class_index(), 42);
	ASSERT_EQ(cimref->get_name_and_type_index(), 10);
}


TEST(Parser, ParseNameAndType)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_NameAndType),
		0, 42,
		0, 10
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_NameAndType);
	std::unique_ptr<ConstantNameAndType> cnametype(static_cast<ConstantNameAndType*>(c.release()));
	ASSERT_EQ(cnametype->get_name_index(), 42);
	ASSERT_EQ(cnametype->get_descriptor_index(), 10);
}


TEST(Parser, ParseMethodType)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_MethodType),
		0, 42
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_MethodType);
	std::unique_ptr<ConstantMethodType> cmtype(static_cast<ConstantMethodType*>(c.release()));
	ASSERT_EQ(cmtype->get_descriptor_index(), 42);
}


TEST(Parser, ParseDynamic)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_Dynamic),
		0, 42,
		0, 10
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Dynamic);
	std::unique_ptr<ConstantDynamic> cdyn(static_cast<ConstantDynamic*>(c.release()));
	ASSERT_EQ(cdyn->get_bootstrap_method_attr_index(), 42);
	ASSERT_EQ(cdyn->get_name_and_type_index(), 10);
}


TEST(Parser, ParseInvokeDynamic)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_InvokeDynamic),
		0, 42,
		0, 10
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_InvokeDynamic);
	std::unique_ptr<ConstantInvokeDynamic> cinvdyn(static_cast<ConstantInvokeDynamic*>(c.release()));
	ASSERT_EQ(cinvdyn->get_bootstrap_method_attr_index(), 42);
	ASSERT_EQ(cinvdyn->get_name_and_type_index(), 10);
}


TEST(Parser, ParseModule)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_Module),
		0, 42
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Module);
	std::unique_ptr<ConstantModule> cmodule(static_cast<ConstantModule*>(c.release()));
	ASSERT_EQ(cmodule->get_name_index(), 42);
}


TEST(Parser, ParsePackage)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_Package),
		0, 42
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Package);
	std::unique_ptr<ConstantPackage> cpkg(static_cast<ConstantPackage*>(c.release()));
	ASSERT_EQ(cpkg->get_name_index(), 42);
}