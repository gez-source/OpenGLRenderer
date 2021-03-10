#ifndef ANIMATED_MODEL_SHADER_H
#define ANIMATED_MODEL_SHADER_H

#include <string>
#include "ShaderProgram.h"
#include "UniformMatrix.h"
#include "UniformSampler.h"
#include "UniformVec3.h"
#include "UniformMat4Array.h"
#include "UniformInt.h"

class AnimatedModelShader : public ShaderProgram 
{
private:
	int MAX_JOINTS = 100;// max number of joints in a skeleton
	int DIFFUSE_TEX_UNIT = 0;

	inline static std::string VERTEX_SHADER = "Shaders/animatedEntityVertex.glsl";
	inline static std::string VERTEX_SHADER_HAS_ANIM = "Shaders/animatedEntityVertex_Anim.glsl";
	inline static std::string FRAGMENT_SHADER = "Shaders/animatedEntityFragment.glsl";

	GLuint vertexShaderID_HasAnim;
	GLuint programID_HasAnim;
	GLuint programID_Geo;
public:
	UniformMatrix* projectionMatrix = new UniformMatrix("projectionMatrix");
	UniformMatrix* modelviewMatrix = new UniformMatrix("modelviewMatrix");
	UniformMatrix* viewMatrix = new UniformMatrix("viewMatrix");
	UniformVec3* lightDirection = new UniformVec3("lightDirection");
	UniformVec3* eye = new UniformVec3("eye");
	UniformMat4Array* jointTransforms = new UniformMat4Array("jointTransforms", MAX_JOINTS);
	UniformSampler* diffuseTexture = new UniformSampler("diffuseTexture");
	UniformSampler* normalTexture = new UniformSampler("normalTexture");
	UniformSampler* specularTexture = new UniformSampler("specularTexture");
	UniformSampler* glowTexture = new UniformSampler("glowTexture");
public:
	/**
	 * Creates the shader program for the {@link AnimatedModelRenderer} by
	 * loading up the vertex and fragment shader code files. It also gets the
	 * location of all the specified uniform variables, and also indicates that
	 * the diffuse texture will be sampled from texture unit 0.
	 */
	AnimatedModelShader() : ShaderProgram() //: ShaderProgram(VERTEX_SHADER, FRAGMENT_SHADER)
	{
		vertexShaderID = loadShader(VERTEX_SHADER, GL_VERTEX_SHADER);
		vertexShaderID_HasAnim = loadShader(VERTEX_SHADER_HAS_ANIM, GL_VERTEX_SHADER);
		fragmentShaderID = loadShader(FRAGMENT_SHADER, GL_FRAGMENT_SHADER);

		AssociateShader(VERTEX_SHADER, ShaderType::ST_VertexShader);
		AssociateShader(VERTEX_SHADER_HAS_ANIM, ShaderType::ST_VertexShader);
		AssociateShader(FRAGMENT_SHADER, ShaderType::ST_FragmentShader);

		// Create primary program for when dealing with just geometry.
		programID = glCreateProgram();
		programID_Geo = programID;
		attach();
		link();

		// Create second program for when animations are included.
		programID_HasAnim = glCreateProgram();
		programID = programID_HasAnim;
		{
			GLuint temp = vertexShaderID;
			vertexShaderID = vertexShaderID_HasAnim;
			attach();
			link();
			vertexShaderID = temp;
		}
		programID = programID_Geo;

		std::vector<Uniform*> inUniforms;
		inUniforms.push_back((Uniform*)projectionMatrix);
		inUniforms.push_back((Uniform*)modelviewMatrix);
		inUniforms.push_back((Uniform*)viewMatrix);
		inUniforms.push_back((Uniform*)diffuseTexture);
		inUniforms.push_back((Uniform*)normalTexture);
		inUniforms.push_back((Uniform*)specularTexture);
		inUniforms.push_back((Uniform*)glowTexture);
		inUniforms.push_back((Uniform*)lightDirection);
		inUniforms.push_back((Uniform*)eye);
		inUniforms.push_back((Uniform*)jointTransforms);

		storeAllUniformLocations(inUniforms);
		connectTextureUnits();

		programID = programID_HasAnim;
		{
			storeAllUniformLocations(inUniforms);
			connectTextureUnits();
		}
		programID = programID_Geo;
	}

	~AnimatedModelShader() 
	{
		delete projectionMatrix;
		delete modelviewMatrix;
		delete viewMatrix;
		delete lightDirection;
		delete jointTransforms;
		delete diffuseTexture;
		delete normalTexture;
		delete specularTexture;
		delete glowTexture;
		delete eye;

		ShaderProgram::~ShaderProgram();

		glDeleteProgram(programID_HasAnim);
	}

	void start()
	{
		programID = programID_Geo;
		ShaderProgram::start();
	}

	void startEnableAnimations()
	{
		programID = programID_HasAnim;
		ShaderProgram::start();
	}

private:
	/**
	 * Indicates which texture units the textures should be sampled from.
	 */
	void connectTextureUnits() 
	{
		start();
		// Indicates which texture unit the diffuse texture should be sampled from
		diffuseTexture->loadTexUnit(programID, DIFFUSE_TEX_UNIT);
		normalTexture->loadTexUnit(programID, 1);
		specularTexture->loadTexUnit(programID, 2);
		glowTexture->loadTexUnit(programID, 3);
		stop();
	}

};

#endif