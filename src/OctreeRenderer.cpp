#include "OctreeRenderer.h"

OctreeRenderer::OctreeRenderer() : octreeInput(nullptr)
{

}
OctreeRenderer::~OctreeRenderer()
{
	if (octreeInput)
	{
		delete octreeInput;
		octreeInput = nullptr;
	}
}

void OctreeRenderer::CompressOBJToOctree(std::string objFileName, Octree* octree)
{
	WavefrontOBJMeshImporter* importer = new WavefrontOBJMeshImporter();



	delete importer;
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
	AABB sceneBounds(Vector3::Zero, 10000);
	octreeInput = new Octree();
	octreeInput->BBox = &sceneBounds;
	CompressOBJToOctree("Assets/diablo3_pose/diablo3_pose.obj", octreeInput); // TODO: set position of model in octree
}

void OctreeRenderer::Render(sf::RenderWindow* window, Transform cameraTransform, Matrix4 projectionMatrix, Matrix4 modelview, Vector3 light_dir, Matrix4 lightModelView)
{
	// Which objects in the frustum need to be rendered.
	Vector3 eyePosition = cameraTransform.GetLocalPosition();
	AABB currentArea = AABB(eyePosition, 10); // TODO: use frustum intersection for current area

	std::vector<IndexedTriangleSet*> nearestList = std::vector<IndexedTriangleSet*>();
	octreeInput->FindNearestObjects(currentArea, nearestList);

	// For each IFS, initilise if not already and then render.

}