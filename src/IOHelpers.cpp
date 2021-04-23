#include "IOHelpers.h"
#include <string>
#include <cstdio>
#include <filesystem>

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

int GetDirectoryFiles(std::string path, std::vector<std::string>& files)
{
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		files.push_back(std::string(file.path().u8string()));
	}

	return files.size();
}

std::string GetFileExtension(std::string filename)
{
	return std::string(std::filesystem::path(filename).extension().u8string());
}

std::string ToLowerCaseCopy(const std::string input)
{
	std::string copy = std::string(input);
	std::for_each(copy.begin(), copy.end(), [](char& c) {
		c = ::tolower(c);
	});
	return copy;
}