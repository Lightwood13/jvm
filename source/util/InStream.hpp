#ifndef IN_STREAM_HPP
#define IN_STREAM_HPP

#include <istream>
#include <cstdint>
#include <stdexcept>
#include <string>

class InStream
{
private:
	std::istream& stream;
	std::string name;
public:
	InStream(std::istream& stream, const std::string& name) : stream(stream), name(name) {}

	std::string get_name() { return name; }

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

	void skip(size_t length)
	{
		stream.seekg(length, std::ios_base::cur);
	}
};

#endif // IN_STREAM_HPP