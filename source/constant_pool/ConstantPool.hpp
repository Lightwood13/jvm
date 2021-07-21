#ifndef CONSTANT_POOL_HPP
#define CONSTANT_POOL_HPP

#include "ConstantBase.hpp"
#include <vector>
#include <memory>
#include <stdexcept>

class ConstantPool
{
public:
	ConstantPool(std::vector<std::shared_ptr<ConstantBase>>&& pool) : pool(std::move(pool))
	{ }

	std::shared_ptr<ConstantBase> operator[](uint16_t index) const
	{
		if (index == 0 || index > pool.size() || !pool[index - 1])
			throw std::invalid_argument("Invalid constant pool index");
		return pool[index - 1];
	}
private:
	const std::vector<std::shared_ptr<ConstantBase>> pool;
};

#endif // CONSTANT_POOL_HPP