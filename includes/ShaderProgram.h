#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <GL/glew.h>
#include <vector>
#include "ShaderHelpers.h"
#include <Uniform.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"

enum ShaderType
{
	ST_Unknown = 0,
	ST_VertexShader,
	ST_FragmentShader
};

class ShaderProgram 
{
private:
	//static std::map<std::string, ShaderProgram*> linkedPrograms;
	static std::multimap<std::string, ShaderProgram*> linkedPrograms;
	static std::map<std::string, ShaderType> mapShaderType;

	std::string vertexFile;
	std::string fragmentFile;
protected:
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
public:
	GLuint programID;
	ShaderProgram();
	ShaderProgram(std::string vertexFile, std::string fragmentFile);
	~ShaderProgram();

	void AssociateShader(std::string filename, ShaderType shaderType);

	static void RecompileShader(std::string filename);
	static void HotSwap(std::string filename, ShaderType shaderType, ShaderProgram* shaderProgram);
protected:
	void attach();
	void link();
	void detach();
	void deleteShaders();

public:
	void storeAllUniformLocations(std::vector<Uniform*> uniforms);
	void start();
	void stop();
	void setBool(std::string name, bool value);
	void setInt(std::string name, int value);
	void setFloat(std::string name, float value);
	void setVec2(std::string name, Vector2& value);
	void setVec2(std::string name, float x, float y);
	void setVec3(std::string name, Vector3& value);
	void setVec3(std::string name, float x, float y, float z);
	void setVec4(std::string name, Vector4& value);
	void setVec4(std::string name, float x, float y, float z, float w);
	void setMat3(std::string name, Matrix3& mat);
	void setMat4(std::string name, Matrix4& mat);
};

#endif