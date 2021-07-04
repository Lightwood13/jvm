#ifndef UTIL_HPP
#define UTIL_HPP

#include <array>
#include <cstdint>
#include <cstring>
#include <atomic>
#include <algorithm>

enum class Endianness
{
	UNINITIALIZED,
	LITTLE_ENDIAN,
	BIG_ENDIAN
};

Endianness get_endianness() {
	static std::atomic<Endianness> endianness = Endianness::UNINITIALIZED;

	if (endianness == Endianness::UNINITIALIZED)
	{
		const uint32_t i = 0x01020304;
		std::array<uint8_t, 4> i_bytes;
		std::memcpy(i_bytes.data(), &i, 4);
		endianness = (i_bytes[0] == 0x01)
			? Endianness::BIG_ENDIAN
			: Endianness::LITTLE_ENDIAN;
	}
	return endianness;
}

float bytes_to_float(std::array<uint8_t, 4>& data)
{
	if (get_endianness() == Endianness::LITTLE_ENDIAN)
	{
		std::swap(data[0], data[3]);
		std::swap(data[1], data[2]);
	}
	float res;
	std::memcpy(&res, data.data(), data.size());
	return res;
}

float bytes_to_double(std::array<uint8_t, 8>& data)
{
	if (get_endianness() == Endianness::LITTLE_ENDIAN)
	{
		std::swap(data[0], data[7]);
		std::swap(data[1], data[6]);
		std::swap(data[2], data[5]);
		std::swap(data[3], data[4]);
	}
	double res;
	std::memcpy(&res, data.data(), data.size());
	return res;
}

#endif // UTIL_HPP