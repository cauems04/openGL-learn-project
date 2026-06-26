#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;

layout (location = 1) in vec3 aColor;

layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 texCoord;

void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   
   ourColor = vec4(aColor, 1.0f);
   texCoord = aTexCoord;
};


#shader fragment
#version 330 core

out vec4 fragColor;

in vec4 ourColor;
in vec2 texCoord;

uniform sampler2D containerTex;
uniform sampler2D happyFaceTex;
uniform float visibilityTrade;

void main()
{
	// FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	fragColor = mix(texture(containerTex, texCoord), texture(happyFaceTex, vec2(1.0 - texCoord.x, texCoord.y)), visibilityTrade);
};