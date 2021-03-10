#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_texcoord;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec3 vertex_tangent;
layout(location = 4) in vec3 vertex_bitangent;

uniform mat4 depthMVP;
uniform mat4 projectionMatrix;
uniform mat4 modelviewMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightDir;
uniform vec3 eye;
uniform int outlinePass;
 
out vec3 FragPos;
out vec4 ShadowCoord;
out vec2 tex_coord;
out vec3 normal_coord;
out vec3 lightDirection;
out mat3 TBN;

void main() 
{
	mat4 MVP = projectionMatrix * viewMatrix * modelviewMatrix;
	
	vec3 position = vertex_position.xyz + (vertex_normal * 0.01 * outlinePass);
	
	gl_Position = MVP * vec4(position, 1.0);
	//FragPos = gl_Position.xyz;
	FragPos = vec3(modelviewMatrix * vec4(position, 1.0));
	
	// Same position but with lights MVP matrix
	ShadowCoord = depthMVP * vec4(vertex_position, 1.0); 
	
	//normal_coord = (transpose(inverse(MVP)) * vec4(vertex_normal, 0.0)).xyz;
	normal_coord = vertex_normal;
	
	tex_coord = vertex_texcoord;
	lightDirection = lightDir;
	
	// See https://learnopengl.com/Advanced-Lighting/Normal-Mapping
	vec3 T = normalize(vec3(modelviewMatrix * vec4(vertex_tangent, 0.0)));
	vec3 B = normalize(vec3(modelviewMatrix * vec4(vertex_bitangent, 0.0)));
	vec3 N = normalize(vec3(modelviewMatrix * vec4(vertex_normal, 0.0)));
	TBN = mat3(T, B, N);
	//TBN = transpose(mat3(T, B, N)); 
}
