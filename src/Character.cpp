#include "Character.h"
#include "OpenGlUtils.h"

void Character::LoadModelBuffers(Model* model,
	std::vector<GLfloat>& verticies,
	std::vector<GLfloat>& texCoords,
	std::vector<GLfloat>& normals,
	std::vector<GLfloat>& tangents,
	std::vector<GLfloat>& bitangents,
	int& verticiesCount)
{
	int i;
	int j;
	Vector3 tangent;
	Vector3 bitangent;

	for (i = 0; i < model->nfaces(); i++)
	{
		// Vertex Coordinate
		Vector3 v0 = model->vert(i, 0);
		Vector3 v1 = model->vert(i, 1);
		Vector3 v2 = model->vert(i, 2);

		verticies.push_back(v0.x);
		verticies.push_back(v0.y);
		verticies.push_back(v0.z);

		verticies.push_back(v1.x);
		verticies.push_back(v1.y);
		verticies.push_back(v1.z);

		verticies.push_back(v2.x);
		verticies.push_back(v2.y);
		verticies.push_back(v2.z);

		// Texture Coordinates
		Vector2 t0 = model->uv(i, 0);
		Vector2 t1 = model->uv(i, 1);
		Vector2 t2 = model->uv(i, 2);

		texCoords.push_back(t0.x);
		texCoords.push_back(t0.y);

		texCoords.push_back(t1.x);
		texCoords.push_back(t1.y);

		texCoords.push_back(t2.x);
		texCoords.push_back(t2.y);

		// Normal Coordinates
		Vector3 n0 = model->normal(i, 0);
		Vector3 n1 = model->normal(i, 1);
		Vector3 n2 = model->normal(i, 2);

		// Tangents and Bitangents
		TriangleTangentSpace(v0, v1, v2, t0, t1, t2, tangent, bitangent);

		//Vector3 normal;
		//ComputeTangentBasis(v0, v1, v2, t0, t1, t2, normal, tangent, bitangent);
		//n0 = normal;
		//n1 = normal;
		//n2 = normal;

		normals.push_back(n0.x);
		normals.push_back(n0.y);
		normals.push_back(n0.z);

		normals.push_back(n1.x);
		normals.push_back(n1.y);
		normals.push_back(n1.z);

		normals.push_back(n2.x);
		normals.push_back(n2.y);
		normals.push_back(n2.z);

		for (j = 0; j <= 2; j++)
		{
			// Add the same data for each vertex 

			tangents.push_back(tangent.x);
			tangents.push_back(tangent.y);
			tangents.push_back(tangent.z);

			bitangents.push_back(bitangent.x);
			bitangents.push_back(bitangent.y);
			bitangents.push_back(bitangent.z);
		}
	}

	verticiesCount += model->nfaces() * 3;
}

void Character::LoadModel(std::string fileNameModel)
{
	std::vector<GLfloat> verticies;
	std::vector<GLfloat> texCoords;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> tangents;
	std::vector<GLfloat> bitangents;

	model = new Model(fileNameModel);

	LoadModelBuffers(model, verticies, texCoords, normals, tangents, bitangents, verticiesCount);

	GLuint points_vbo;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticies.size(), &verticies[0], GL_STATIC_DRAW);

	GLuint texcoords_vbo;
	glGenBuffers(1, &texcoords_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texcoords_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);

	GLuint normals_vbo;
	glGenBuffers(1, &normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(), &normals[0], GL_STATIC_DRAW);

	GLuint tangents_vbo;
	glGenBuffers(1, &tangents_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, tangents_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * tangents.size(), &tangents[0], GL_STATIC_DRAW);

	GLuint bitangents_vbo;
	glGenBuffers(1, &bitangents_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, bitangents_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * bitangents.size(), &bitangents[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, texcoords_vbo);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, tangents_vbo);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, bitangents_vbo);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
	//glEnableVertexAttribArray(3);
	//glEnableVertexAttribArray(4);
}

void Character::LoadShaders()
{
	program = new ShaderProgram("Shaders/character_vs.glsl", "Shaders/character_fs.glsl");
	programShadowmap = new ShaderProgram("Shaders/shadowmap_vs.glsl", "Shaders/shadowmap_fs.glsl");
}

void Character::LoadTextures()
{
	if (model->diffusemap_->textureData->getBuffer() != nullptr)
	{
		model->diffusemap_->bindToUnit(0);
		texture = model->diffusemap_->textureId;
	}

	if (model->normalmap_->textureData->getBuffer() != nullptr)
	{
		model->normalmap_->bindToUnit(1);
		textureNormalMap = model->normalmap_->textureId;
	}

	if (model->specularmap_->textureData->getBuffer() != nullptr)
	{
		model->specularmap_->bindToUnit(1);
		textureSpecularMap = model->specularmap_->textureId;
	}

	if (model->glowmap_->textureData->getBuffer() != nullptr)
	{
		model->glowmap_->bindToUnit(1);
		textureGlowMap = model->glowmap_->textureId;
	}
}

