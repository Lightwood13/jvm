#ifndef ATTRIBUTE_BASE_HPP
#define ATTRIBUTE_BASE_HPP

#include <cstdint>

enum class AttributeName
{
	CODE,
	UNRESOLVED
};

class AttributeBase
{
public:
	virtual AttributeName get_name() const = 0;
	virtual ~AttributeBase() {};
};

class AttributeUnresolved : public AttributeBase
{
public:
	AttributeName get_name() const override { return AttributeName::UNRESOLVED; }
};

#endif // ATTRIBUTE_BASE_HPP