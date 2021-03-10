#include "Skybox.h"

Skybox::Skybox(std::string texFileRightPosX, std::string texFileLeftNegX,
	std::string texFileTopPosY, std::string texFileBottomNegY,
	std::string texFileBackPosZ, std::string texFileFrontNegZ)
{
	// Load textures into cubemap.
	cubemap = Cubemap::LoadTexture(texFileRightPosX, texFileLeftNegX, texFileTopPosY, texFileBottomNegY, texFileBackPosZ, texFileFrontNegZ)
		.Anisotropic()->ClampEdges()->Create();

	// Load shader program
	shader = new ShaderProgram("Shaders/skybox_vs.glsl", "Shaders/skybox_fs.glsl");

	// Construct the VAO/VBO for cube geometry.
	std::vector<GLfloat> vertices;
	vertices.push_back(-1.0f);
	vertices.push_back(-1.0f);
	vertices.push_back(1.0f);
	vertices.push_back(1.0f);
	vertices.push_back(-1.0f);
	vertices.push_back(1.0f);
	vertices.push_back(1.0f);
	vertices.push_back(1.0f);
	vertices.push_back(1.0f);
	vertices.push_back(-1.0f);
	vertices.push_back(1.0f);
	vertices.push_back(1.0f);

	vertices.push_back(-1.0f);
	vertices.push_back(-1.0f);
	vertices.push_back(-1.0f);
	vertices.push_back(1.0f);
	vertices.push_back(-1.0f);
	vertices.push_back(-1.0f);
	vertices.push_back(1.0f);
	vertices.push_back(1.0f);
	vertices.push_back(-1.0f);
	vertices.push_back(-1.0f);
	vertices.push_back(1.0f);
	vertices.push_back(-1.0f);

	std::vector<GLfloat> texCoords;
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(0);
	texCoords.push_back(0);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(0);
	texCoords.push_back(0);

	std::vector<GLfloat> normals;
	normals.push_back(-1.0f);
	normals.push_back(-1.0f);
	normals.push_back(1.0f);
	normals.push_back(1.0f);
	normals.push_back(-1.0f);
	normals.push_back(1.0f);
	normals.push_back(1.0f);
	normals.push_back(1.0f);
	normals.push_back(1.0f);
	normals.push_back(-1.0f);
	normals.push_back(1.0f);
	normals.push_back(1.0f);

	normals.push_back(-1.0f);
	normals.push_back(-1.0f);
	normals.push_back(-1.0f);
	normals.push_back(1.0f);
	normals.push_back(-1.0f);
	normals.push_back(-1.0f);
	normals.push_back(1.0f);
	normals.push_back(1.0f);
	normals.push_back(-1.0f);
	normals.push_back(-1.0f);
	normals.push_back(1.0f);
	normals.push_back(-1.0f);

	std::vector<int> indexBuffer;

	int indices[48] =
	{
		// front face
		0, 1, 2, 
		2, 3, 0, 
		// top face
		3, 2, 6, 
		6, 7, 3, 
		// back face
		7, 6, 5, 
		5, 4, 7, 
		// left face
		4, 0, 3, 
		3, 7, 4, 
		// bottom face
		0, 1, 5, 
		5, 4, 0, 
		// right face
		1, 5, 6, 
		6, 2, 1, 
	};

	for (int i = 0; i < 48; i++) indexBuffer.push_back(indices[i]);

	vao = Vao::create();
	vao->bind();
	vao->createIndexBuffer(indexBuffer);
	vao->createAttribute(0, vertices, 3);
	vao->createAttribute(1, texCoords, 2);
	vao->createAttribute(2, normals, 3);
	vao->unbind();
}

Skybox::~Skybox()
{
	delete cubemap;
	delete shader;
	delete vao;
}

void Skybox::Render(Transform cameraTransform, Matrix4 projection)
{
	std::vector<Uniform*> inUniforms;
	inUniforms.push_back((Uniform*)projectionMatrix);
	inUniforms.push_back((Uniform*)viewMatrix);
	inUniforms.push_back((Uniform*)cubeMap);
	
	vao->bind(0, 2);
	shader->start();
	projectionMatrix->loadMatrix(shader->programID, projection);

	// Remove translation from view matrix.
	Matrix4 view = Matrix4(cameraTransform.localMatrix.GetRow0(), cameraTransform.localMatrix.GetRow1(), cameraTransform.localMatrix.GetRow2(), cameraTransform.localMatrix.GetRow3());
	view.SetTranslation(Vector3::Zero);
	
	viewMatrix->loadMatrix(shader->programID, view);
	cubeMap->loadTexUnit(shader->programID, 0); // Indicates which texture unit the cubemap texture should be sampled from
	shader->storeAllUniformLocations(inUniforms);
	cubemap->bindToUnit(0);
	
	OpenGlUtils::Antialias(true);
	OpenGlUtils::DisableBlending();
	OpenGlUtils::EnableDepthTesting(true);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	//OpenGlUtils::goWireframe(true);
	glDrawElements(GL_TRIANGLES, vao->getIndexCount(), GL_UNSIGNED_INT, nullptr);
	shader->stop();
	vao->unbind(0, 2);

	glDepthFunc(GL_LESS);
}