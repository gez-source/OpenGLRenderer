#version 420

in vec3 colour;

//out vec4 frag_colour;
layout (location = 0) out vec4 frag_colour;

void main() 
{	
	frag_colour = vec4(colour, 1.0);
	//frag_colour = vec4(vec3(1,1,1), 1.0);
}