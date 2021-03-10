#include "IOHelpers.h"
#include <string>
#include <cstdio>

/// Copy a shader from a plain text file into a character array.
bool ReadShaderSource(const std::string file_name, std::string& shader_str)
{
	FILE* file = fopen(file_name.c_str(), "r");

	// if unable to open file
	if (file == nullptr)
	{
		return false;
	}

	// seek to end of file
	fseek(file, 0, SEEK_END);

	// get current file position which is end from seek
	size_t size = ftell(file);

	// allocate string space and set length
	shader_str.resize(size);

	// go back to beginning of file for read
	rewind(file);

	// read 1*size bytes from sfile into shader_str
	fread(&shader_str[0], 1, size, file);

	// close the file
	fclose(file);

	return true;
}