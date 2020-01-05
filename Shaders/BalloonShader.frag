#version 330 core

uniform sampler2D balloonTexture; //Sampler

in vec4 fNormal;
in vec4 fPos;
in vec4 fTexCoord; //Texturkoordinate aus VertexShader


out vec3 balloonColor;

void main()
{    
	balloonColor = texture(balloonTexture, fTexCoord.st).rgb; //Texturzugriff
}
