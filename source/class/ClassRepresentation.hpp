#ifndef CLASS_REPRESENTATION_HPP
#define CLASS_REPERSENTATION_HPP

#include "../metainfo/constant_pool/ConstantPool.hpp"

class ClassRepresentation
{
public:
	ClassRepresentation(const ConstantPool& cp) = delete;
	ClassRepresentation(ConstantPool&& cp) noexcept : constant_pool(std::move(cp))
	{

	}
private:
	ConstantPool constant_pool;
};

#endif // CLASS_REPERSENTATION_HPP