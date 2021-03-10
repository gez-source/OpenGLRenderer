#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_textureCoords;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in ivec3 in_jointIndices;
layout(location = 4) in vec3 in_weights;
layout(location = 5) in vec3 in_tangent;
layout(location = 6) in vec3 in_bitangent;

const int MAX_JOINTS = 100;//max joints allowed in a skeleton
const int MAX_WEIGHTS = 3;//max number of joints that can affect a vertex

out vec3 FragPos;
out vec2 pass_textureCoords;
out vec3 pass_normal;
out mat3 TBN;

uniform mat4 jointTransforms[MAX_JOINTS];
uniform mat4 projectionMatrix;
uniform mat4 modelviewMatrix;
uniform mat4 viewMatrix;
uniform vec3 eye;

void main(void)
{
	mat4 MVP = projectionMatrix * viewMatrix * modelviewMatrix;
	
	gl_Position = MVP * vec4(in_position, 1.0); // Without skeletal animation data
	//FragPos = gl_Position.xyz;
	FragPos = vec3(modelviewMatrix * vec4(in_position, 1.0));
	
	pass_normal = normalize(vec3(modelviewMatrix * vec4(in_normal, 0.0)));

	pass_textureCoords = in_textureCoords;
	
	// See https://learnopengl.com/Advanced-Lighting/Normal-Mapping
	vec3 T = normalize(vec3(modelviewMatrix * vec4(in_tangent, 0.0)));
	//vec3 B = normalize(vec3(modelviewMatrix * vec4(in_bitangent, 0.0)));
	vec3 N = normalize(vec3(modelviewMatrix * vec4(in_normal, 0.0)));
	
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);
	
	TBN = mat3(T, B, N);
	//TBN = transpose(TBN);
}