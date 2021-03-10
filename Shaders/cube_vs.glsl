#version 410
//#extension GL_ARB_draw_instanced : enable
//#extension GL_EXT_draw_instanced : enable

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_textureCoord;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in vec3 in_bitangent;
layout(location = 5) in mat4 instanceTransform;

out vec3 FragPos;
out vec3 textureCoord;
out vec3 normal;
out mat3 TBN;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

//const int MAX_CHUNKS = 1000; // limitation to just 1000 uniform variables
//uniform mat4 chunkTransforms[MAX_CHUNKS];
//uniform vec3 chunkTransforms[MAX_CHUNKS];

void main(void)
{
	//vec3 instanceTransform = chunkTransforms[gl_InstanceIDARB];
	//mat4 instanceTransform = chunkTransforms[gl_InstanceIDARB]; // gl_InstanceIDEXT gl_InstanceID
	mat4 MVP = projectionMatrix * viewMatrix * modelMatrix * instanceTransform;
	
	gl_Position = MVP * vec4(in_position, 1.0);
	//FragPos = gl_Position.xyz;
	FragPos = vec3(modelMatrix * instanceTransform * vec4(in_position, 1.0));
	
	normal = in_normal;
	textureCoord = in_position.xyz;
	
	vec3 T = normalize(vec3(modelMatrix * vec4(in_tangent, 0.0)));
	//vec3 B = normalize(vec3(modelMatrix * vec4(in_bitangent, 0.0)));
	vec3 N = normalize(vec3(modelMatrix * vec4(in_normal, 0.0)));
	
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);
	
	TBN = mat3(T, B, N);
	TBN = transpose(TBN);
}