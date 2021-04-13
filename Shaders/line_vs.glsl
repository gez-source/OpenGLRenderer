#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

uniform mat4 projectionMatrix;
uniform mat4 modelviewMatrix;
uniform mat4 viewMatrix;
 
out vec3 colour;

void main() 
{
	mat4 MVP = projectionMatrix * viewMatrix * modelviewMatrix;
	
	vec3 position = vertex_position.xyz;
	
	gl_Position = MVP * vec4(position, 1.0);
	
	colour = vertex_colour;
}
