#include "Renderer.h"
#include <iostream>
#include <vector>
#include "MathHelpers.h"
#include "AnimatedModelLoader.h"
#include "TimeManager.h"
#include "Debug.h"

Renderer::Renderer() : window(nullptr), textureCursor(nullptr)
{

}
Renderer::~Renderer()
{
	delete virtualWorld;

	delete vaoCursor;
	delete textureCursor;
	delete fw;

	glDeleteProgram(programScreenQuad);
	glDeleteProgram(programScreenQuadAO);
	glDeleteProgram(programCursor);

	delete shaderBlur;
	delete shaderBloomFinal;
}

void Renderer::Initilise()
{
	std::function<void(std::string, FileStatus)> action = [](std::string filename, FileStatus status) -> void
	{
		// Process only regular files, all other file types are ignored
		if (!std::filesystem::is_regular_file(std::filesystem::path(filename)) && status != FileStatus::erased)
		{
			return;
		}

		std::string extension = filename.substr(filename.find_last_of(".") + 1);

		MessageQueue::Current()->Notify_FileChanged(filename, extension, status);
	};

	messageQueue = MessageQueue::Create();

	fw = new FileWatcher("Shaders", std::chrono::milliseconds(5000), action);
	fw->start();

    if(InitGL())
    {
		LoadModels();
		LoadShaders();

		InitCursor();
		InitScreenQuad();

		InitScene();
    }
}

bool Renderer::InitGL()
{
	noErrors = true;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		noErrors = false;
		std::cout << "machine does not support GLEW. GLEW not ok." << std::endl;
	}
	if (!GLEW_VERSION_2_1)  // check that the machine supports the 2.1 API.
	{
		noErrors = false;
		std::cout << "machine does not support GLEW >= 2.1 API" << std::endl;
	}
	return noErrors;
}

void Renderer::OnResize(int width, int height)
{
	// adjust the viewport when the window is resized
	glViewport(0, 0, width, height);

	projectionMatrix = Matrix4::Perspective(fov, (float)width / (float)height, 0.01f, 1000);

	InitFBOs();
}

void Renderer::LoadModels()
{




	virtualWorld = new VirtualWorld();
	virtualWorld->Initilise();

	InitNoiseMap();
}

void Renderer::LoadShaders()
{
	programScreenQuad = loadShader("Shaders/screenQuad_vs.glsl", "Shaders/screenQuad_fs.glsl");
	programScreenQuadAO = loadShader("Shaders/screenQuadAO_vs.glsl", "Shaders/screenQuadAO_fs.glsl");
	programCursor = loadShader("Shaders/screenCursor_vs.glsl", "Shaders/screenCursor_fs.glsl");
	
	shaderBlur = new ShaderProgram("Shaders/blur_vs.glsl", "Shaders/blur_fs.glsl");
	shaderBloomFinal = new ShaderProgram("Shaders/bloom_final_vs.glsl","Shaders/bloom_final_fs.glsl");

	Debug::DrawLines_Init();
}

void Renderer::initQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
}

