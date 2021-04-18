#version 420

struct DirLight 
{
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float intensity;

	int isActive; // If the light is active
};  

struct PointLight 
{    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float intensity;

	int isActive; // If the light is active
};  

struct Light 
{
    vec3 position;
	vec3 spotDir;
	float spotCutOff;
    vec3 colour;
	vec3 specularColour;
	
	float constant;
    float linear;
    float quadratic; 
};

struct PBRMaterial
{
	vec3 albedo;
	vec3 specular;
	float metallic;
	float roughness;
	float ao;
	float gMatSpecularIntensity;
	float shininess;
};


in vec3 FragPos;
in vec4 ShadowCoord;
in vec2 tex_coord;
in vec3 normal_coord;
in vec3 lightDirection;
in mat3 TBN;

//out vec4 frag_colour;
layout (location = 0) out vec4 frag_colour;
layout (location = 1) out vec4 bright_color;

layout(binding=0) uniform sampler2D diffuseTexture;
layout(binding=1) uniform sampler2D normalTexture;
layout(binding=2) uniform sampler2D specularTexture;
layout(binding=3) uniform sampler2D glowTexture;
layout(binding=4) uniform sampler2D depthTexture;
layout(binding=5) uniform sampler2D lut00;
layout(binding=6) uniform sampler2D lut01;
layout(binding=7) uniform sampler2D lut02;
layout(binding=8) uniform sampler2D lut03;
layout(binding=9) uniform sampler2D lut04;

uniform float near_plane;
uniform float far_plane;
uniform vec3 eye;
uniform int outlinePass;

//uniform DirLight dirLight;

#define NR_POINT_LIGHTS 40
uniform int numPointLights;
uniform PointLight pointLights[NR_POINT_LIGHTS];

//#define NR_SPOT_LIGHTS 4  
//uniform Light spotLights[NR_SPOT_LIGHTS];

const float PI = 3.14159265359;
  
  
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
float SpecularLighting(float spec, float gMatSpecularIntensity, float shininess, vec3 lightDir, vec3 viewDir, vec3 n);

vec3 PBR_Lighting(vec3 FragPosition, vec3 viewDir, vec3 n, Light light, PBRMaterial material)
{
	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, material.albedo, material.metallic);
	// reflectance equation
	// calculate per-light radiance
	vec3 L = normalize(light.position - FragPosition);
	vec3 H = normalize(viewDir + L);
	float distance    = length(light.position - FragPosition);
	//float attenuation = 1.0 / (distance * distance);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	vec3 radiance     = light.colour * attenuation;        
	
	// cook-torrance brdf
	float NDF = DistributionGGX(n, H, material.roughness);        
	float G   = GeometrySmith(n, viewDir, L, material.roughness);      
	vec3 F    = fresnelSchlick(max(dot(H, viewDir), 0.0), F0);       
	
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - material.metallic;
	
	vec3 numerator    = NDF * G * F;
	float denominator = 4.0 * max(dot(n, viewDir), 0.0) * max(dot(n, L), 0.0);
	vec3 specular2     = (numerator / max(denominator, 0.001));   

	float specularValue2 = SpecularLighting(material.specular.r, material.gMatSpecularIntensity, material.shininess, light.position, viewDir, n);
	
	// add to outgoing radiance Lo light_contribution
	float surfaceFactor = max(dot(n, L), 0.0);
	
	vec3 diffuse = (kD * (material.albedo / PI));
	vec3 specular = (material.specular * specularValue2) + specular2;
	
	vec3 light_contribution = (diffuse + specular) * surfaceFactor * radiance;
	
	// light.spotDir = vec3(1,0,0);
	// vec3 ld = normalize(light.position);
    // vec3 sd = normalize(vec3(-light.spotDir));  
	// float spotCutOff = cos(90*PI/180);
	// bool insideCone = dot(sd,ld) > spotCutOff;
    // if (!insideCone) // inside the cone?
	// {
		// // within cone so perform lighting
		// light_contribution = vec3(0);
	// }
	
	light_contribution = clamp(light_contribution, 0, 1);
	
	return light_contribution;
}

