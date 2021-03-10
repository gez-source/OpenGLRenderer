#ifndef STRING_HELPERS
#define STRING_HELPERS

#include <vector>
#include <string>
#include "Matrix4.h"

std::string LeftTrim(std::string s);
std::string RightTrim(std::string s);
std::string Trim(std::string s);
std::vector<std::string> StringSplit(std::string data, std::string delimiter);
void StringSplit(std::string data, std::string delimiter, std::vector<std::string>& tokens);
void Split(std::string data, std::string delimiter, std::vector<std::string>& tokens, bool removeWhitespace);
Matrix4 ConvertDataToMatrix(std::string data);
Matrix4 ConvertDataToMatrix(std::vector<std::string> data, int matrixIndex);


#endif