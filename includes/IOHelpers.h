#ifndef IOHELPERS_H
#define IOHELPERS_H

#include <string>

/// Copy a shader from a plain text file into a character array.
bool ReadShaderSource(const std::string file_name, std::string& shader_str);

#endif
