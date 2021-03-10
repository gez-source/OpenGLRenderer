#ifndef BINARY_WRITER_H
#define BINARY_WRITER_H

#include <string>
#include <fstream>

class BinaryWriter
{
private:
	int position = 0;
	std::fstream* ofs;
	std::string fileName;
public:

	BinaryWriter(std::string fileName);

	~BinaryWriter();

	// Set the position of the parser to the specified position in bytes.
	void SetPosition(long position);

	// Add the specified offset in bytes to the current position of the parser.
	void Advance(long offset);

	// Saves a double precision floating point value at the current position of the parser.
	void SetDouble(double value);

	// Saves a single precision floating point value at the current position of the parser.
	void SetFloat(float value);

	// Saves a 32 bit integer value at the current position of the parser.
	void SetInt32(int value);

	// Saves a 64 bit integer value at the current position of the parser.
	void SetInt64(long value);

	// Saves a string value at the current position of the parser.
	void SetString(std::string value, long size);
};

#endif
