#version 410

//layout(location = 0) out vec4 frag_colour;
layout(location = 0) out float frag_colour;

void main()
{
	//frag_colour = vec4(vec3(gl_FragCoord.z), 1.0);
	//frag_colour = (gl_FragCoord.z + 1) / 2;
	frag_colour = gl_FragCoord.z;
}