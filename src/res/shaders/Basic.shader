#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ViewFragPos;
out vec3 Normal;

void main()
{
	vec4 viewSpacePos = view * model * vec4(aPos, 1.0);
	ViewFragPos = vec3(viewSpacePos);
	Normal = vec3(view * model * vec4(aNormal, 0));

	gl_Position = projection * viewSpacePos;
};


#shader fragment
#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

out vec4 fragColor;

in vec3 ViewFragPos;
in vec3 Normal;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize(lightPos - ViewFragPos);
	vec3 norm = normalize(Normal);

	float diffStrengh = max(dot(norm, lightDir), 0.0);
	vec3 diffuse =  diffStrengh * lightColor;

	float specIntensity = 0.5;
	vec3 reflectedLightDir = reflect(-lightDir, norm);
	vec3 cameraDir =  normalize(vec3(0.0) - ViewFragPos);
	float specStrength =  pow(max(dot(cameraDir, reflectedLightDir), 0.0), 16.0);
	vec3 specular = specIntensity * specStrength * lightColor;

	vec3 resultColor = (ambient + diffuse + specular) * objectColor;

	fragColor = vec4(resultColor, 1.0);
};
