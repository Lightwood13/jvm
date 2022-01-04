#ifndef PARSER_HPP
#define PARSER_HPP

#include "../util/InStream.hpp"
#include "../metainfo/constant_pool/ConstantPool.hpp"
#include "../metainfo/attribute/AtrributeBase.hpp"

ConstantPool parse_constant_pool(InStream& stream);
std::unique_ptr<AttributeBase> parse_attribute(InStream& stream);

#endif // PARSER_HPP