#version 410

layout(location = 0) out vec4 frag_colour;

in vec2 TexCoords;

uniform sampler2D cursorTexture;
uniform vec2 textureSize;

void main()
{
	//vec2 uv = gl_FragCoord.xy / textureSize.xy;
	vec4 colour = texture(cursorTexture, TexCoords).rgba;

	if(colour.a < 0.1)
	{
		discard;
	}

	//frag_colour = vec4(1,1,1,1);
	//frag_colour = vec4(1,0,0,0.5);
	frag_colour = colour;
}