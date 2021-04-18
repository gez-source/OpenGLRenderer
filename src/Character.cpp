#include "Character.h"
#include "OpenGlUtils.h"
#include "Debug.h"

void Character::LoadModel(std::string fileNameModel)
{
	meshBuffer->LoadModel(fileNameModel);
}

void Character::LoadShaders()
{
	program = new ShaderProgram("Shaders/character_vs.glsl", "Shaders/character_fs.glsl");
	programShadowmap = new ShaderProgram("Shaders/shadowmap_vs.glsl", "Shaders/shadowmap_fs.glsl");
}

void Character::LoadTextures()
{
	meshBuffer->LoadTextures();
}

void Character::Initilise()
{
	LoadShaders();

	InitShadowMap();
	InitAOMap();

	// Initilise test dynamic lighting:
	for (int i = 0; i < numPointLights; i++)
	{
		PointLight* light = new PointLight();
		light->position = Vector3(PointLight::RandomFloat(), PointLight::RandomFloat(), PointLight::RandomFloat()) * 10;
		light->aabb = AABB(light->position, Vector3::One * 0.1f);

		light->constant = 0.01f;
		light->linear = 0.001f;
		light->quadratic = 0.001f;
		light->ambient = Vector3(0, 0, 0);
		light->diffuse = Vector3(PointLight::RandomFloat01(), PointLight::RandomFloat01(), PointLight::RandomFloat01());
		light->specular = Vector3(PointLight::RandomFloat01(), PointLight::RandomFloat01(), PointLight::RandomFloat01());
		light->intensity = PointLight::RandomFloat01();
		light->transform = Matrix4::GetIdentity();
		light->isActive = 1;

		pointLights.push_back(light);
	}

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

	meshBuffer->Bounds->Debug();
	Debug::DrawLines_RenderDispatch(window, cameraTransform, projectionMatrix, modelview);
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

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_FILL GL_LINE


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

	//float* ldir = cameraTransform.localMatrix.TransformVector(light_dir).ToArray();
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

	int numPointLightsLocation = glGetUniformLocation(program->programID, "numPointLights");
	glUniform1i(numPointLightsLocation, numPointLights);

	for (int i = 0; i < pointLights.size(); i++)
	{
		std::string number = std::to_string(i);
		std::string elem = "pointLights[" + number + "]";

		PointLight* light = pointLights[i];

		//light->aabb.Center = light->position;
		
		float smoothing = 1.1;
		light->transform.Translate(PointLight::RandomVector() * smoothing);
		Vector3 pos = light->transform.TransformPosition(light->position);
		pos = modelview.TransformPosition(pos);

		float* position = pos.ToArray();
		glUniform3fv(glGetUniformLocation(program->programID, (elem + ".position").c_str()), 1, position);
		glUniform1f(glGetUniformLocation(program->programID, (elem + ".constant").c_str()), light->constant);
		glUniform1f(glGetUniformLocation(program->programID, (elem + ".linear").c_str()), light->linear);
		glUniform1f(glGetUniformLocation(program->programID, (elem + ".quadratic").c_str()), light->quadratic);

		float* ambient = light->ambient.ToArray();
		float* diffuse = light->diffuse.ToArray();
		float* specular = light->specular.ToArray();

		glUniform3fv(glGetUniformLocation(program->programID, (elem + ".ambient").c_str()), 1, ambient);
		glUniform3fv(glGetUniformLocation(program->programID, (elem + ".diffuse").c_str()), 1, diffuse);
		glUniform3fv(glGetUniformLocation(program->programID, (elem + ".specular").c_str()), 1, specular);

		glUniform1f(glGetUniformLocation(program->programID, (elem + ".intensity").c_str()), light->intensity);

		glUniform1i(glGetUniformLocation(program->programID, (elem + ".isActive").c_str()), light->isActive);

		delete specular;
		delete diffuse;
		delete ambient;
		delete position;
	}
	

	meshBuffer->BindTextures();

	if (lutTexture00 != nullptr) lutTexture00->bindToUnit(5);
	if (lutTexture01 != nullptr) lutTexture01->bindToUnit(6);
	if (lutTexture02 != nullptr) lutTexture02->bindToUnit(7);
	if (lutTexture03 != nullptr) lutTexture03->bindToUnit(8);
	if (lutTexture04 != nullptr) lutTexture04->bindToUnit(9);

	meshBuffer->Render();

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

	for (int i = 0; i < pointLights.size(); i++)
	{
		pointLights[i]->aabb.Debug(ColourRGBA(Vector4(1.0f, pointLights[i]->diffuse.x, pointLights[i]->diffuse.y, pointLights[i]->diffuse.z)));
		Debug::DrawLines_RenderDispatch(window, cameraTransform, projectionMatrix, modelview * pointLights[i]->transform);
	}
	
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
	meshBuffer->Render();
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
	meshBuffer->Render();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	delete[] arrAOMVP;
}