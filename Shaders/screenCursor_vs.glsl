#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_texcoord; 

uniform mat4 MVP;

out vec2 TexCoords;

void main() 
{
	TexCoords = vertex_texcoord;
	gl_Position = MVP * vec4(vertex_position, 1.0);
}