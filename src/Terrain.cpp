#include "Terrain.h"
#include "TimeManager.h"

Terrain::Terrain()
{
	Init_Perlin_Noisy();

	// Load textures into cubemap.
	cubemapDiffuse = Cubemap::LoadTexture("Assets/crate.tga", "Assets/crate.tga", "Assets/crate.tga", "Assets/crate.tga", "Assets/crate.tga", "Assets/crate.tga")
		.Anisotropic()->ClampEdges()->Create();

	cubemapSpecular = Cubemap::LoadTexture("Assets/crate_specular.tga", "Assets/crate_specular.tga", "Assets/crate_specular.tga", "Assets/crate_specular.tga", "Assets/crate_specular.tga", "Assets/crate_specular.tga")
		.Anisotropic()->ClampEdges()->Create();

	// Load shader program
	shader = new ShaderProgram("Shaders/cube_vs.glsl", "Shaders/cube_fs.glsl");

	Initilise();

	model = Matrix4::GetIdentity();
	//model.Scale(0.1f);
}

Terrain::~Terrain()
{
	delete cubemapDiffuse;
	delete cubemapSpecular;
	delete shader;
	delete vao;
	delete projectionMatrix;
	delete viewMatrix;
	delete modelMatrix;
	delete cubeMap;
}

