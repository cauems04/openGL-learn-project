#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

out vec3 color;

void main()
{
	vec4 viewSpacePos = view * model * vec4(aPos, 1.0);
	vec3 pos = vec3(viewSpacePos);
	vec3 normal = normalize(vec3(view * model * vec4(aNormal, 0)));

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize(lightPos - pos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse =  diff * lightColor;

	float specIntensity = 1.5;
	vec3 reflectedLightDir = reflect(-lightDir, normal);
	vec3 cameraDir =  normalize(vec3(0.0) - pos);
	float spec =  pow(max(dot(cameraDir, reflectedLightDir), 0.0), 220.0);
	vec3 specular = specIntensity * spec * lightColor;

	vec3 resultColor = (ambient + diffuse + specular) * objectColor;

	color = resultColor;
	gl_Position = projection * viewSpacePos;
};


#shader fragment
#version 330 core

in vec3 color;
out vec4 fragColor;

void main()
{
	fragColor = vec4(color, 1.0);
};
