#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 WorldFragPos;
out vec3 Normal;

void main()
{
	vec4 worldSpacePos = model * vec4(aPos, 1.0);
	WorldFragPos = vec3(worldSpacePos);
	Normal = vec3(model * vec4(aNormal, 0));

	gl_Position = projection * view * worldSpacePos;
};


#shader fragment
#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

out vec4 fragColor;

in vec3 WorldFragPos;
in vec3 Normal;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize(lightPos - WorldFragPos);
	float diffStrengh = max(dot(normalize(Normal), lightDir), 0.0);
	vec3 diffuse =  diffStrengh * lightColor;

	vec3 resultColor = (ambient + diffuse) * objectColor;

	fragColor = vec4(resultColor, 1.0);
};
