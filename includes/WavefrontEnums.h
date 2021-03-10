#ifndef WAVEFRONT_ENUMS_H
#define WAVEFRONT_ENUMS_H

enum OBJToken
{
	WF_UNDEFINED = 0,
	WF_Object = 1,
	WF_Group = 2,
	WF_Vertex = 3,
	WF_VertexParameterSpace = 4,
	WF_VertexTextureCoordinate = 5,
	WF_VertexNormal = 6,
	WF_Face = 7,
	WF_MaterialDefinition = 8, // MTL file defined by: mtllib
	WF_MaterialReference = 9, // Material reference defined by: usemtl
	WF_SmoothShadingParameter = 10,
	WF_CommentLine = 11
};

#endif
