#version 410

in vec3 textureCoord;
in vec3 normal;

uniform samplerCube cubemap;

out vec4 out_colour;

void main()
{             
	vec3 textureDir = textureCoord; // direction vector representing a 3D texture coordinate
    out_colour = texture(cubemap, textureDir);
	//out_colour = vec4(1,0,0,1);
} 