void Renderer::renderQuad()
{
	glBindVertexArray(quadVAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Renderer::Render(Transform cameraTransform)
{
    if(!noErrors) return;

	TimeManager::Update();
	messageQueue->update();

	// CLEAR THE BUFFERS
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window->getSize().x, window->getSize().y);

	// 1. render scene into floating point framebuffer
	// -----------------------------------------------
	//bloom = false;
	if(bloom)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);





	//Matrix4 mv2 = Matrix4::GetIdentity();
	//mv2.Scale(Vector3::One * 0.1f);
	//mv2 *= model * modelview;
	//mv2.Translate(Vector3(-0.5f, -0.4f, -2));
	

	virtualWorld->worldPosition = this->worldPosition;

	virtualWorld->Render(window, cameraTransform, projectionMatrix, modelview, lightDir, lightModelView);


	//
	//ShowShadowMap(cameraTransform);
	//ShowAOMap();

	if (bloom)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. blur bright fragments with two-pass Gaussian Blur 
		// --------------------------------------------------
		bool horizontal = true, first_iteration = true;
		unsigned int amount = 8;
		shaderBlur->start();

		for (unsigned int i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			shaderBlur->setInt("image", 0);
			shaderBlur->setInt("horizontal", horizontal);

			// bind texture of other framebuffer (or scene if first iteration)
			glActiveTexture(GL_TEXTURE0);
			if (first_iteration)
			{
				glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)	
			}
			renderQuad();
			horizontal = !horizontal;
			if (first_iteration)
			{
				first_iteration = false;
			}
		}
		shaderBlur->stop();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		// --------------------------------------------------------------------------------------------------------------------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderBloomFinal->start();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
		shaderBloomFinal->setInt("scene", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
		shaderBloomFinal->setInt("bloomBlur", 1);

		shaderBloomFinal->setInt("bloom", bloom);
		shaderBloomFinal->setFloat("exposure", exposure);
		renderQuad();
		shaderBloomFinal->stop();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//std::cout << "bloom: " << (bloom ? "on" : "off") << "| exposure: " << exposure << std::endl;
	}

	if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0))
	{
		//terrain->Initilise();

		// Update noise map
		noisePixelData = GetNoisePixelData(freq, amp, pers, octaves, scaleX, scaleY, scaleZ);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, noisePixelData);
	}
	//ShowNoiseMap();

	RenderCursor(cameraTransform);
}

void Renderer::InitFBOs()
{
	// FBO Setup for Blurring and Bloom effect
	// Set up floating point framebuffer to render scene to
	unsigned int SCR_WIDTH = window->getSize().x;
	unsigned int SCR_HEIGHT = window->getSize().y;

	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}
	// create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ping-pong-framebuffer for blurring
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Framebuffer not complete!" << std::endl;
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::InitScene()
{
	// Matricies
	projectionMatrix = Matrix4::Perspective(fov, (float)window->getSize().x / (float)window->getSize().y, 0.1f, 1000);

	lightModelView = Matrix4::GetIdentity();

	modelview = Matrix4::GetIdentity();
	modelview.Translate(Vector3(0, 0, -2.0f));

	// Time Management.
	TimeManager::Initilise();

	// FBO Setup for Blurring and Bloom effect
	InitFBOs();

	initQuad();
}

