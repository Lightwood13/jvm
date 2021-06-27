#ifndef CONSTANT_BASE_HPP
#define CONSTANT_BASE_HPP

#include <cstdint>

enum class ConstantTag : uint8_t;

class ConstantBase
{
public:
	ConstantTag virtual get_tag() const = 0;
	virtual ~ConstantBase() {};
};

enum class ConstantTag : uint8_t
{
	CONSTANT_Class = 7,
	CONSTANT_Fieldref = 9,
	CONSTANT_Methodref = 10,
	CONSTANT_InterfaceMethodref = 11,
	CONSTANT_String = 8,
	CONSTANT_Integer = 3,
	CONSTANT_Float = 4,
	CONSTANT_Long = 5,
	CONSTANT_Double = 6,
	CONSTANT_NameAndType = 12,
	CONSTANT_Utf8 = 1,
	CONSTANT_MethodHandle = 15,
	CONSTANT_MethodType = 16,
	CONSTANT_Dynamic = 17,
	CONSTANT_InvokeDynamic = 18,
	CONSTANT_Module = 19,
	CONSTANT_Package = 20
};

std::string constant_tag_to_string(ConstantTag tag) {
	switch (tag) {
	case ConstantTag::CONSTANT_Class: return "CONSTANT_Class";
	case ConstantTag::CONSTANT_Fieldref: return "CONSTANT_Fieldref";
	case ConstantTag::CONSTANT_Methodref: return "CONSTANT_Methodref";
	case ConstantTag::CONSTANT_InterfaceMethodref: return "CONSTANT_InterfaceMethodref";
	case ConstantTag::CONSTANT_String: return "CONSTANT_String";
	case ConstantTag::CONSTANT_Integer: return "CONSTANT_Integer";
	case ConstantTag::CONSTANT_Float: return "CONSTANT_Float";
	case ConstantTag::CONSTANT_Long: return "CONSTANT_Long";
	case ConstantTag::CONSTANT_Double: return "CONSTANT_Double";
	case ConstantTag::CONSTANT_NameAndType: return "CONSTANT_NameAndType";
	case ConstantTag::CONSTANT_Utf8: return "CONSTANT_Utf8";
	case ConstantTag::CONSTANT_MethodHandle: return "CONSTANT_MethodHandle";
	case ConstantTag::CONSTANT_MethodType: return "CONSTANT_MethodType";
	case ConstantTag::CONSTANT_InvokeDynamic: return "CONSTANT_InvokeDynamic";
	default: return "Unknown";
	}
}

#endif // CONSTANT_BASE_HPP
