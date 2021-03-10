#version 410

layout(location = 0) out vec4 frag_colour;

in vec4 VertexCoord;

uniform sampler2D depthTexture;
uniform vec2 framebufferSize;
uniform float near_plane;
uniform float far_plane;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{
	vec2 uv = gl_FragCoord.xy / framebufferSize.xy;
	
	float zValue = texture(depthTexture, uv).z;
	
	//frag_colour = vec4(vec3(zValue), 1.0);
	
	frag_colour = vec4(vec3(LinearizeDepth(zValue) / far_plane), 1.0);
	
	//frag_colour = vec4(1,0,0,1);
	
	
}