void Renderer::InitScreenQuad() 
{
	// Load the Screen Quad
	float vertices[] =
	{
		// Left bottom triangle
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		// Right top triangle
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	screenQuadVertexCount = 6;

	GLuint points_vbo;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 18, &vertices[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 18, vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vaoScreenQuad);
	glBindVertexArray(vaoScreenQuad);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void Renderer::ShowShadowMap(Transform cameraTransform)
{
	//TODO: show window in bottom right with shadow map

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	
	Vector2 spriteSize = Vector2(1024, 1024); // size of the sprite in pixels
	Vector2 spritePosition = Vector2(0.0f, 0.0f);
	Vector2 screenSize = Vector2((float)window->getSize().x, (float)window->getSize().y);
	Vector3 position = ScreenSpaceToOrthographic(spritePosition, screenSize, spriteSize);
	//Vector3 position = spritePosition;

	float sizeX = (spriteSize.x / (float)window->getSize().x);
	float sizeY = (spriteSize.y / (float)window->getSize().y);
	//float sizeX = spriteSize.x;
	//float sizeY = spriteSize.y;

	Matrix4 model = Matrix4::GetIdentity();
	model.Scale(Vector3(sizeX, sizeY, 1.0f));
	model.Translate(Vector3(-0.5f * sizeX, -0.5f * sizeY, 0.0f));
	model = model * Matrix4::RotationYawPitchRoll(0.0f, 0.0f, 0.0f);
	model.Translate(Vector3(0.5f * sizeX, 0.5f * sizeY, 0.0f));
	model.Translate(position);
	
	
	Matrix4 quadView;
	//quadView = Matrix4::LookAt(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3::Up);
	quadView = cameraTransform.localMatrix;
	Matrix4 quadProj;
	//quadProj = Matrix4::Orthographic(-1, 1, -1, 1, 0.1f, 1);
	//quadProj = Matrix4::Perspective(90, window->getSize().x / (float)window->getSize().y, 0.1f, 1000);
	//quadProj = Matrix4::Orthographic(0.0f, window->getSize().x, window->getSize().y, 0.0f, -1.0f, 1.0f);
	//quadProj = Matrix4::Orthographic(0, 1, -1, 0, -1.0f, 1.0f);
	quadProj = Matrix4::Orthographic(-1, 1, -1, 1, -1.0f, 1.0f);
	Matrix4 MVP = quadProj * quadView * model;
	MVP = quadProj * model;

	float* arrMVP = MVP.ToArray();

	OpenGlUtils::EnableDepthTesting(false);
	glEnable(GL_CULL_FACE);
	glUseProgram(programScreenQuad);
	glUniformMatrix4fv(glGetUniformLocation(programScreenQuad, "MVP"), 1, GL_FALSE, arrMVP);
	glUniform2f(glGetUniformLocation(programScreenQuad, "framebufferSize"), 1024, 1024);
	glUniform1f(glGetUniformLocation(programScreenQuad, "near_plane"), 0.1f);
	glUniform1f(glGetUniformLocation(programScreenQuad, "far_plane"), 1000);
	glUniform1i(glGetUniformLocation(programScreenQuad, "depthTexture"), 0);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, virtualWorld->character00->depthTexture);
	//glRecti(-1, -1, 1, 1);
	glBindVertexArray(vaoScreenQuad);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, screenQuadVertexCount);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glUseProgram(0);

	delete[] arrMVP;
}

void Renderer::ShowAOMap()
{
	//TODO: show window in bottom right with shadow map

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(programScreenQuadAO);

	Vector2 spriteSize = Vector2(400, 400); // size of the sprite in pixels
	Vector2 spritePosition = Vector2(0.0f, 0.75f);
	Vector2 screenSize = Vector2((float)window->getSize().x, (float)window->getSize().y);
	Vector3 position = ScreenSpaceToOrthographic(spritePosition, screenSize, spriteSize);

	float sizeX = (spriteSize.x / (float)window->getSize().x);
	float sizeY = (spriteSize.y / (float)window->getSize().y);
	Matrix4 quadModel = Matrix4::GetIdentity();
	quadModel.Scale(Vector3(sizeX, sizeY, 1.0f));
	//quadModel.Translate(Vector3(-0.5f * sizeX, -0.5f * sizeY, 0.0f));
	quadModel = quadModel * Matrix4::RotationYawPitchRoll(0.0f, 0.0f, 0.0f);
	//quadModel.Translate(Vector3(0.5f * sizeX, 0.5f * sizeY, 0.0f));
	quadModel.Translate(position);

	Matrix4 quadView;
	quadView = Matrix4::GetIdentity();
	
	Matrix4 quadProj;
	quadProj = Matrix4::Orthographic(-1, 1, -1, 1, -1.0f, 1.0f);

	Matrix4 MVP = quadProj * quadView * quadModel;

	float* arrMVP = MVP.ToArray();

	OpenGlUtils::EnableDepthTesting(false);
	glUniformMatrix4fv(glGetUniformLocation(programScreenQuadAO, "MVP"), 1, GL_FALSE, arrMVP);
	glUniform2f(glGetUniformLocation(programScreenQuadAO, "framebufferSize"), spriteSize.x, spriteSize.y);
	glUniform1f(glGetUniformLocation(programScreenQuadAO, "near_plane"), 0.1f);
	glUniform1f(glGetUniformLocation(programScreenQuadAO, "far_plane"), 1000);
	glUniform1i(glGetUniformLocation(programScreenQuadAO, "depthTexture"), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, virtualWorld->character00->aoTexture);
	glBindVertexArray(vaoScreenQuad);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, screenQuadVertexCount);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glUseProgram(0);

	delete[] arrMVP;
}

