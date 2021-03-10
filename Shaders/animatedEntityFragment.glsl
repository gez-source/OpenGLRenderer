#version 420

const vec2 lightBias = vec2(0.7, 0.6);//just indicates the balance between diffuse and ambient lighting

in vec3 FragPos;
in vec2 pass_textureCoords;
in vec3 pass_normal;
in mat3 TBN;

out vec4 frag_colour;

layout(binding=0) uniform sampler2D diffuseTexture;
layout(binding=1) uniform sampler2D normalTexture;
layout(binding=2) uniform sampler2D specularTexture;
layout(binding=3) uniform sampler2D glowTexture;
layout(binding=4) uniform sampler2D lut00;
layout(binding=5) uniform sampler2D lut01;
layout(binding=6) uniform sampler2D lut02;
layout(binding=7) uniform sampler2D lut03;
layout(binding=8) uniform sampler2D lut04;

uniform vec3 lightDirection;
uniform vec3 eye;

vec3 GetNormal(vec3 colour)
{
	vec3 result; // Convert from a colour space to a normal vector in range [-1,1]
	//colour=vec3(128, 128, 255);
	result = vec3(
		colour.x * 2.f - 1.f, 
		colour.y * 2.f - 1.f, 
		colour.z * 2.f - 1.f
	);
	return result;
}

vec3 CelShading(float diff)
{
	float nn = clamp(diff, 0, 1);
	vec3 lutValue0 = texture(lut00, vec2(nn, 0)).rgb;
	vec3 lutValue1 = texture(lut01, vec2(nn, 0)).rgb;
	vec3 lutValue2 = texture(lut02, vec2(nn, 0)).rgb;
	vec3 lutValue3 = texture(lut03, vec2(nn, 0)).rgb;
	vec3 lutValue4 = texture(lut04, vec2(nn, 0)).rgb;
	
	vec3 celShadeDiff = vec3(0,0,0);
	
	// celShadeDiff = (vec3(0,1,0) * when_gt(nn, 0.9))
					// + (vec3(0,0,1) * and(when_gt(nn, 0.75), when_lt(nn, 0.9)))
					// + (vec3(1,0,0) * and(when_gt(nn, 0.1), when_lt(nn, 0.75)))					
					// + diffuse;
					
	//celShadeDiff = vec3(1) * (lutValue4.r);
	celShadeDiff = lutValue4;

	// if(diff > 0.5)
	// {
		// celShadeDiff = diffuse * 2.0;
	// }
	// else if(diff < -0.5)
	// {
		// celShadeDiff = diffuse / 2.0;
	// }
	// else
	// {
		// celShadeDiff = diffuse * 0.5;
	// }
	
	return celShadeDiff;
}

void main(void)
{
	//vec3 FragPosition = gl_FragCoord.xyz;
	vec3 FragPosition = FragPos;
	vec2 uv = pass_textureCoords;
	
	vec4 diffuseColour = texture(diffuseTexture, uv);

	vec3 normal = GetNormal(texture(normalTexture, uv).xyz);
	normal = pass_normal;
	vec3 n = normalize(TBN * normal);
	n = normal;
	
	vec3 viewPos = eye;
	//vec3 viewDir = TBN * normalize(viewPos - FragPosition);
	vec3 viewDir = normalize(viewPos - FragPosition);
	
	//vec3 lightDir = TBN * normalize(lightDirection - FragPosition);
	vec3 lightDir = normalize(lightDirection - FragPosition);
	
	float diff = max(0.f, dot(n, lightDir));
	float spec = texture(specularTexture, uv).x;
	
	
 	vec3 diffuse = texture(diffuseTexture, uv).rgb;
    vec3 emissive = texture(glowTexture, uv).rgb;
    vec3 specular = vec3(1, 1, 1);
    vec3 ambient = vec3(1, 1, 1);
    float light_ambient = 0.0;
    float diffuseValue = diff;
    float emissiveness = 1.0;
    float specularValue = spec;
    float shininess = 1.0;

	vec3 VertexToEye = viewDir;
    //vec3 VertexToEye = normalize(eye - FragPosition);
    vec3 LightReflect = normalize(reflect(lightDir, n));
    float SpecularFactor = dot(VertexToEye, LightReflect);
    vec3 SpecularColor = vec3(0,0,0);
	
    if (SpecularFactor > 0)
    {
        float gMatSpecularIntensity = 1.0;
        float gSpecularPower = shininess;
        SpecularFactor = pow(max(SpecularFactor, 0.0), gSpecularPower);

        SpecularColor = specular;

        specularValue = (spec * gMatSpecularIntensity * SpecularFactor);
    }
	
	vec3 celShadeDiff = CelShading(diff);
	
	SpecularColor = vec3(0,0,0);
	//diffuse = vec3(0,0,0);
	//SpecularColor = celShadeSpec;
	diffuse = celShadeDiff;
	//diffuse = vec3(1,0,0);
	emissive = vec3(0,0,0);
	ambient = vec3(0,0,0);
	light_ambient = 1.0;
	
	float visibility = 1.0;
	
	
	frag_colour = vec4(
		(emissive * emissiveness) 
		+ (ambient * light_ambient) 
		+ (diffuse * diffuseValue * visibility) 
		+ (SpecularColor * specularValue * visibility)
	, 1.0); 
	
	//float diffuseLight = diff;// * lightBias.x + lightBias.y;
	//frag_colour = diffuseColour * diffuseLight;
	//frag_colour = vec4(1,0,0,1);
}