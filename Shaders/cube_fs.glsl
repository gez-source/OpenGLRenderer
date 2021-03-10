#version 420

struct Light 
{
    vec3 position;
    vec3 colour;
	vec3 specularColour;
	
	float constant;
    float linear;
    float quadratic; 
};
const int NR_LIGHTS = 32;

in vec3 FragPos;
in vec3 textureCoord;
in vec3 normal;
in mat3 TBN;

layout(binding=0) uniform samplerCube cubemapDiffuse;
layout(binding=1) uniform samplerCube cubemapSpecular;
layout(binding=2) uniform sampler2D lut00;
layout(binding=3) uniform sampler2D lut01;
layout(binding=4) uniform sampler2D lut02;
layout(binding=5) uniform sampler2D lut03;
layout(binding=6) uniform sampler2D lut04;

uniform vec3 lightDirection;
uniform vec3 eye;
uniform Light lights[NR_LIGHTS];
uniform float material_shininess = 1.0;

out vec4 frag_colour;

void main()
{             
	//vec3 FragPosition = gl_FragCoord.xyz;
	vec3 FragPosition = FragPos;
	vec3 textureDir = textureCoord; // direction vector representing a 3D texture coordinate
	
	vec3 n = normalize(TBN * normal);
	//n = normal;
	
	vec3 viewPos = eye;
	vec3 viewDir = TBN * normalize(viewPos - FragPosition);
	
	//vec3 lightDir = lightDirection;
	//vec3 lightDir = TBN * (lightDirection);
	vec3 lightDir = TBN * normalize(lightDirection - FragPosition);
	
	float diff = max(0.f, dot(n, lightDir));
	vec3 diffuseTexture = texture(cubemapDiffuse, textureDir).rgb;
	vec3 specularTexture = texture(cubemapSpecular, textureDir).rgb;
	//vec3 lighting = diffuse * diff;
	//vec3 lighting = diffuse;
	vec3 lighting = vec3(0,0,0);
	
	// https://learnopengl.com/Lighting/Multiple-lights
	for(int i = 0; i < NR_LIGHTS; ++i)
    {
		Light light = lights[i];
        float distance = length(light.position - FragPosition);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        vec3 surfaceToLight =  TBN * normalize(light.position - FragPosition);
		vec3 light_halfwayDir = normalize(surfaceToLight + viewDir);
		//float light_diffuse = dot(n, light_halfwayDir);
		float light_diffuse = dot(n, surfaceToLight);
		light_diffuse = clamp(light_diffuse, 0.0, 1.0); 
		
		vec3 reflectDir = reflect(-surfaceToLight, n);
		float light_specular = pow(max(dot(surfaceToLight, reflectDir), 0.0), material_shininess);
		//float light_specular = pow(max(dot(light_halfwayDir, reflectDir), 0.0), material_shininess);
		//float light_specular = pow(max(dot(n, light_halfwayDir), 0.0), material_shininess);
		
		//light_diffuse = dot(n, surfaceToLight) / (length(surfaceToLight) * length(n));
		//float light_diffuse = max(dot(n, light_halfwayDir), 0.0);
		
		vec3 diffuse = (light_diffuse * diffuseTexture);
		vec3 specular = (light_specular * specularTexture);
		vec3 radiance = light.colour * attenuation;
        vec3 light_contribution = (diffuse + specular) * radiance;
		
        lighting += light_contribution;
    }
	
	//float nn = clamp(lighting.r, 0, 1);
	float nn = lighting.r;
	// vec3 luminanceThreshold = vec3(0.2126, 0.7152, 0.0722); // luma coefficients
    // float brightness = dot(lighting.rgb, luminanceThreshold);
	
	// if(brightness > 1)
	// {
		// lighting = vec3(brightness * 2);
	// }
	// else{
		// lighting = vec3(0);
	// }
	
	vec3 lutValue0 = texture(lut00, vec2(nn, 0)).rgb;
	vec3 lutValue1 = texture(lut01, vec2(nn, 0)).rgb;
	vec3 lutValue2 = texture(lut02, vec2(nn, 0)).rgb;
	vec3 lutValue3 = texture(lut03, vec2(nn, 0)).rgb;
	vec3 lutValue4 = texture(lut04, vec2(nn, 0)).rgb;
	//lighting = lighting * (lutValue4);
	//lighting = (lighting) * (brightness * 0.03);
    frag_colour = vec4(lighting * diffuseTexture, 1.0);
	//frag_colour = vec4(1,0,0,1);
} 