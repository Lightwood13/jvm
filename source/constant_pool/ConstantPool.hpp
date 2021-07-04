#ifndef CONSTANT_POOL_HPP
#define CONSTANT_POOL_HPP

#include "ConstantBase.hpp"
#include <vector>
#include <memory>
#include <stdexcept>

class ConstantPool
{
public:
	ConstantPool(std::vector<std::shared_ptr<const ConstantBase>>&& pool) : pool(std::move(pool))
	{ }
	std::shared_ptr<const ConstantBase> operator[](uint16_t index) const
	{
		if (index >= pool.size() || !pool[index])
			throw std::invalid_argument("Invalid constant pool index");
		return pool[index];
	}
private:
	const std::vector<std::shared_ptr<const ConstantBase>> pool;
};

#endif // CONSTANT_POOL_HPP