void Renderer::InitCursor()
{
	textureCursor = Texture::LoadTexture("light_cursor.tga").Create();
	std::vector<GLfloat> vertices;
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(0);

	vertices.push_back(1);
	vertices.push_back(0);
	vertices.push_back(0);
	vertices.push_back(1);
	vertices.push_back(1);
	vertices.push_back(0);

	vertices.push_back(0);
	vertices.push_back(1);
	vertices.push_back(0);

	std::vector<GLfloat> texCoords;
	texCoords.push_back(0.0f);
	texCoords.push_back(0.0f);
	texCoords.push_back(1.0f);
	texCoords.push_back(0.0f);
	texCoords.push_back(1.0f);
	texCoords.push_back(1.0f);
	texCoords.push_back(0.0f);
	texCoords.push_back(1.0f);


	std::vector<int> indicies;
	indicies.push_back(0);
	indicies.push_back(1);
	indicies.push_back(2);
	indicies.push_back(3);

	vaoCursor = Vao::create();
	vaoCursor->bind();
	vaoCursor->createIndexBuffer(indicies);
	vaoCursor->createAttribute(0, vertices, 3);
	vaoCursor->createAttribute(1, texCoords, 2);
	vaoCursor->unbind();
}

void Renderer::RenderCursor(Transform cameraTransform)
{
	//float screenW = 64, screenH = 64; // size of the cursor in pixels
	//float sizeX = (screenW / (float)window->getSize().x);
	//float sizeY = (screenH / (float)window->getSize().y);
	//float halfX = sizeX * 0.25f;
	//float halfY = sizeY * 0.25f;

	//sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	//Vector2 mouse = Vector2((mousePosition.x / (float)window->getSize().x), (mousePosition.y / (float)window->getSize().y));
	//Vector3 position = Vector3(((mouse.x - halfX) * 2.0f) - 1.0f, ((1.0f - mouse.y - halfY) * 2.0f) - 1.0f, -1.0f);

	Vector2 cursorSize = Vector2(64, 64); // size of the cursor in pixels
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	Vector2i mouse = Vector2i(mousePosition.x, mousePosition.y);
	Vector2 screenSize = Vector2((float)window->getSize().x, (float)window->getSize().y);
	Vector3 position = ScreenSpaceToOrthographic(mouse, screenSize, cursorSize);

	Matrix4 model = Matrix4::GetIdentity();

	float sizeX = (cursorSize.x / (float)window->getSize().x);
	float sizeY = (cursorSize.y / (float)window->getSize().y);
	model.Scale(Vector3(sizeX, sizeY, 1.0f));
	model.Translate(Vector3(-0.5f * sizeX, -0.5f * sizeY, 0.0f));
	model = model * Matrix4::RotationYawPitchRoll(0.0f, 0.0f, 0.0f);
	model.Translate(Vector3(0.5f * sizeX, 0.5f * sizeY, 0.0f));
	model.Translate(position);


	Matrix4 scalem = Matrix4::GetIdentity();
	//scalem.Scale(Vector3(1,1,1) * 0.01f);

	Matrix4 quadModelView = model;
	//quadModelView.Scale(Vector3(1, 1, 1) * 10);
	//quadModelView.Translate(Vector3(10,0,0));
	Matrix4 quadView;
	//quadView = Matrix4::LookAt(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3::Up);
	quadView = cameraTransform.localMatrix;
	Matrix4 quadProj;
	quadProj = Matrix4::Orthographic(-1, 1, -1, 1, -1.0f, 1.0f);
	//quadProj = Matrix4::Perspective(90, window->getSize().x / (float)window->getSize().y, 0.1f, 1000);
	//quadProj = Matrix4::Orthographic(0.0f, window->getSize().x, window->getSize().y, 0.0f, -1.0f, 1.0f);
	//quadProj = Matrix4::Orthographic(0, 1, -1, 0, -1.0f, 1.0f);
	//quadProj = Matrix4::Orthographic(0, window->getSize().x, window->getSize().y, 0, 0.1f, 1.0f);
	//quadProj = Matrix4::GetIdentity();
	Matrix4 MVP = quadProj * quadView * quadModelView;
	MVP = quadProj * scalem * quadModelView;

	float* arrMVP = MVP.ToArray();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	OpenGlUtils::EnableAlphaBlending();
	OpenGlUtils::EnableDepthTesting(false);
	glUseProgram(programCursor);
	glUniformMatrix4fv(glGetUniformLocation(programCursor, "MVP"), 1, GL_FALSE, arrMVP);
	glUniform2f(glGetUniformLocation(programCursor, "textureSize"), 320, 320);
	glUniform1i(glGetUniformLocation(programCursor, "cursorTexture"), 0);

	textureCursor->bindToUnit(0);
	vaoCursor->bind(0, 1);
	glDrawElements(GL_QUADS, 6, GL_UNSIGNED_INT, nullptr);
	vaoCursor->unbind(0, 1);
	glUseProgram(0);

	delete[] arrMVP;
}

