#ifndef IN_STREAM_HPP
#define IN_STREAM_HPP

#include <istream>
#include <cstdint>
#include <stdexcept>

class InStream
{
private:
	std::istream& stream;
public:
	InStream(std::istream& stream) : stream(stream) {}
	void read(char* dest, std::streamsize count)
	{
		stream.read(dest, count);
		if (stream.eof())
			throw std::runtime_error("Unexpected end of file");
	}

	uint8_t get_u1()
	{
		int res = stream.get();
		if (stream.eof())
			throw std::runtime_error("Unexpected end of file");
		return static_cast<uint8_t>(res);
	}

	uint16_t get_u2()
	{
		return (get_u1() << 8) | get_u1();
	}

	uint32_t get_u4()
	{
		return (get_u2() << 16) | get_u2();
	}
};

#endif // IN_STREAM_HPP