void Terrain::Initilise()
{
	// Construct the VAO/VBO for cube geometry.
	std::vector<GLfloat> vertices;                    
	vertices.push_back(1);
	vertices.push_back(1);
	vertices.push_back(-1);
	vertices.push_back(1);
	vertices.push_back(-1);
	vertices.push_back(-1);
	vertices.push_back(-1);
	vertices.push_back(-0.9999998);
	vertices.push_back(-1);
	vertices.push_back(-0.9999997);
	vertices.push_back(1);
	vertices.push_back(-1);

	vertices.push_back(1);
	vertices.push_back(0.9999995);
	vertices.push_back(1);
	vertices.push_back(0.9999994);
	vertices.push_back(-1.000001);
	vertices.push_back(1);
	vertices.push_back(-1);
	vertices.push_back(-0.9999997);
	vertices.push_back(1);
	vertices.push_back(-1);
	vertices.push_back(1);
	vertices.push_back(1);

	std::vector<GLfloat> texCoords;
	
	texCoords.push_back(0);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(0);

	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(0);

	texCoords.push_back(0);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(0);

	texCoords.push_back(0);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(1);

	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(1);

	texCoords.push_back(0);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(1);
	texCoords.push_back(1);
	texCoords.push_back(0);
	texCoords.push_back(1);

	std::vector<GLfloat> normals;
	normals.push_back(0);
	normals.push_back(0);
	normals.push_back(-1);
	normals.push_back(0);
	normals.push_back(0);
	normals.push_back(1);
	normals.push_back(1);
	normals.push_back(0);
	normals.push_back(-2.38419e-7);
	normals.push_back(0);
	normals.push_back(-1);
	normals.push_back(-4.76837e-7);
	normals.push_back(-1);
	normals.push_back(2.38419e-7);
	normals.push_back(-1.49012e-7);
	normals.push_back(2.68221e-7);
	normals.push_back(1);
	normals.push_back(2.38419e-7);
	normals.push_back(0);
	normals.push_back(0);
	normals.push_back(-1);
	normals.push_back(0);
	normals.push_back(0);
	normals.push_back(1);
	normals.push_back(1);
	normals.push_back(-5.96046e-7);
	normals.push_back(3.27825e-7);
	normals.push_back(-4.76837e-7);
	normals.push_back(-1);
	normals.push_back(0);
	normals.push_back(-1);
	normals.push_back(2.38419e-7);
	normals.push_back(-1.19209e-7);
	normals.push_back(2.08616e-7);
	normals.push_back(1);
	normals.push_back(0);

	std::vector<int> indexBuffer;

	// Vertex, Normal, Texcoord
	int indices[108] =
	{
		0, 0, 0, 
		2, 0, 1, 
		3, 0, 2, 
		7, 1, 3,
		5, 1, 4,
		4, 1, 5, 
		4, 2, 6, 
		1, 2, 7, 
		0, 2, 8, 
		5, 3, 9, 
		2, 3, 10, 
		1, 3, 11, 
		2, 4, 12, 
		7, 4, 13, 
		3, 4, 14, 
		0, 5, 15, 
		7, 5, 16, 
		4, 5, 17, 
		0, 6, 18, 
		1, 6, 19, 
		2, 6, 20,
		7, 7, 21, 
		6, 7, 22, 
		5, 7, 23, 
		4, 8, 24, 
		5, 8, 25, 
		1, 8, 26, 
		5, 9, 27, 
		6, 9, 28, 
		2, 9, 29, 
		2, 10, 30,
		6, 10, 31, 
		7, 10, 32, 
		0, 11, 33, 
		3, 11, 34, 
		7, 11, 35
	};

	std::vector<GLfloat> _vertices;
	std::vector<GLfloat> _normals;
	std::vector<GLfloat> _texcords;
	std::vector<int> _indicies;

	int id = 0;
	for(int i=0;i<108;i+=3)
	{
		int vert = indices[i];
		int norm = indices[i + 1];
		int texc = indices[i + 2];

		_vertices.push_back(vertices[vert * 3]);
		_vertices.push_back(vertices[vert * 3 + 1]);
		_vertices.push_back(vertices[vert * 3 + 2]);

		_normals.push_back(normals[norm * 3]);
		_normals.push_back(normals[norm * 3 + 1]);
		_normals.push_back(normals[norm * 3 + 2]);

		_texcords.push_back(texCoords[texc * 2]);
		_texcords.push_back(texCoords[texc * 2 + 1]);

		_indicies.push_back(id);
		id++;
	}

	std::vector<GLfloat> vertexTangents = std::vector<GLfloat>(_vertices.size());
	std::vector<GLfloat> vertexBiTangents = std::vector<GLfloat>(_vertices.size());
	Vector3 normal;
	Vector3 tangent;
	Vector3 bitangent;

	for (int i = 0; i < 36; i++)
	{
		int index = _indicies[i];
		
		int faceIndex = i % 3;

		indexBuffer.push_back(index);

		if(faceIndex == 0)
		{
			int ind0 = _indicies[i];
			int ind1 = _indicies[i + 1];
			int ind2 = _indicies[i + 2];

			Vector3 v0 = Vector3(_vertices[ind0 * 3],
				_vertices[ind0 * 3 + 1],
				_vertices[ind0 * 3 + 2]);

			Vector3 v1 = Vector3(_vertices[ind1 * 3],
				_vertices[ind1 * 3 + 1],
				_vertices[ind1 * 3 + 2]);

			Vector3 v2 = Vector3(_vertices[ind2 * 3],
				_vertices[ind2 * 3 + 1],
				_vertices[ind2 * 3 + 2]);

			Vector2 t0 = Vector2(_texcords[ind0 * 2], _texcords[ind0 * 2 + 1]);
			Vector2 t1 = Vector2(_texcords[ind1 * 2], _texcords[ind1 * 2 + 1]);;
			Vector2 t2 = Vector2(_texcords[ind2 * 2], _texcords[ind2 * 2 + 1]);;

			//TriangleTangentSpace(v0, v1, v2, t0, t1, t2, tangent, bitangent);
			ComputeTangentBasis(v0, v1, v2, t0, t1, t2, normal, tangent, bitangent);

			//_normals[ind0 * 3] = normal.x;
			//_normals[ind0 * 3 + 1] = normal.y;
			//_normals[ind0 * 3 + 2] = normal.z;

			//_normals[ind1 * 3] = normal.x;
			//_normals[ind1 * 3 + 1] = normal.y;
			//_normals[ind1 * 3 + 2] = normal.z;

			//_normals[ind2 * 3] = normal.x;
			//_normals[ind2 * 3 + 1] = normal.y;
			//_normals[ind2 * 3 + 2] = normal.z;


			vertexTangents[ind0 * 3] = tangent.x;
			vertexTangents[ind0 * 3 + 1] = tangent.y;
			vertexTangents[ind0 * 3 + 2] = tangent.z;

			vertexTangents[ind1 * 3] = tangent.x;
			vertexTangents[ind1 * 3 + 1] = tangent.y;
			vertexTangents[ind1 * 3 + 2] = tangent.z;

			vertexTangents[ind2 * 3] = tangent.x;
			vertexTangents[ind2 * 3 + 1] = tangent.y;
			vertexTangents[ind2 * 3 + 2] = tangent.z;


			vertexBiTangents[ind0 * 3] = bitangent.x;
			vertexBiTangents[ind0 * 3 + 1] = bitangent.y;
			vertexBiTangents[ind0 * 3 + 2] = bitangent.z;

			vertexBiTangents[ind1 * 3] = bitangent.x;
			vertexBiTangents[ind1 * 3 + 1] = bitangent.y;
			vertexBiTangents[ind1 * 3 + 2] = bitangent.z;

			vertexBiTangents[ind2 * 3] = bitangent.x;
			vertexBiTangents[ind2 * 3 + 1] = bitangent.y;
			vertexBiTangents[ind2 * 3 + 2] = bitangent.z;
		}
	}



	//////////////////////////
	float placementScale = 2.0f;
	transforms = std::vector<Matrix4>(MAX_CHUNKS);

	int size = cube_root(MAX_CHUNKS);

	float noiseScale = 0.1f;
	float scaleX = noiseScale;
	float scaleY = noiseScale;
	float scaleZ = noiseScale;
	int octaves = 10;
	float pers = 0.1f;
	float seed = 1;

	Vector3 offset(Random01() * seed, Random01() * seed, Random01() * seed);

	int i = 0;
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			for (int z = 0; z < size; z++)
			{
				float noiseValue = OctavePerlin((x * scaleX) + offset.x, (y * scaleY) + offset.y, (z * scaleZ) + offset.z, octaves, pers);
				//std::cout << z << std::endl;

				if (noiseValue < 0.5f)
				{
					Matrix4 m = Matrix4::GetIdentity();

					//float depth = (1 - noiseValue) * 10;
					float depth = y;
					Vector3 transl = (Vector3(x, depth, z) * placementScale);
					m.Translate(transl);

					transforms[i] = m;
					i++;
				}
			}
		}
	}

	if(vao != nullptr)
	{
		delete vao;
	}
	vao = Vao::create();
	vao->bind();
	vao->createIndexBuffer(indexBuffer);
	vao->createAttribute(0, _vertices, 3);
	vao->createAttribute(1, _texcords, 2);
	vao->createAttribute(2, _normals, 3);
	vao->createAttribute(3, vertexTangents, 3);
	vao->createAttribute(4, vertexBiTangents, 3);
	vao->createAttribute(5, transforms);
	vao->unbind();

	InitiliseLights();
}

