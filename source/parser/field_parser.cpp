#include "../util/InStream.hpp"
#include "../metainfo/field_info.hpp"
#include "parser.hpp"

FieldInfo parse_field_info(InStream& stream)
{
	uint16_t access_flags = stream.get_u2();
	uint16_t name_index = stream.get_u2();
	uint16_t descriptor_index = stream.get_u2();
	uint16_t attributes_count = stream.get_u2();

	std::vector<std::shared_ptr<AttributeBase>> attributes;
	attributes.reserve(attributes_count);
	for (uint16_t i = 0; i < attributes_count; i++)
		attributes.push_back(parse_attribute(stream));

	return FieldInfo(access_flags, name_index, descriptor_index, std::move(attributes));
}