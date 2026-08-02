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

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

out vec4 fragColor;

in vec3 ViewFragPos;
in vec3 Normal;
in vec2 texCoord;

void main()
{
	vec3 ambient = vec3(texture(material.diffuse, texCoord)) * light.ambient;

	vec3 lightDir = normalize(light.position - ViewFragPos);
	vec3 norm = normalize(Normal);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));

	vec3 reflectedLightDir = reflect(-lightDir, norm);
	vec3 cameraDir =  normalize(vec3(0.0) - ViewFragPos);
	float spec = pow(max(dot(cameraDir, reflectedLightDir), 0.0), material.intensity);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));

	vec3 resultColor = ambient + diffuse + specular;

	fragColor = vec4(resultColor, 1.0);
};
