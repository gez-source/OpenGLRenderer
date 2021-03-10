#include "BinaryWriter.h"

BinaryWriter::BinaryWriter(std::string fileName)
{
	this->position = 0;
	this->fileName = fileName;

	ofs = new std::fstream();
	ofs->open(fileName, std::ios::out | std::ios::in | std::ios::binary);
}

BinaryWriter::~BinaryWriter()
{
	ofs->close();
	delete ofs;
}

// Set the position of the parser to the specified position in bytes.
void BinaryWriter::SetPosition(long position)
{
	this->position = position;
}

// Add the specified offset in bytes to the current position of the parser.
void BinaryWriter::Advance(long offset)
{
	this->position += offset;
}

// Saves a double precision floating point value at the current position of the parser.
void BinaryWriter::SetDouble(double value)
{
	ofs->write((char*)&value, sizeof(double));
	Advance(sizeof(double));
}

// Saves a single precision floating point value at the current position of the parser.
void BinaryWriter::SetFloat(float value)
{
	ofs->write((char*)&value, sizeof(float));
	Advance(sizeof(float));
}

// Saves a 32 bit integer value at the current position of the parser.
void BinaryWriter::SetInt32(int value)
{
	ofs->write((char*)&value, sizeof(int));
	Advance(sizeof(int));
}

// Saves a 64 bit integer value at the current position of the parser.
void BinaryWriter::SetInt64(long value)
{
	ofs->write((char*)&value, sizeof(long));
	Advance(sizeof(long));
}

// Saves a string value at the current position of the parser.
void BinaryWriter::SetString(std::string value, long size)
{
	ofs->write(&value.at(0), size);
	Advance(size);
}
