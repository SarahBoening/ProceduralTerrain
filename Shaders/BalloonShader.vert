#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec4 vPos;
layout(location = 2) in vec4 vNormal;
layout (location = 3) in vec4 vTexCoord;

out vec4 fPos;
out vec4 fNormal;
out vec4 fTexCoord;

void main()
{
	gl_Position = projection * view * model * vPos;
    fPos = vPos;
    fNormal = vNormal;
	fTexCoord = vTexCoord; //Texturkoordinate weitergeben
}
