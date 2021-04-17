#include "ShaderProgram.h"
#include <filesystem>

//std::map<std::string, ShaderProgram*> ShaderProgram::linkedPrograms;
std::multimap<std::string, ShaderProgram*> ShaderProgram::linkedPrograms;
std::map<std::string, ShaderType> ShaderProgram::mapShaderType;

ShaderProgram::ShaderProgram() : vertexShaderID(0), fragmentShaderID(0), programID(0)
{

}

ShaderProgram::ShaderProgram(std::string vertexFile, std::string fragmentFile)
{
	this->vertexFile = vertexFile;
	this->fragmentFile = fragmentFile;

	vertexShaderID = loadShader(vertexFile, GL_VERTEX_SHADER);
	fragmentShaderID = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	programID = glCreateProgram();

	attach();
	link();

	AssociateShader(vertexFile, ShaderType::ST_VertexShader);
	AssociateShader(fragmentFile, ShaderType::ST_FragmentShader);
}

ShaderProgram::~ShaderProgram()
{
	stop();
	glDeleteProgram(programID);
}

void ShaderProgram::AssociateShader(std::string filename, ShaderType shaderType)
{
	linkedPrograms.insert({ filename , this });

	mapShaderType.insert({ filename, shaderType });
}

void ShaderProgram::RecompileShader(std::string filename)
{
	ShaderProgram* shaderProgram = nullptr;
	ShaderType shaderType = ShaderType::ST_Unknown;

	for(auto it : mapShaderType)
	{
		if (std::filesystem::equivalent(filename, it.first))
		{
			shaderType = it.second;

			break;
		}
	}

	for(auto it : linkedPrograms)
	{
		if(std::filesystem::equivalent(filename, it.first))
		{
			shaderProgram = it.second;

			if (shaderProgram != nullptr && shaderType != ShaderType::ST_Unknown)
			{
				HotSwap(filename, shaderType, shaderProgram);
			}

			//break;
		}
	}
	

}

void ShaderProgram::HotSwap(std::string filename, ShaderType shaderType, ShaderProgram* shaderProgram)
{
	int params = -1;

	// The file is a shader so we need to Hot Swap or recompile the shader and then attach to its program.
	if(shaderType == ST_FragmentShader)
	{
		std::cout << "[Hot Swap] Compiling Fragment Shader '" << filename << "' Associated to Program ID (" << shaderProgram->programID << ")" << std::endl;

		glDetachShader(shaderProgram->programID, shaderProgram->fragmentShaderID);
		glDeleteShader(shaderProgram->fragmentShaderID);

		shaderProgram->fragmentShaderID = loadShader(filename, GL_FRAGMENT_SHADER);

		params = -1;
		glGetShaderiv(shaderProgram->fragmentShaderID, GL_COMPILE_STATUS, &params);

		if(params == GL_TRUE)
		{
			glAttachShader(shaderProgram->programID, shaderProgram->fragmentShaderID);
		}
	}
	if (shaderType == ST_VertexShader)
	{
		std::cout << "[Hot Swap] Compiling Vertex Shader '" << filename << "' Associated to Program ID (" << shaderProgram->programID << ")" << std::endl;

		glDetachShader(shaderProgram->programID, shaderProgram->vertexShaderID);
		glDeleteShader(shaderProgram->vertexShaderID);

		shaderProgram->vertexShaderID = loadShader(filename, GL_VERTEX_SHADER);

		params = -1;
		glGetShaderiv(shaderProgram->vertexShaderID, GL_COMPILE_STATUS, &params);

		if (params == GL_TRUE)
		{
			glAttachShader(shaderProgram->programID, shaderProgram->vertexShaderID);		
		}
	}

	if (params == GL_TRUE)
	{
		glLinkProgram(shaderProgram->programID);

		glGetProgramiv(shaderProgram->programID, GL_LINK_STATUS, &params);
		if (params != GL_TRUE)
		{
			std::cout << "[Hot Swap] ERROR: could not re-link shader program GL index " << shaderProgram->programID << " '" << filename << "'" << std::endl;
			printProgramLog(shaderProgram->programID);
		}
		else
		{
			std::cout << "[Hot Swap] Recompiled Vertex Shader ID (" << shaderProgram->vertexShaderID << ") Associated To Program ID (" << shaderProgram->programID << ")" << std::endl;
		}
	}
}

void ShaderProgram::attach()
{
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
}

void ShaderProgram::link()
{
	int params = -1;

	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &params);

	if (GL_TRUE != params)
	{
		std::cout << "ERROR: could not link shader program GL index " << programID << " '" << this->vertexFile << "' '" << this->fragmentFile << "'" << std::endl;
		printProgramLog(programID);
	}
}

void ShaderProgram::detach()
{
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
}

void ShaderProgram::deleteShaders()
{
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void ShaderProgram::storeAllUniformLocations(std::vector<Uniform*> uniforms)
{
	for (Uniform* uniform : uniforms)
	{
		uniform->storeUniformLocation(programID);
	}
	glValidateProgram(programID);
}

void ShaderProgram::start()
{
	glUseProgram(programID);
}

void ShaderProgram::stop()
{
	glUseProgram(0);
}

void ShaderProgram::setBool(std::string name, bool value)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void ShaderProgram::setInt(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void ShaderProgram::setFloat(std::string name, float value)
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void ShaderProgram::setVec2(std::string name, Vector2& value)
{
	//glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
	glUniform2f(glGetUniformLocation(programID, name.c_str()), value.x, value.y);
}
void ShaderProgram::setVec2(std::string name, float x, float y)
{
	glUniform2f(glGetUniformLocation(programID, name.c_str()), x, y);
}

void ShaderProgram::setVec3(std::string name, Vector3& value)
{
	//glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
	glUniform3f(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z);
}
void ShaderProgram::setVec3(std::string name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(programID, name.c_str()), x, y, z);
}

void ShaderProgram::setVec4(std::string name, Vector4& value)
{
	//glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
	glUniform4f(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z, value.w);
}
void ShaderProgram::setVec4(std::string name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(programID, name.c_str()), x, y, z, w);
}

void ShaderProgram::setMat3(std::string name, Matrix3& mat)
{
	float* matArr = mat.ToArray();
	glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, matArr);
	delete[] matArr;
}

void ShaderProgram::setMat4(std::string name, Matrix4& mat)
{
	float* matArr = mat.ToArray();
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, matArr);
	delete[] matArr;
}