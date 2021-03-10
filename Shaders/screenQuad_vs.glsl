#version 410

layout(location = 0) in vec3 vertex_position;

uniform mat4 MVP;

//out vec4 VertexCoord;

void main() 
{
	gl_Position  = MVP * vec4(vertex_position, 1.0);
	
	//gl_Position  = vec4(vertex_position * 0.5, 1.0);
	
	//VertexCoord  = MVP * vec4(vertex_position, 1.0);
}