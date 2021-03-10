#ifndef WOBJ_MESH_H
#define WOBJ_MESH_H

class WOBJ_Group;
class HalfEdgeMesh;
class MeshBuffer;

#include "MeshBuffer.h"
#include "WOBJ_Group.h"
#include "HalfEdgeMesh.h"
#include <vector>

class WOBJ_Mesh : CoorinateSet
{
public:
	std::vector<WOBJ_Group*>* Groups;
	MeshBuffer* meshBuffer;
	HalfEdgeMesh* HalfEdge;

	WOBJ_Mesh()
	{
		HalfEdge = new HalfEdgeMesh();
		meshBuffer = new MeshBuffer();
		Groups = new std::vector<WOBJ_Group*>();
	}
};

#endif
