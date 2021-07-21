#include <gtest/gtest.h>

#include "../source/parser/parser.cpp"

#include <fstream>
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
		0x40, 0x0f, 0xff, 0xff, 0xfc, 0, 0, 0
	};
	HelperStream in(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_Double);
	std::unique_ptr<ConstantDouble> cdouble(static_cast<ConstantDouble*>(c.release()));
	ASSERT_EQ(cdouble->get_contents(), 3.9999999701976776123046875);
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


TEST(Parser, ParseMethodHandle)
{
	std::vector<uint8_t> data = {
		static_cast<uint8_t>(ConstantTag::CONSTANT_MethodHandle),
		42, 0, 42
	};
	HelperStream in(data);
	ASSERT_THROW(
	{
		try
		{ std::unique_ptr<ConstantBase> c = parse_constant(in); }
		catch (const std::logic_error& e)
		{
			ASSERT_STREQ("Unresolved reference kind: 42", e.what());
			throw;
		}
	}, std::logic_error);

	data[1] = static_cast<uint8_t>(ConstantMethodHandle::ReferenceKind::REF_getField);
	HelperStream in2(data);
	std::unique_ptr<ConstantBase> c = parse_constant(in2);
	ASSERT_EQ(c->get_tag(), ConstantTag::CONSTANT_MethodHandle);
	std::unique_ptr<ConstantMethodHandle> cmh(static_cast<ConstantMethodHandle*>(c.release()));
	ASSERT_EQ(cmh->get_reference_kind(), ConstantMethodHandle::ReferenceKind::REF_getField);
	ASSERT_EQ(cmh->get_reference_index(), 42);
}


TEST(Parser, ParseUnsupportedConstant)
{
	std::vector<uint8_t> data = { 42 };
	HelperStream in(data);
	ASSERT_THROW(
	{
		try
		{ std::unique_ptr<ConstantBase> c = parse_constant(in); }
		catch (const std::logic_error& e)
		{
			ASSERT_STREQ("Unsupported constant type: 42", e.what());
			throw;
		}
	}, std::logic_error);
}


TEST(Parser, ParseConstantPool)
{
	std::ifstream inFile("../../../test/resources/TestConstant.class");
	ASSERT_EQ(inFile.is_open(), true);
	inFile.seekg(8);
	InStream inStream(inFile);
	ConstantPool cp = parse_constant_pool(inStream);
	ASSERT_THROW(
	{
		try
		{ cp[0]; }
		catch (const std::invalid_argument& e)
		{
			ASSERT_STREQ("Invalid constant pool index", e.what());
			throw;
		}
	}, std::invalid_argument);

	ASSERT_THROW({
		try
		{ cp[16]; }
		catch (const std::invalid_argument& e)
		{
			ASSERT_STREQ("Invalid constant pool index", e.what());
			throw;
		}
	}, std::invalid_argument);

	std::shared_ptr<ConstantBase> temp = cp[1];
	ASSERT_EQ(temp->get_tag(), ConstantTag::CONSTANT_Methodref);
	std::shared_ptr<ConstantMethodref> c1 = std::static_pointer_cast<ConstantMethodref>(temp);
	ASSERT_EQ(c1->get_class_index(), 2);
	ASSERT_EQ(c1->get_name_and_type_index(), 3);

	temp = cp[2];
	ASSERT_EQ(temp->get_tag(), ConstantTag::CONSTANT_Class);
	std::shared_ptr<ConstantClass> c2 = std::static_pointer_cast<ConstantClass>(temp);
	ASSERT_EQ(c2->get_name_index(), 4);

	temp = cp[3];
	ASSERT_EQ(temp->get_tag(), ConstantTag::CONSTANT_NameAndType);
	std::shared_ptr<ConstantNameAndType> c3 = std::static_pointer_cast<ConstantNameAndType>(temp);
	ASSERT_EQ(c3->get_name_index(), 5);
	ASSERT_EQ(c3->get_descriptor_index(), 6);

	temp = cp[4];
	ASSERT_EQ(temp->get_tag(), ConstantTag::CONSTANT_Utf8);
	std::shared_ptr<ConstantUtf8> c4 = std::static_pointer_cast<ConstantUtf8>(temp);
	ASSERT_EQ(c4->get_contents(), "java/lang/Object");

	temp = cp[5];
	ASSERT_EQ(temp->get_tag(), ConstantTag::CONSTANT_Utf8);
	std::shared_ptr<ConstantUtf8> c5 = std::static_pointer_cast<ConstantUtf8>(temp);
	ASSERT_EQ(c5->get_contents(), "<init>");

	temp = cp[6];
	ASSERT_EQ(temp->get_tag(), ConstantTag::CONSTANT_Utf8);
	std::shared_ptr<ConstantUtf8> c6 = std::static_pointer_cast<ConstantUtf8>(temp);
	ASSERT_EQ(c6->get_contents(), "()V");

	temp = cp[7];
	ASSERT_EQ(temp->get_tag(), ConstantTag::CONSTANT_Integer);
	std::shared_ptr<ConstantInteger> c7 = std::static_pointer_cast<ConstantInteger>(temp);
	ASSERT_EQ(c7->get_contents(), 1000000);

	temp = cp[8];
	ASSERT_EQ(temp->get_tag(), ConstantTag::CONSTANT_Class);
	std::shared_ptr<ConstantClass> c8 = std::static_pointer_cast<ConstantClass>(temp);
	ASSERT_EQ(c8->get_name_index(), 9);

	temp = cp[9];
	ASSERT_EQ(temp->get_tag(), ConstantTag::CONSTANT_Utf8);
	std::shared_ptr<ConstantUtf8> c9 = std::static_pointer_cast<ConstantUtf8>(temp);
	ASSERT_EQ(c9->get_contents(), "TestConstant");

	temp = cp[10];
	ASSERT_EQ(temp->get_tag(), ConstantTag::CONSTANT_Utf8);
	std::shared_ptr<ConstantUtf8> c10 = std::static_pointer_cast<ConstantUtf8>(temp);
	ASSERT_EQ(c10->get_contents(), "Code");

	temp = cp[11];
	ASSERT_EQ(temp->get_tag(), ConstantTag::CONSTANT_Utf8);
	std::shared_ptr<ConstantUtf8> c11 = std::static_pointer_cast<ConstantUtf8>(temp);
	ASSERT_EQ(c11->get_contents(), "LineNumberTable");

	temp = cp[12];
	ASSERT_EQ(temp->get_tag(), ConstantTag::CONSTANT_Utf8);
	std::shared_ptr<ConstantUtf8> c12 = std::static_pointer_cast<ConstantUtf8>(temp);
	ASSERT_EQ(c12->get_contents(), "add_42");

	temp = cp[13];
	ASSERT_EQ(temp->get_tag(), ConstantTag::CONSTANT_Utf8);
	std::shared_ptr<ConstantUtf8> c13 = std::static_pointer_cast<ConstantUtf8>(temp);
	ASSERT_EQ(c13->get_contents(), "(I)I");

	temp = cp[14];
	ASSERT_EQ(temp->get_tag(), ConstantTag::CONSTANT_Utf8);
	std::shared_ptr<ConstantUtf8> c14 = std::static_pointer_cast<ConstantUtf8>(temp);
	ASSERT_EQ(c14->get_contents(), "SourceFile");

	temp = cp[15];
	ASSERT_EQ(temp->get_tag(), ConstantTag::CONSTANT_Utf8);
	std::shared_ptr<ConstantUtf8> c15 = std::static_pointer_cast<ConstantUtf8>(temp);
	ASSERT_EQ(c15->get_contents(), "TestConstant.java");
}