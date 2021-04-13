#include "OctreeRenderer.h"
#include "OpenGlUtils.h"
#include <stack>

OctreeRenderer::OctreeRenderer() : octreeInput(nullptr)
{
	program = new ShaderProgram("Shaders/character_vs.glsl", "Shaders/character_fs.glsl");
}
OctreeRenderer::~OctreeRenderer()
{
	if (octreeInput)
	{
		delete octreeInput;
		octreeInput = nullptr;
	}
}

void OctreeRenderer::AddOBJToOctree(MeshBufferVAO* meshBufferVAO)
{
	octreeInput->Add(meshBufferVAO);
}

bool OctreeRenderer::SaveToBinaryFile(std::string fileName, Octree* octree)
{
	return true;
}

bool OctreeRenderer::LoadFromBinaryFile(std::string fileName, Octree* octree)
{
	return true;
}

void OctreeRenderer::Initilise()
{
	AABB* sceneBounds = new AABB(Vector3::Zero, Vector3::One * 10);
	octreeInput = new Octree();
	octreeInput->BBox = sceneBounds;
}

void OctreeRenderer::Render(sf::RenderWindow* window, Transform cameraTransform, Matrix4 projectionMatrix, Matrix4 modelview, Vector3 light_dir, Matrix4 lightModelView)
{
	bool outlinePass = false;

	// Which objects in the frustum need to be rendered?
	Vector3 eyePosition = cameraTransform.GetLocalPosition();
	AABB currentArea = AABB(eyePosition, Vector3::One * 10); // TODO: use frustum intersection for current area

	std::vector<MeshBufferVAO*> nearestList = std::vector<MeshBufferVAO*>();
	octreeInput->FindNearestObjects(currentArea, nearestList);


	// Render dispatch.
	OpenGlUtils::Antialias(true);
	OpenGlUtils::DisableBlending();
	OpenGlUtils::EnableDepthTesting(true);

	glEnable(GL_CULL_FACE);  // cull face
	if (outlinePass)
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

	if (lutTexture00 != nullptr) lutTexture00->bindToUnit(5);
	if (lutTexture01 != nullptr) lutTexture01->bindToUnit(6);
	if (lutTexture02 != nullptr) lutTexture02->bindToUnit(7);
	if (lutTexture03 != nullptr) lutTexture03->bindToUnit(8);
	if (lutTexture04 != nullptr) lutTexture04->bindToUnit(9);

	// For each mesh, initilise if not already and then render.
	for (auto mesh : nearestList)
	{
		mesh->BindTextures();
		mesh->Render();
		mesh->Bounds->Debug();
	}

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

void OctreeRenderer::RenderAABB()
{
	std::stack<Octree*> stack;
	Octree* node;

	stack.push(octreeInput);

	while (!stack.empty())
	{
		node = stack.top();
		stack.pop();

		node->BBox->Debug();
		for (auto obj : (*node->sceneObjects))
		{
			obj->Bounds->Debug();
		}

		if (!node->IsEmpty())
		{
			// Continue the search frontier.
			if (node->TopBackLeft != nullptr)
			{
				stack.push(node->TopBackLeft);
			}
			if (node->TopBackRight != nullptr)
			{
				stack.push(node->TopBackRight);
			}
			if (node->TopFrontLeft != nullptr)
			{
				stack.push(node->TopFrontLeft);
			}
			if (node->TopFrontRight != nullptr)
			{
				stack.push(node->TopFrontRight);
			}

			if (node->BottomBackLeft != nullptr)
			{
				stack.push(node->BottomBackLeft);
			}
			if (node->BottomBackRight != nullptr)
			{
				stack.push(node->BottomBackRight);
			}
			if (node->BottomFrontLeft != nullptr)
			{
				stack.push(node->BottomFrontLeft);
			}
			if (node->BottomFrontRight != nullptr)
			{
				stack.push(node->BottomFrontRight);
			}
		}
	}
}