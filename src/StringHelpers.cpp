#include "StringHelpers.h"

std::string LeftTrim(std::string s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !(std::isspace(ch));
		}));
	return s;
}

std::string RightTrim(std::string s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), s.end());
	return s;
}

std::string Trim(std::string s)
{
	s = LeftTrim(s);
	s = RightTrim(s);
	return s;
}

std::vector<std::string> StringSplit(std::string data, std::string delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	auto start = 0U;
	data = Trim(data);
	auto end = data.find(delimiter);
	while (end != std::string::npos)
	{
		token = data.substr(start, end - start);
		start = end + delimiter.length();
		end = data.find(delimiter, start);

		if (!token.empty())
		{
			tokens.push_back(Trim(token));
		}
	}

	token = data.substr(start, end);
	if (!token.empty())
	{
		tokens.push_back(Trim(token));
	}
	return tokens;
}

void StringSplit(std::string data, std::string delimiter, std::vector<std::string>& tokens)
{
	std::string token;
	auto start = 0U;
	data = Trim(data);
	auto end = data.find(delimiter);
	while (end != std::string::npos)
	{
		token = data.substr(start, end - start);
		start = end + delimiter.length();
		end = data.find(delimiter, start);

		if (!token.empty())
		{
			token = Trim(token);

			tokens.push_back(Trim(token));
		}
	}

	token = data.substr(start, end);
	if (!token.empty())
	{
		token = Trim(token);

		tokens.push_back(token);
	}
}

void Split(std::string data, std::string delimiter, std::vector<std::string>& tokens, bool removeWhitespace)
{
	std::string token;
	auto start = 0U;
	data = Trim(data);
	auto end = data.find(delimiter);
	while (end != std::string::npos)
	{
		token = data.substr(start, end - start);
		start = end + delimiter.length();
		end = data.find(delimiter, start);

		if (!token.empty())
		{
			token = Trim(token);

			if (removeWhitespace) 
			{
				StringSplit(token, "\n", tokens);
			}
		}
	}

	token = data.substr(start, end);
	if (!token.empty())
	{
		token = Trim(token);

		if (removeWhitespace)
		{
			StringSplit(token, "\n", tokens);
		}
	}
}

Matrix4 ConvertDataToMatrix(std::string data)
{
	std::vector<std::string> tokens;
	Split(data, " ", tokens, true);
	return ConvertDataToMatrix(tokens, 0);
}

Matrix4 ConvertDataToMatrix(std::vector<std::string> data, int matrixIndex)
{
	Matrix4 m;

	int stride = 16 * matrixIndex;

	m.m00 = std::stof(data[stride + 0]);
	m.m01 = std::stof(data[stride + 1]);
	m.m02 = std::stof(data[stride + 2]);
	m.m03 = std::stof(data[stride + 3]);

	m.m10 = std::stof(data[stride + 4]);
	m.m11 = std::stof(data[stride + 5]);
	m.m12 = std::stof(data[stride + 6]);
	m.m13 = std::stof(data[stride + 7]);

	m.m20 = std::stof(data[stride + 8]);
	m.m21 = std::stof(data[stride + 9]);
	m.m22 = std::stof(data[stride + 10]);
	m.m23 = std::stof(data[stride + 11]);

	m.m30 = std::stof(data[stride + 12]);
	m.m31 = std::stof(data[stride + 13]);
	m.m32 = std::stof(data[stride + 14]);
	m.m33 = std::stof(data[stride + 15]);

	return m;
}