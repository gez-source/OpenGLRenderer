#ifndef FILEHELPERS_H
#define FILEHELPERS_H

#include <string>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <vector>
#include <iostream>

inline bool nullorempty(std::string input)
{
	std::string* ptr = &input;
	if(ptr == nullptr) return true;
	return input.empty();
}

inline std::string clone(std::string input)
{
	std::string tmp(input);
	return tmp;
}

inline char* clone(char* cstr)
{
	std::string a_input = std::string(cstr);
	std::string tmp(a_input);
	char* result = const_cast<char*>(tmp.c_str());
    return result;
}

inline void split(std::string input, std::string delimiter, std::vector<std::string>** outTokens, bool ignoreEmpty = false)
{
	std::vector<std::string>* tokens = new std::vector<std::string>();
	std::string s = std::string(input);
	size_t pos = 0;
	std::string token;

	while ((pos = s.find(delimiter)) != std::string::npos) 
	{
		token = s.substr(0, pos);
		if(ignoreEmpty)
		{
			if(!nullorempty(token))
			{
				tokens->push_back(token);
			}
		}
		else
		{
			tokens->push_back(token);
		}

		s.erase(0, pos + delimiter.length());
	}
    tokens->push_back(token);

	*outTokens = tokens;
}

inline std::string remove(std::string input, int startIndex, int endIndex)
{
	input.erase(input.begin() + startIndex, input.begin() + endIndex);
    return input;
}

inline std::string ltrim(std::string s) 
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
    return s;
}

inline std::string rtrim(std::string s) 
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
    return s;
}

inline std::string trim(std::string s) 
{
	s = ltrim(s);
	s = rtrim(s);
    return s;
}

inline std::string ltrim_copy(std::string input) 
{
	std::string s = input;
	ltrim(s);
	return s;
}

inline std::string rtrim_copy(std::string input) 
{
	std::string s = input;
	rtrim(s);
	return s;
}

inline std::string trim_copy(std::string input) 
{
	std::string s = input;
	trim(s);
	return s;
}

inline bool startswith(std::string input, std::string prefix)
{
	return (!input.compare(0, prefix.size(), prefix));
}

inline bool startswith(std::string input, char* cstrPrefix)
{
	std::string a_prefix = std::string(cstrPrefix);
	return startswith(input, a_prefix);
}

inline bool startswith(char* input, char* cstrPrefix)
{
	std::string a_input = std::string(input);
	std::string a_prefix = std::string(cstrPrefix);
	return startswith(a_input, a_prefix);
}

inline bool int_try_parse(std::string input, int* output)
{
	try
	{
		*output = std::stoi(input);
		return true;
	}
	catch (const std::invalid_argument& ia)
	{
		return false;
	}
}

inline bool float_try_parse(std::string input, float& output)
{
	try
	{
		float f = std::stof(input);
        output = f;
		return true;
	}
	catch (const std::invalid_argument& ia)
	{
		return false;
	}
}

#endif
