#include "ShaderHelpers.h"

void printShaderLog(GLuint shader)
{
	GLint log_length = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
	std::string str;
	if(log_length > 0)
	{
		str.reserve(log_length);
		glGetShaderInfoLog(shader, log_length, NULL, &str[0]);

		std::cout << &str[0] << std::endl;
	}
}

void printProgramLog(GLuint program)
{
	GLint log_length = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
	std::string str;
	if (log_length > 0)
	{
		str.reserve(log_length);
		glGetProgramInfoLog(program, log_length, NULL, &str[0]);

		std::cout << &str[0] << std::endl;
	}
}


GLuint loadShader(std::string fileName, GLenum shaderType)
{
	// LOAD shader.
	std::string shaderSource;
	ReadShaderSource(fileName, shaderSource);

	GLuint shaderID = glCreateShader(shaderType);
	const GLchar* p = (const GLchar*)shaderSource.c_str();
	glShaderSource(shaderID, 1, &p, nullptr);
	glCompileShader(shaderID);

	// Check for compiler errors
	int params = -1;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params)
	{
		printShaderLog(shaderID);
		std::cout << "ERROR: GL shader index " << shaderID << " did not compile " << fileName << std::endl;
	}
	return shaderID;
}

GLuint loadShader(std::string fileNameVertexShader, std::string fileNameFragShader)
{
	// LOAD shader.
	std::string vertex_shader;
	std::string fragment_shader;

	ReadShaderSource(fileNameVertexShader, vertex_shader);
	ReadShaderSource(fileNameFragShader, fragment_shader);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *p = (const GLchar *)vertex_shader.c_str();
	glShaderSource(vs, 1, &p, nullptr);
	glCompileShader(vs);

	// Check for compile errors.
	int params = -1;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params)
	{
		std::cout << "ERROR: VS GL shader index " << vs << " did not compile " << fileNameVertexShader << std::endl;
		printShaderLog(vs);
		return 0; // or exit or something
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	p = (const GLchar *)fragment_shader.c_str();
	glShaderSource(fs, 1, &p, NULL);
	glCompileShader(fs);

	// Check for compile errors.
	glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params)
	{
		std::cout << "ERROR: FS GL shader index " << fs << " did not compile " << fileNameFragShader << std::endl;
		printShaderLog(fs);
		return 0; // or exit or something
	}

	// Link the shader program.
	GLuint program = glCreateProgram();
	glAttachShader(program, fs);
	glAttachShader(program, vs);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &params);
	if (GL_TRUE != params)
	{
		fprintf(stderr, "ERROR: could not link shader program GL index %i\n",
			program);
		printProgramLog(program);
		return 0;
	}

	return program;
}

GLuint loadShaderFS(std::string fileNameFragShader)
{
	// LOAD shader.
	std::string fragment_shader;
	ReadShaderSource(fileNameFragShader, fragment_shader);

	// Check for compile errors.
	int params = -1;

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* p = (const GLchar *)fragment_shader.c_str();
	glShaderSource(fs, 1, &p, NULL);
	glCompileShader(fs);

	// Check for compile errors.
	glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params)
	{
		std::cout << "ERROR: FS GL shader index " << fs << " did not compile" << std::endl;
		printShaderLog(fs);
		return 0; // or exit or something
	}

	// Link the shader program.
	GLuint program = glCreateProgram();
	glAttachShader(program, fs);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &params);
	if (GL_TRUE != params)
	{
		fprintf(stderr, "ERROR: could not link shader programme GL index %i\n",
			program);
		printProgramLog(program);
		return 0;
	}

	return program;
}