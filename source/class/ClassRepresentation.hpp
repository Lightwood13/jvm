#ifndef CLASS_REPRESENTATION_HPP
#define CLASS_REPERSENTATION_HPP

#include "../metainfo/constant_pool/ConstantPool.hpp"
#include <cstdint>

class ClassRepresentation
{
public:
	ClassRepresentation(const ClassRepresentation& cr) = delete;
	ClassRepresentation(
		uint16_t minor_version,
		uint16_t major_version,
		ConstantPool&& cp,
		uint16_t access_flags,
		uint16_t this_class,
		uint16_t super_class,
		std::vector<uint16_t>&& interfaces,
		FieldInfo&& field_info
	) noexcept : minor_version(minor_version), major_version(major_version), constant_pool(std::move(cp)),
		access_flags(access_flags), this_class(this_class), super_class(super_class),
		interfaces(std::move(interfaces)), field_info(std::move(field_info)) {};

private:
	uint16_t minor_version;
	uint16_t major_version;
	ConstantPool constant_pool;
	uint16_t access_flags;
	uint16_t this_class;
	uint16_t super_class;
	std::vector<uint16_t> interfaces;
	FieldInfo field_info;
};

#endif // CLASS_REPERSENTATION_HPP