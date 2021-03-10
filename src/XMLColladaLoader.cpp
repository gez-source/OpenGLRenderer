#include "XMLColladaLoader.h"
#include "XMLSkinLoader.h"
#include "pugixml.hpp"
#include <iostream>
#include "XMLSkeletonLoader.h"
#include "XMLGeometryLoader.h"
#include "XMLAnimationLoader.h"

AnimatedModelData* ColladaLoader::loadColladaModel(std::string colladaFile, int maxWeights, bool& loadedData, bool& loadedSkeletonData, bool& loadedAnimData)
{
	AnimationData* animData = nullptr;
	SkinningData skinningData;
	SkeletonData jointsData;
	MeshData* meshData = nullptr;
	bool loadedSkinningData = false;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(colladaFile.c_str());

	loadedData = false;
	loadedSkeletonData = false;
	loadedAnimData = false;

	if (!result)
	{
		std::cout << "Error loading model/animation " << colladaFile << std::endl;
	}

	pugi::xpath_node rootNode = doc.select_node("/COLLADA");
	pugi::xpath_node libraryController = doc.select_node("/COLLADA//library_controllers");
	pugi::xpath_node libraryVisualScenes = doc.select_node("/COLLADA//library_visual_scenes");

	if(HasChildren(libraryController))
	{
		// Extract skin data and joint order.
		SkinLoader skinLoader = SkinLoader(libraryController, libraryVisualScenes, maxWeights);
		skinningData = skinLoader.extractSkinData();

		loadedSkinningData = true;

		if (HasChildren(libraryVisualScenes))
		{
			SkeletonLoader jointsLoader = SkeletonLoader(libraryVisualScenes, skinningData.jointOrder, skinningData.bindPoses);
			jointsData = jointsLoader.extractBoneData(loadedSkeletonData);
		}
	}

	pugi::xpath_node libraryGeometries = doc.select_node("/COLLADA//library_geometries");
	if (HasChildren(libraryGeometries))
	{
		std::vector<VertexSkinData> verticesSkinData;
		if(loadedSkinningData)
		{
			verticesSkinData = skinningData.verticesSkinData;
		}
		GeometryLoader g = GeometryLoader(libraryGeometries, verticesSkinData);
		meshData = g.extractModelData();
	}

	loadedData = true;

	if (HasChild(rootNode, "library_animations"))
	{
		pugi::xpath_node animNode = doc.select_node("/COLLADA//library_animations");

		pugi::xpath_node jointsNode = doc.select_node("/COLLADA//library_visual_scenes");
		AnimationLoader loader = AnimationLoader(animNode, jointsNode);
		animData = loader.extractAnimation();

		loadedAnimData = true;
	}
	else
	{
		std::cout << "No animations present in file " << colladaFile << std::endl;
	}

	return new AnimatedModelData(meshData, jointsData, animData);
}