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

	float innerCutOut;
	float outerCutOut;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform DirLight dirLight;

out vec4 fragColor;

in vec3 ViewFragPos;
in vec3 Normal;
in vec2 texCoord;

void main()
{
	vec3 ambient = vec3(texture(material.diffuse, texCoord)) * light.ambient;
	
	vec3 lightDir = light.position - ViewFragPos;
	vec3 lightDirNomalized = normalize(lightDir);
	vec3 norm = normalize(Normal);

	float diff = max(dot(norm, lightDirNomalized), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));

	vec3 reflectedLightDir = reflect(-lightDirNomalized, norm);
	vec3 cameraDir =  normalize(vec3(0.0) - ViewFragPos);
	float spec = pow(max(dot(cameraDir, reflectedLightDir), 0.0), material.intensity);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));

	float lightDirLength = length(lightDir);
	float attenuation = 1 / (light.constant + (light.linear * lightDirLength) + (light.quadratic * (lightDirLength * lightDirLength)));

	vec3 resultColor = (ambient + diffuse + specular) * attenuation;

	fragColor = vec4(resultColor, 1.0);
};

vec3 calcDirLight(DirLight dirLight, vec3 normal, vec3 cameraDir){
	float diff = max(dot(normal, dirLight.direction), 0);
	vec3 diffuse = dirLight.diffuse * diff;

	vec3 reflectedLight = reflect(-dirLight.direction);
	float spec = max(dot(reflectedLight, cameraDir), 0);
	vec3 specular = dirLight.specular * spec;

	return dirLight.ambient * dirLight.diffuse + dirLight.specular;
}

vec3 calcPointLight(PointLight pointLight, float normal, vec3 cameraDir, vec3 fragPos){
	lightDirection = normalize(pointLight.position - fragPos);

	float diff = max(dot(normal, lightDirection), 0);
	vec3 diffuse = pointLight.diffuse * diff;

	vec3 reflectedLight = reflect(-lightDirection);
	float spec = max(dot(reflectedLight, cameraDir), 0);
	vec3 specular = pointLight.specular * spec;

	//calculate attenuation

	return pointLight.ambient * pointLight.diffuse + pointLight.specular;
}

//vec3 calcSpotLight(){
//}
