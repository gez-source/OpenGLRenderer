#include "BinaryReader.h"
#include <fstream>

BinaryReader::BinaryReader(std::string fileName)
{
	this->position = 0;
	this->fileName = fileName;

	ifs = new std::ifstream();
	ifs->open(fileName, std::ios::in | std::fstream::binary);
}

BinaryReader::~BinaryReader()
{
	ifs->close();
	delete ifs;
}

// Set the position of the parser to the specified position in bytes.
void BinaryReader::SetPosition(long position)
{
	this->position = position;
}

// Add the specified offset in bytes to the current position of the parser.
void BinaryReader::Advance(long offset)
{
	this->position += offset;
}

// Get the next floating point number from the current position of the parser.
float BinaryReader::GetFloat()
{
	float value;

	ifs->read((char*)&value, sizeof(float));

	Advance(sizeof(float));

	return value;
}

// Get the next 32 bit integer from the current position of the parser.
int BinaryReader::GetInt32()
{
	int value;

	ifs->read((char*)&value, sizeof(int));

	Advance(sizeof(int));

	return value;
}

// Get the next 64 bit integer from the current position of the parser.
long BinaryReader::GetInt64()
{
	long value;

	ifs->read((char*)&value, sizeof(long));

	Advance(sizeof(long));

	return value;
}
// Get the next string given its specified size from the current position of the parser.
std::string BinaryReader::GetString(long size)
{
	char* stringValue = new char[size + 1];

	ifs->read(stringValue, size);

	stringValue[size] = 0;

	return std::string(stringValue);
}
