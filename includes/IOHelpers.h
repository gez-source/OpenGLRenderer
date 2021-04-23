#ifndef IOHELPERS_H
#define IOHELPERS_H

#include <string>
#include <vector>

/// Copy a shader from a plain text file into a character array.
bool ReadShaderSource(const std::string file_name, std::string& shader_str);

int GetDirectoryFiles(std::string dir, std::vector<std::string>& files);

std::string GetFileExtension(std::string filename);

std::string ToLowerCaseCopy(const std::string input);

#endif
