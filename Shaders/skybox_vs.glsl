#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_textureCoord;
layout(location = 2) in vec3 in_normal;

out vec3 textureCoord;
out vec3 normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(void)
{
	mat4 MVP = projectionMatrix * viewMatrix;
	
	vec4 pos = MVP * vec4(in_position, 1.0);
	gl_Position = pos.xyww;

	normal = in_normal;
	textureCoord = in_position.xyz;
}