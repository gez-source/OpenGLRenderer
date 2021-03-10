#ifndef BINARY_READER_H
#define BINARY_READER_H

#include <string>

class BinaryReader
{
private:
	int position = 0;
	std::ifstream* ifs;
	std::string fileName;
public:

	BinaryReader(std::string fileName);

	~BinaryReader();

	// Set the position of the parser to the specified position in bytes.
	void SetPosition(long position);

	// Add the specified offset in bytes to the current position of the parser.
	void Advance(long offset);

	// Get the next floating point number from the current position of the parser.
	float GetFloat();

	// Get the next 32 bit integer from the current position of the parser.
	int GetInt32();

	// Get the next 64 bit integer from the current position of the parser.
	long GetInt64();

	// Get the next string given its specified size from the current position of the parser.
	std::string GetString(long size);
};

#endif