vec3 PBR_Lighting2(vec3 FragPosition, vec3 viewDir, vec3 n, PointLight light, PBRMaterial material)
{
	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, material.albedo, material.metallic);
	// reflectance equation
	// calculate per-light radiance
	vec3 L = normalize(light.position - FragPosition);
	vec3 H = normalize(viewDir + L);
	float distance    = length(light.position - FragPosition);
	//float attenuation = 1.0 / (distance * distance);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	vec3 radiance     = light.diffuse * attenuation * light.intensity;        
	
	// cook-torrance brdf
	float NDF = DistributionGGX(n, H, material.roughness);        
	float G   = GeometrySmith(n, viewDir, L, material.roughness);      
	vec3 F    = fresnelSchlick(max(dot(H, viewDir), 0.0), F0);       
	
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - material.metallic;
	
	vec3 numerator    = NDF * G * F;
	float denominator = 4.0 * max(dot(n, viewDir), 0.0) * max(dot(n, L), 0.0);
	vec3 specular2     = (numerator / max(denominator, 0.001));   

	float specularValue2 = SpecularLighting(material.specular.r, material.gMatSpecularIntensity, material.shininess, light.position, viewDir, n);
	
	// add to outgoing radiance Lo light_contribution
	float surfaceFactor = max(dot(n, L), 0.0);
	
	vec3 diffuse = (kD * (material.albedo / PI));
	vec3 specular = (material.specular * specularValue2) + specular2;
	
	vec3 light_contribution = (diffuse + specular) * surfaceFactor * radiance;
	
	// light.spotDir = vec3(1,0,0);
	// vec3 ld = normalize(light.position);
    // vec3 sd = normalize(vec3(-light.spotDir));  
	// float spotCutOff = cos(90*PI/180);
	// bool insideCone = dot(sd,ld) > spotCutOff;
    // if (!insideCone) // inside the cone?
	// {
		// // within cone so perform lighting
		// light_contribution = vec3(0);
	// }
	
	light_contribution = clamp(light_contribution, 0, 1);
	
	return light_contribution;
}

float when_eq(float x, float y) {
  return 1.0 - abs(sign(x - y));
}

float when_neq(float x, float y) {
  return abs(sign(x - y));
}

float when_gt(float x, float y) {
  return max(sign(x - y), 0.0);
}

float when_lt(float x, float y) {
  return max(sign(y - x), 0.0);
}

float when_ge(float x, float y) {
  return 1.0 - when_lt(x, y);
}

float when_le(float x, float y) {
  return 1.0 - when_gt(x, y);
}

float and(float a, float b) {
  return a * b;
}

vec3 and(vec3 a, vec3 b) {
  return a * b;
}

float or(float a, float b) {
  return min(a + b, 1.0);
}

vec3 or(vec3 a, vec3 b) {
  return min(a + b, 1.0);
}
//float xor(float a, float b) {
//  return (a + b) % 2.0;
//}

float not(float a) {
  return 1.0 - a;
}

float normalise(float value, float min, float max)
{
	return (value-min)/(max-min);
}

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

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
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
	celShadeDiff = lutValue3;

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

float ShadowMapping()
{
	// perform perspective divide
    vec3 projCoords = ShadowCoord.xyz / ShadowCoord.w;
	projCoords = projCoords * 0.5 + 0.5;
	float visibility = 1.0;
	float zValue = texture(depthTexture, projCoords.xy).r;
	if (zValue  <  projCoords.z)
	{
		visibility = 0.1;
	}	
	return visibility;
}

float SpecularLighting(float spec, float gMatSpecularIntensity, float shininess, vec3 lightDir, vec3 viewDir, vec3 n)
{
	float gSpecularPower = shininess;
    vec3 LightReflect = normalize(reflect(lightDir, n));
    float SpecularFactor = dot(viewDir, LightReflect);
	SpecularFactor = when_gt(SpecularFactor, 0) * pow(max(SpecularFactor, 0.0), gSpecularPower);
	float specularValue = (spec * gMatSpecularIntensity * SpecularFactor);
	return specularValue;
}

vec3 ADSE_Lighting(vec3 emissive, float emissiveness, 
				   vec3 ambient, float light_ambient,
				   vec3 diffuse, float diffuseValue,
				   vec3 specular, float specularValue,
				   float visibility)
{
	vec3 lighting = (emissive * emissiveness) 
		+ (ambient * light_ambient) 
		+ (diffuse * diffuseValue * visibility) 
		+ (specular * specularValue * visibility);
	return lighting;
}

vec3 CalcDirLight(DirLight light, PBRMaterial material, vec3 n, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(n, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, n);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = light.ambient;
    vec3 diffuse  = light.diffuse  * diff;
    vec3 specular = light.specular * spec;

    return (ambient + diffuse + specular) * light.isActive * light.intensity;
} 

