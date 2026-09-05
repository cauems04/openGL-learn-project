#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ViewFragPos;
out vec3 Normal;
out vec2 texCoord;

void main()
{
	vec4 viewSpacePos = view * model * vec4(aPos, 1.0);
	ViewFragPos = vec3(viewSpacePos);
	Normal = vec3(view * model * vec4(aNormal, 0));

	texCoord =  aTexCoord;

	gl_Position = projection * viewSpacePos;
};


#shader fragment
#version 330 core

#define NR_POINT_LIGHTS 2

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float intensity;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float innerCutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;
};

vec3 calcDirLight(DirLight light, vec3 normal, vec3 cameraDir, vec3 diffColor, vec3 specColor);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 cameraDir, vec3 diffColor, vec3 specColor);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 cameraDir, vec3 diffColor, vec3 specColor);

uniform Material material;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

out vec4 fragColor;

in vec3 ViewFragPos;
in vec3 Normal;
in vec2 texCoord;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 cameraDir = normalize(-ViewFragPos);

	vec3 diffColor = vec3(texture(material.diffuse, texCoord));
	vec3 specColor = vec3(texture(material.specular, texCoord));

	vec3 result = vec3(0.0);

	// Directional Light
	result += calcDirLight(dirLight, norm, cameraDir, diffColor, specColor);
	
	// Point Light
	for (int i = 0; i < NR_POINT_LIGHTS; i++){
		result += calcPointLight(pointLights[i], norm, cameraDir, diffColor, specColor);
	}

	// SpotLight
	result += calcSpotLight(spotLight, norm, cameraDir, diffColor, specColor);

	fragColor = vec4(result, 1.0);
};

vec3 calcDirLight(DirLight light, vec3 normal, vec3 cameraDir, vec3 diffColor, vec3 specColor){
	vec3 lightDir = normalize(dirLight.direction);

	float diff = max(dot(normal, lightDir), 0);

	vec3 reflectedLight = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectedLight, cameraDir), 0), material.intensity);

	vec3 ambient = light.ambient * diffColor;
	vec3 diffuse = light.diffuse * diff * diffColor;
	vec3 specular = light.specular * spec * specColor;

	return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 cameraDir, vec3 diffColor, vec3 specColor){
	vec3 lightDirection = normalize(light.position - ViewFragPos);

	float diff = max(dot(normal, lightDirection), 0);

	vec3 reflectedLight = reflect(-lightDirection, normal);
	float spec = pow(max(dot(reflectedLight, cameraDir), 0), material.intensity);

	float lightLength = length(light.position - ViewFragPos);
	float attenuation = 1 / (light.constant + (light.linear * lightLength) + (light.quadratic * (pow(lightLength, 2))));

	vec3 ambient = light.ambient * diffColor;
	vec3 diffuse = light.diffuse * diff * diffColor;
	vec3 specular = light.specular * spec * specColor;

	return (ambient + diffuse + specular) * attenuation;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 cameraDir, vec3 diffColor, vec3 specColor){
	vec3 fragToLightDirection = normalize(light.position - ViewFragPos);

	float diff = max(dot(normal, fragToLightDirection), 0);

	vec3 reflectedLight = reflect(-fragToLightDirection, normal);
	float spec = pow(max(dot(reflectedLight, cameraDir), 0), material.intensity);

	float lightLength = length(light.position - ViewFragPos);
	float attenuation = 1 / (light.constant + (light.linear * lightLength) + (light.quadratic * (pow(lightLength, 2))));

	float lightToFragDirectionAngle = max(dot(normalize(light.direction), -fragToLightDirection), 0);
	float cutOff = (lightToFragDirectionAngle - light.outerCutOff) /  (light.innerCutOff - light.outerCutOff);
	cutOff = max(min(cutOff, 1), 0);

	vec3 ambient = light.ambient * diffColor;
	vec3 diffuse = light.diffuse * diff * diffColor;
	vec3 specular = light.specular * spec * specColor;

	return (ambient + diffuse + specular) * (attenuation * cutOff);
}
