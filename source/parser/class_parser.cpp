#include "../util/InStream.hpp"
#include "../class/ClassRepresentation.hpp"
#include "parser.hpp"

#include <stdexcept>
#include <vector>

ClassRepresentation parse_class(InStream& stream)
{
	if (stream.get_u4() != 0xCAFEBABE)
		throw std::logic_error("Invalid magic number at the start of class file: " + stream.get_name());

	uint16_t minor_version = stream.get_u2();
	uint16_t major_version = stream.get_u2();

	ConstantPool cp = parse_constant_pool(stream);

	uint16_t access_flags = stream.get_u2();
	uint16_t this_class = stream.get_u2();
	uint16_t super_class = stream.get_u2();

	uint16_t interface_count = stream.get_u2();
	std::vector<uint16_t> interfaces(interface_count);
	for (int i = 0; i < interface_count; i++)
		interfaces[i] = stream.get_u2();
	
	FieldInfo field_info = parse_field_info(stream);
	return ClassRepresentation(
		minor_version,
		major_version,
		std::move(cp),
		access_flags,
		this_class,
		super_class,
		std::move(interfaces),
		std::move(field_info)
	);
}