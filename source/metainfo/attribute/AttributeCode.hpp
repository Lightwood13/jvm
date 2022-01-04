#ifndef ATTRIBUTE_CODE_HPP
#define ATTRIBUTE_CODE_HPP

#include "AtrributeBase.hpp"

class AttributeCode : public AttributeBase
{
public:
	AttributeName get_name() const override { return AttributeName::CODE; }
};

#endif // ATTRIBUTE_CODE_HPP