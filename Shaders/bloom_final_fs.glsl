#version 410
out vec4 frag_colour;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main()
{             
    const float gamma = 1.0; // 2.2
    vec3 hdrColor = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
	
    if(bloom)
	{
		hdrColor += bloomColor; // additive blending
	}
    // tone mapping
	float exposure_ = exposure * 1;
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure_);
	
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
	
	//result = bloomColor;
	//result = hdrColor;
    frag_colour = vec4(result, 1.0);
}