vec3 CalcPointLight(PointLight light, PBRMaterial material, vec3 n, vec3 FragPosition, vec3 viewDir)
{
	// vec3 pos = light.position;
    // vec3 lightDir = normalize(pos - FragPosition);
    // // diffuse shading
    // float diff = max(dot(n, lightDir), 0.0);
    // // specular shading
    // vec3 reflectDir = reflect(-lightDir, n);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // // attenuation
    // float distance    = length(pos - FragPosition);

	// //float attenuation = 1.0 / (distance * distance);    
    // float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // // combine results
    // vec3 ambient  = light.ambient;
    // vec3 diffuse  = light.diffuse  * diff;
    // vec3 specular = light.specular * spec;

	// // ambient  *= attenuation;
    // // diffuse  *= attenuation;
    // // specular *= attenuation;

	// vec3 radiance = (ambient + diffuse + specular) * attenuation * light.intensity;

    // return radiance * light.isActive;



 	vec3 pos = light.position;
	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, material.albedo, material.metallic);
	// reflectance equation
	// calculate per-light radiance
	vec3 L = normalize(pos - FragPosition);
	vec3 H = normalize(viewDir + L);
	float distance    = length(pos - FragPosition);
	//float attenuation = 1.0 / (distance * distance);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	vec3 radiance     = light.diffuse * attenuation * light.intensity;        
	
	// cook-torrance brdf
	float NDF = DistributionGGX(n, H, material.roughness);        
	float G   = GeometrySmith(n, viewDir, L, material.roughness);      
	vec3 F    = fresnelSchlick(max(dot(H, viewDir), 0.0), F0);       
	
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - material.metallic;
	
	vec3 numerator    = NDF * G * F;
	float denominator = 4.0 * max(dot(n, viewDir), 0.0) * max(dot(n, L), 0.0);
	vec3 specular2     = (numerator / max(denominator, 0.001));   

	float specularValue2 = SpecularLighting(material.specular.r, material.gMatSpecularIntensity, material.shininess, light.position, viewDir, n);
	
	// add to outgoing radiance Lo light_contribution
	float surfaceFactor = max(dot(n, L), 0.0);
	
	vec3 diffuse = (kD * (material.albedo / PI));
	vec3 specular = (material.specular * specularValue2) + specular2;
	

	vec3 light_contribution = (diffuse + specular) * surfaceFactor * radiance;
	//light_contribution = vec3(1,0,0);
	
	// light.spotDir = vec3(1,0,0);
	// vec3 ld = normalize(light.position);
    // vec3 sd = normalize(vec3(-light.spotDir));  
	// float spotCutOff = cos(90*PI/180);
	// bool insideCone = dot(sd,ld) > spotCutOff;
    // if (!insideCone) // inside the cone?
	// {
		// // within cone so perform lighting
		// light_contribution = vec3(0);
	// }
	
	light_contribution = clamp(light_contribution, 0, 1);
	
	return light_contribution;
} 

vec3 ApplyLighting(DirLight dirLight, PBRMaterial material, vec3 n, vec3 FragPosition, vec3 viewDir)
{
	vec3 light_contribution = vec3(0);

	// For each light append contribution

	// Directional lights.
	//light_contribution += CalcDirLight(dirLight, material, n, viewDir);
	
	// Point lights.
	for(int i = 0; i < numPointLights; i++)
    {
		//light_contribution += CalcPointLight(pointLights[i], material, n, FragPosition, viewDir); 
		light_contribution += PBR_Lighting2(FragPosition, viewDir, n, pointLights[i], material);
	}

	return light_contribution;
}