void Renderer::InitNoiseMap()
{
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Renderer::ShowNoiseMap()
{
	//TODO: show window in bottom left with noise map

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(programScreenQuadAO);

	Vector2 spriteSize = Vector2(400, 400); // size of the sprite in pixels
	Vector2 spritePosition = Vector2(0.0f, 0.75f);
	Vector2 screenSize = Vector2((float)window->getSize().x, (float)window->getSize().y);
	Vector3 position = ScreenSpaceToOrthographic(spritePosition, screenSize, spriteSize);

	float sizeX = (spriteSize.x / (float)window->getSize().x);
	float sizeY = (spriteSize.y / (float)window->getSize().y);
	Matrix4 quadModel = Matrix4::GetIdentity();
	quadModel.Scale(Vector3(sizeX, sizeY, 1.0f));
	//quadModel.Translate(Vector3(-0.5f * sizeX, -0.5f * sizeY, 0.0f));
	quadModel = quadModel * Matrix4::RotationYawPitchRoll(0.0f, 0.0f, 0.0f);
	//quadModel.Translate(Vector3(0.5f * sizeX, 0.5f * sizeY, 0.0f));
	quadModel.Translate(position);

	Matrix4 quadView;
	quadView = Matrix4::GetIdentity();

	Matrix4 quadProj;
	quadProj = Matrix4::Orthographic(-1, 1, -1, 1, -1.0f, 1.0f);

	Matrix4 MVP = quadProj * quadView * quadModel;

	float* arrMVP = MVP.ToArray();

	OpenGlUtils::EnableDepthTesting(false);
	glUniformMatrix4fv(glGetUniformLocation(programScreenQuadAO, "MVP"), 1, GL_FALSE, arrMVP);
	glUniform2f(glGetUniformLocation(programScreenQuadAO, "framebufferSize"), spriteSize.x, spriteSize.y);
	glUniform1f(glGetUniformLocation(programScreenQuadAO, "near_plane"), 0.1f);
	glUniform1f(glGetUniformLocation(programScreenQuadAO, "far_plane"), 1000);
	glUniform1i(glGetUniformLocation(programScreenQuadAO, "depthTexture"), 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
	{
		pers += 0.1f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
	{
		pers -= 0.1f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
	{
		scaleX += 0.1f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4))
	{
		scaleX -= 0.1f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5))
	{
		scaleY += 0.1f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6))
	{
		scaleY -= 0.1f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7))
	{
		scaleZ += 0.1f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8))
	{
		scaleZ -= 0.1f;
	}
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glBindVertexArray(vaoScreenQuad);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, screenQuadVertexCount);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glUseProgram(0);

	delete[] arrMVP;
}