void Terrain::InitiliseLights()
{
	lights.clear();
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		Light l;

		l.position = Vector3(Random01(), Random01(), Random01()) * 200;
		l.colour = ColourRGBA(1.0f, Random01(), Random01(), Random01());

		lights.push_back(l);
	}
}

void Terrain::Render(Transform cameraTransform, Matrix4 projection, Vector3 lightDir)
{
	renderCubes_instanced(cameraTransform, projection, model, lightDir);
}

void Terrain::renderCube(Transform cameraTransform, Matrix4 projection, Matrix4 model)
{
	std::vector<Uniform*> inUniforms;
	inUniforms.push_back((Uniform*)projectionMatrix);
	inUniforms.push_back((Uniform*)viewMatrix);
	inUniforms.push_back((Uniform*)modelMatrix);
	inUniforms.push_back((Uniform*)cubeMap);

	vao->bind(0, 2);
	shader->start();
	projectionMatrix->loadMatrix(shader->programID, projection);
	modelMatrix->loadMatrix(shader->programID, this->model * model);
	viewMatrix->loadMatrix(shader->programID, cameraTransform.localMatrix);
	cubeMap->loadTexUnit(shader->programID, 0); // Indicates which texture unit the cubemap texture should be sampled from
	shader->storeAllUniformLocations(inUniforms);
	cubemapDiffuse->bindToUnit(0);
	cubemapSpecular->bindToUnit(1);

	OpenGlUtils::Antialias(true);
	OpenGlUtils::DisableBlending();
	OpenGlUtils::EnableDepthTesting(true);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glDrawElements(GL_TRIANGLES, vao->getIndexCount(), GL_UNSIGNED_INT, nullptr);
	shader->stop();
	vao->unbind(0, 2);
}

