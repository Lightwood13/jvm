#include <memory>
#include <cstdint>

#include "../util/InStream.hpp"
#include "../metainfo/attribute/AtrributeBase.hpp"
#include "../metainfo/attribute/AttributeCode.hpp"


std::unique_ptr<AttributeBase> parse_attribute(InStream& stream)
{
	uint16_t attribute_name_index = stream.get_u2();
	uint32_t attribute_length = stream.get_u4();
	stream.skip(attribute_length);
	return std::make_unique<AttributeUnresolved>();
}