void Character::Initilise()
{
	LoadShaders();

	InitShadowMap();
	InitAOMap();
}

void Character::Render(sf::RenderWindow* window, Transform cameraTransform, Matrix4 projectionMatrix, Matrix4 modelview, Vector3 light_dir, Matrix4 lightModelView)
{
	bool outlinePass = false;

	// Step 1. Render depth map.
	//RenderShadowMap(window, cameraTransform, modelview, light_dir, lightModelView);
	//RenderAOMap(window, cameraTransform, modelview, light_dir, lightModelView);

	// Step 2. Render scene as normal without shadow mapping using the computed depth map.
	RenderModel(window, cameraTransform, projectionMatrix, modelview, light_dir, false);
	if(outlinePass)
	{
		RenderModel(window, cameraTransform, projectionMatrix, modelview, light_dir, true);
	}
}

void Character::RenderModel(sf::RenderWindow* window, Transform cameraTransform, Matrix4 projectionMatrix, Matrix4 modelview, Vector3 light_dir, bool outlinePass)
{
	OpenGlUtils::Antialias(true);
	OpenGlUtils::DisableBlending();
	OpenGlUtils::EnableDepthTesting(true);

	glEnable(GL_CULL_FACE);  // cull face
	if(outlinePass)
	{
		glCullFace(GL_FRONT);		// cull front face
	}
	else
	{
		glCullFace(GL_BACK);		// cull back face
	}
	glFrontFace(GL_CCW);			// GL_CCW for counter clock-wise

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	// Draw object with current shader.
	glUseProgram(program->programID);
	glUniform1i(glGetUniformLocation(program->programID, "depthTexture"), 4);
	glUniform1f(glGetUniformLocation(program->programID, "near_plane"), 0.1f);
	glUniform1f(glGetUniformLocation(program->programID, "far_plane"), 1000);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	float* arrProjectionMat = projectionMatrix.ToArray();
	float* arrModelviewMat = modelview.ToArray();
	float* arrViewMat = cameraTransform.localMatrix.ToArray();

	float* ldir = light_dir.ToArray();
	float* eyePos = cameraTransform.localMatrix.GetTranslation().ToArray();

	int projectionMatrixLocation = glGetUniformLocation(program->programID, "projectionMatrix");
	int modelviewMatrixLocation = glGetUniformLocation(program->programID, "modelviewMatrix");
	int viewMatrixLocation = glGetUniformLocation(program->programID, "viewMatrix");
	int lightdirLocation = glGetUniformLocation(program->programID, "lightDir");
	int eyeLocation = glGetUniformLocation(program->programID, "eye");

	int diffuseTextureLocation = glGetUniformLocation(program->programID, "diffuseTexture");
	int normalTextureLocation = glGetUniformLocation(program->programID, "normalTexture");
	int specularTextureLocation = glGetUniformLocation(program->programID, "specularTexture");
	int glowTextureLocation = glGetUniformLocation(program->programID, "glowTexture");
	int outlinePassLocation = glGetUniformLocation(program->programID, "outlinePass");

	float* arrDepthMVP = depthMVP.ToArray();
	glUniformMatrix4fv(glGetUniformLocation(program->programID, "depthMVP"), 1, GL_FALSE, arrDepthMVP);

	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, arrProjectionMat);
	glUniformMatrix4fv(modelviewMatrixLocation, 1, GL_FALSE, arrModelviewMat);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, arrViewMat);
	glUniform3fv(lightdirLocation, 1, ldir);
	glUniform3fv(eyeLocation, 1, eyePos);

	glUniform1i(diffuseTextureLocation, 0);
	glUniform1i(normalTextureLocation, 1);
	glUniform1i(specularTextureLocation, 2);
	glUniform1i(glowTextureLocation, 3);

	glUniform1i(outlinePassLocation, outlinePass ? 1 : 0);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	model->diffusemap_->bindToUnit(0);
	model->normalmap_->bindToUnit(1);
	model->specularmap_->bindToUnit(2);
	model->glowmap_->bindToUnit(3);

	if (lutTexture00 != nullptr) lutTexture00->bindToUnit(5);
	if (lutTexture01 != nullptr) lutTexture01->bindToUnit(6);
	if (lutTexture02 != nullptr) lutTexture02->bindToUnit(7);
	if (lutTexture03 != nullptr) lutTexture03->bindToUnit(8);
	if (lutTexture04 != nullptr) lutTexture04->bindToUnit(9);

	glDrawArrays(GL_TRIANGLES, 0, verticiesCount);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glBindVertexArray(0);

	glUseProgram(0);

	if (outlinePass)
	{
		glCullFace(GL_BACK);		// cull back face
	}

	delete[] arrModelviewMat;
	delete[] arrProjectionMat;
	delete[] arrViewMat;
	delete[] ldir;
	delete[] eyePos;
	delete[] arrDepthMVP;
}