void Terrain::renderCubes_slow(Transform cameraTransform, Matrix4 projection, Matrix4 model)
{
	float xScale = 2.0f;
	for (int x = 0; x < 50; x++)
	{
		for (int y = 0; y < 50; y++)
		{
			Matrix4 m = Matrix4::GetIdentity();

			float noiseValue;

			//noiseValue = PerlinNoise3D(x * 0.5f, y * 0.5f, 0, 0.07f, SAMPLE_AMP, SAMPLE_PERS, SAMPLE_OCTAVES);

			noiseValue = Random01() * 10.0f;

			Vector3 transl = Vector3(x, noiseValue, y) * xScale;

			//std::cout << noiseValue << std::endl;

			m.Translate(transl);

			renderCube(cameraTransform, projection, m);
		}
	}
}

// https://learnopengl.com/Advanced-OpenGL/Instancing
void Terrain::renderCubes_instanced(Transform cameraTransform, Matrix4 projection, Matrix4 model, Vector3 lightDir)
{
	std::vector<Uniform*> inUniforms;
	inUniforms.push_back((Uniform*)projectionMatrix);
	inUniforms.push_back((Uniform*)viewMatrix);
	inUniforms.push_back((Uniform*)modelMatrix);
	//inUniforms.push_back((Uniform*)chunkTransforms);
	inUniforms.push_back((Uniform*)cubeMap);
	inUniforms.push_back((Uniform*)lightDirection);

	vao->bind(0, 2);
	shader->start();
	projectionMatrix->loadMatrix(shader->programID, projection);
	modelMatrix->loadMatrix(shader->programID, this->model * model);
	viewMatrix->loadMatrix(shader->programID, cameraTransform.localMatrix);
	lightDirection->loadVec3(shader->programID, lightDir);
	//chunkTransforms->loadMatrixArray(shader->programID, transforms);
	//chunkTransforms->loadVectorArray(shader->programID, transforms);

	cubeMap->loadTexUnit(shader->programID, 0); // Indicates which texture unit the cubemap texture should be sampled from

	//t = fmod( t + TimeManager::GetFrameTime(), 1);
	t += TimeManager::GetFrameTime();

	for (GLuint i = 0; i < MAX_LIGHTS; i++)
	{
		Light light = lights[i];
		std::string number = std::to_string(i);
		Vector3 lightPosition = light.position;
		ColourRGBA lightColour = light.colour;

		glUniform3f(glGetUniformLocation(shader->programID, ("lights[" + number + "].position").c_str()), lightPosition.x, lightPosition.y, lightPosition.z);
		glUniform3f(glGetUniformLocation(shader->programID, ("lights[" + number + "].colour").c_str()), lightColour.r, lightColour.g, lightColour.b);

		glUniform1f(glGetUniformLocation(shader->programID, ("lights[" + number + "].constant").c_str()), 0.01f);
		glUniform1f(glGetUniformLocation(shader->programID, ("lights[" + number + "].linear").c_str()), 0.001f);
		glUniform1f(glGetUniformLocation(shader->programID, ("lights[" + number + "].quadratic").c_str()), 0.00064f);
	}

	Vector3 eye = cameraTransform.localMatrix.GetTranslation();
	float* eyePos = eye.ToArray();
	int eyeLocation = glGetUniformLocation(shader->programID, "eye");
	glUniform3fv(eyeLocation, 1, eyePos);

	shader->storeAllUniformLocations(inUniforms);
	cubemapDiffuse->bindToUnit(0);
	cubemapSpecular->bindToUnit(1);

	if (lutTexture00 != nullptr) lutTexture00->bindToUnit(2);
	if (lutTexture01 != nullptr) lutTexture01->bindToUnit(3);
	if (lutTexture02 != nullptr) lutTexture02->bindToUnit(4);
	if (lutTexture03 != nullptr) lutTexture03->bindToUnit(5);
	if (lutTexture04 != nullptr) lutTexture04->bindToUnit(6);

	OpenGlUtils::Antialias(true);
	OpenGlUtils::DisableBlending();
	OpenGlUtils::EnableDepthTesting(true);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


	//OpenGlUtils::goWireframe(true);
	glDrawElementsInstanced(GL_TRIANGLES, vao->getIndexCount(), GL_UNSIGNED_INT, 0, MAX_CHUNKS);
	//glDrawElementsInstancedARB(GL_TRIANGLES, vao->getIndexCount(), GL_UNSIGNED_INT, 0, MAX_CHUNKS);
	shader->stop();
	vao->unbind(0, 2);

	delete[] eyePos;
}