void main() 
{
	//vec3 FragPosition = gl_FragCoord.xyz;
	vec3 FragPosition = FragPos;
	vec3 bn = normalize(normal_coord);
	vec2 uv = tex_coord;
	
	vec3 normal = GetNormal(texture(normalTexture, uv).xyz);
	//normal = normal_coord;
	vec3 n = normalize(TBN * normal);
	//n = normal;
	
	vec3 viewPos = eye;
	vec3 viewDir = normalize(viewPos - FragPosition);
	
	vec3 lightDir = normalize(lightDirection - FragPosition);
	//vec3 lightDir = normalize(lightDirection - FragPosition);
	//vec3 lightDir = lightDirection;
	
	float diff = max(0.0f, dot(n, lightDir));
	float spec = texture(specularTexture, uv).x;
	
	 
	vec3 diffuse = texture(diffuseTexture, uv).rgb;
    vec3 emissive = texture(glowTexture, uv).rgb;
    vec3 specular = vec3(1, 1, 1);
    vec3 ambient = vec3(1, 1, 1);
    float light_ambient = 0.0;
    float diffuseValue = diff;
    float emissiveness = 20.0;
    float specularValue = spec;
    float shininess = 1.0;
	float gMatSpecularIntensity = 0.01;
	
	specularValue = SpecularLighting(spec, gMatSpecularIntensity, shininess, lightDir, viewDir, n);
	
	float visibility = ShadowMapping();
	
	vec3 celShadeDiff = CelShading(diff);

	
	
	//SpecularColor = vec3(0,0,0);
	//diffuse = vec3(0,0,0);
	//SpecularColor = celShadeSpec;
	//diffuse = celShadeDiff;
	//emissive = vec3(0,0,0);
	ambient = vec3(0,0,0);
	light_ambient = 1.0;
	//diffuseValue = 1.0;
	visibility = 1.0;

	vec3 light_contribution = vec3(0.0);


	//light_contribution += ADSE_Lighting(emissive, emissiveness, ambient, light_ambient, diffuse, diffuseValue, specular, specularValue, visibility);
	//frag_colour = vec4(light_contribution, 1.0);
	
	// material parameters
	vec3  albedo = vec3(1,1,1);
	float metallic = 0.4;
	float roughness = 0.1;
	float ao = 0;
	vec3 lightColor = vec3(1,1,1);
	float light_constant = 0.01;
	float light_linear = 0.001;
	float light_quadratic = 0.001;
	//albedo = vec3(1.0) * diff;
	albedo = diffuse;
	//albedo = frag_colour.rgb;
	//albedo = celShadeDiff;
	
	vec3 LDIR = lightDirection * 1;

	// For each light
	Light light;
	DirLight dirLight;
	PBRMaterial material;
	material.albedo = albedo;
	material.specular = vec3(1,1,1);
	material.metallic = 0.5;
	material.roughness = 0.1;
	material.ao = 0;
	material.shininess = 1.0;
	material.gMatSpecularIntensity = 0.01;

	light.constant = 0.01;
	light.linear = 0.001;
	light.quadratic = 0.001;
	light.colour = vec3(1,1,1);
	light.position = lightDirection;
	
	dirLight.direction = vec3(0,-1,0);
	dirLight.ambient = vec3(0,0,0);
	dirLight.diffuse = vec3(1,1,1);
	dirLight.specular = vec3(1,1,1);
	dirLight.intensity = 0.1;
	dirLight.isActive = 1;

	// For each light append contribution
	//light_contribution += PBR_Lighting(FragPosition, viewDir, n, light, material);
	
    vec3 ambient2 = vec3(0.03) * material.albedo * material.ao;

	//vec3 luminanceThreshold2 = vec3(0.2126, 0.7152, 0.0722); // luma coefficients
    //float brightness2 = dot(light_contribution.rgb, luminanceThreshold2);
	//light_contribution =  light_contribution * CelShading((light_contribution.r + light_contribution.g + light_contribution.b) / 3);
	//light_contribution =  light_contribution * CelShading(brightness2);

    vec3 colour = ambient2 + (emissive * emissiveness) + (light_contribution);
   
    //colour  = colour * visibility;
	//colour = fresnelSchlick(diff, F0);

	vec3 outlineColour = vec3(1,1,1);
	vec3 S0 = step(vec3(diff), vec3(0));
	vec3 S1 = step(vec3(diff), vec3(1) * 1.1);
	vec3 outliner = S1 - S0;

    //colour = colour + outliner * outlineColour;
	colour = colour + (outlineColour * outlinePass);
	//colour = colour + (outliner * outlineColour * outlinePass);
	
	// Final lighting calculations:
	
	colour += ApplyLighting(dirLight, material, n, FragPos, viewDir);
	
    //colour = colour / (colour + vec3(1.0)); // normalise to LDR
    //colour = pow(colour, vec3(1.0/2.2));   // gamma correct

	frag_colour = vec4(colour, 1.0);
	//frag_colour = vec4(1.0, 0, 0, 1.0);
	// check whether fragment output is higher than threshold, if so output as brightness color
	vec3 luminanceThreshold = vec3(0.2126, 0.7152, 0.0722); // luma coefficients
    float brightness = dot(frag_colour.rgb, luminanceThreshold);
	
	bright_color = vec4(frag_colour.rgb * when_gt(brightness, 1.0), 1.0);

    // if(brightness > 1.0)
	// {
		// bright_color = vec4(frag_colour.rgb, 1.0);
	// }
    // else
	// {
		// bright_color = vec4(0.0, 0.0, 0.0, 1.0);	
	// }
	
	//frag_colour = bright_color;
	//frag_colour = vec4(frag_colour.rgb + outlineColour * dot(n,viewDir), 1.0);
	

	
	//vec2 uv2 = gl_FragCoord.xy / vec2(1024, 1024);
	
	//frag_colour = texture(diffuseTexture, uv) * visibility;
    //frag_colour = vec4(1.0,0,0,1.0);
	//frag_colour = texture(diffuseTexture, uv) * diff;
	//frag_colour = vec4(diff,diff,diff,1.0);
	//frag_colour = texture(diffuseTexture, uv);
	//
	//frag_colour = vec4(vec3(zValue), 1.0);
	//frag_colour = vec4(vec3(LinearizeDepth(zValue) / far_plane), 1.0);
	
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}