void Character::InitShadowMap()
{
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Character::RenderShadowMap(sf::RenderWindow* window, Transform cameraTransform, Matrix4 modelview, Vector3 light_dir, Matrix4 lightModelView)
{
	// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-16-shadow-mapping/#point-lights

	// Compute the MVP matrix from the light's point of view
	Matrix4 depthProjectionMatrix;
	//depthProjectionMatrix = Matrix4::Orthographic(-1, 1, -1, 1, -0.1f, 1000);
	depthProjectionMatrix = Matrix4::Perspective(90, (float)window->getSize().x / (float)window->getSize().y, 0.1f, 1000);
	Matrix4 depthViewMatrix = Matrix4::LookAt(light_dir, Vector3(0, 0, 0), Vector3::Up);
	depthMVP = depthProjectionMatrix * depthViewMatrix * lightModelView;
	float* arrDepthMVP = depthMVP.ToArray();

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		// Framebuffer is not ok.
		std::cout << "framebuffer is not ok" << std::endl;
	}


	// CLEAR THE BUFFERS
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window->getSize().x, window->getSize().y);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS);		// depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE);  // cull face
	glCullFace(GL_BACK);		// cull back face
	glFrontFace(GL_CCW);			// GL_CCW for counter clock-wise
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Render without the colour buffer.
	glUseProgram(programShadowmap->programID);
	glUniformMatrix4fv(glGetUniformLocation(programShadowmap->programID, "depthMVP"), 1, GL_FALSE, arrDepthMVP);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, verticiesCount);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	delete[] arrDepthMVP;
}

void Character::InitAOMap()
{
	glGenTextures(1, &aoTexture);
	glBindTexture(GL_TEXTURE_2D, aoTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Character::RenderAOMap(sf::RenderWindow* window, Transform cameraTransform, Matrix4 modelview, Vector3 light_dir, Matrix4 lightModelView)
{
	//TODO: actually need to have deffered shading and a gbuffer to use SSAO like this https://www.gamedev.net/articles/programming/graphics/a-simple-and-practical-approach-to-ssao-r2753/
	// See https://github.com/ssloy/tinyrenderer/wiki/Lesson-8:-Ambient-occlusion

	// Compute the MVP matrix from the light's point of view
	Matrix4 aoProjectionMatrix;
	//aoProjectionMatrix = Matrix4::Orthographic(-2, 2, -2, 2, -1000, 1000);
	aoProjectionMatrix = Matrix4::Perspective(90, (float)window->getSize().x / (float)window->getSize().y, 0.01f, 1000);
	Matrix4 aoViewMatrix = Matrix4::LookAt(Vector3(0, 1, 0) + light_dir, Vector3(0, 0, 0), Vector3::Up);
	//Matrix4 aoViewMatrix = Matrix4::LookAt(Vector3::Up * 2, Vector3(0, 0, 0), Vector3::Up);
	//Matrix4 aoViewMatrix = Matrix4::CreateSQTMatrix(Vector3::One, Quaternion::Euler(0, 0, 0), Vector3(0, 1, 0) + light_dir);
	Matrix4 aoMVP = aoProjectionMatrix * aoViewMatrix * lightModelView;
	float* arrAOMVP = aoMVP.ToArray();

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// AO texture. Can sample it later in your shader
	glBindTexture(GL_TEXTURE_2D, aoTexture);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, aoTexture, 0);
	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		// Framebuffer is not ok.
		std::cout << "framebuffer is not ok" << std::endl;
	}


	// CLEAR THE BUFFERS
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window->getSize().x, window->getSize().y);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS);		// depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE);  // cull face
	glCullFace(GL_BACK);		// cull back face
	glFrontFace(GL_CCW);			// GL_CCW for counter clock-wise
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Render without the colour buffer.
	glUseProgram(programShadowmap->programID);
	glUniformMatrix4fv(glGetUniformLocation(programShadowmap->programID, "depthMVP"), 1, GL_FALSE, arrAOMVP);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, verticiesCount);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	delete[] arrAOMVP;
}