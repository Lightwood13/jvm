#ifndef FIELD_INFO_HPP
#define FIELD_INFO_HPP

#include <cstdint>
#include <vector>
#include <memory>

#include "attribute/AtrributeBase.hpp"

class FieldInfo
{
public:
	FieldInfo(uint16_t access_flags, uint16_t name_index, uint16_t descriptor_index,
		std::vector<std::shared_ptr<AttributeBase>>&& attributes)
		: access_flags(access_flags), name_index(name_index), descriptor_index(descriptor_index),
		attributes(attributes) {};
	uint16_t get_access_flags() const { return access_flags; }
	uint16_t get_name_index() const { return name_index; }
	uint16_t get_descriptor_index() const { return descriptor_index; }
	std::shared_ptr<AttributeBase> operator[](uint16_t index) const
	{
		if (index == 0 || index >= attributes.size())
			throw std::invalid_argument("Invalid field attribute index");
		return attributes[index];
	}
private:
	uint16_t access_flags;
	uint16_t name_index;
	uint16_t descriptor_index;
	std::vector<std::shared_ptr<AttributeBase>> attributes;
	uint32_t offset = 0;
};

#endif // FIELD_INFO_HPP