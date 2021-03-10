#ifndef XML_COLLADA_LOADER
#define XML_COLLADA_LOADER

#include <string>
#include "AnimatedModelData.h"
#include "SkinningData.h"
#include "AnimationData.h"
#include "Texture.h"

class ColladaLoader 
{
public:
	static AnimatedModelData* loadColladaModel(std::string colladaFile, int maxWeights, bool& loadedData, bool& loadedSkeletonData, bool& loadedAnimData);
};

#endif