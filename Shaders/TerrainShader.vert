#version 330 core

// Vertex Shader Terrain


uniform mat4 model;
uniform mat3 modelInvT;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 worldSunDirection; // for shading in world space, this is not needed here (but in the fragment program)

layout(location = 0) in vec4 vPos;
layout(location = 2) in vec4 vNormal;

// for diffuse color
out float height;

// for shading in world space
out vec3 fWorldPos;
out vec3 fWorldNormal;
out vec3 fWorldCam;
out vec3 fWorldCamDir;

// for shading in view space
out vec3 fViewPos;
out vec3 fViewNormal;
out vec3 fViewSunDirection;

void main()
{
	// vertex transformation
	gl_Position = projection * view * model * vPos;

	// for diffuse color
	height = vPos.y;

	// for shading in world space
	//-------------------------------------------------------------------------
	fWorldPos = (model * vPos).xyz;
	fWorldNormal = normalize(modelInvT * vNormal.xyz);									// Bonus
	//fWorldNormal = normalize((model * vNormal).xyz);									// no-Bonus
	fWorldCam = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	fWorldCamDir = normalize((inverse(view) * vec4(0.0, 0.0, -1.0, 0.0)).xyz);

	// for shading in view space
	//-------------------------------------------------------------------------
	fViewPos = (view * model * vPos).xyz;
	fViewNormal = normalize((view * vec4(modelInvT * vNormal.xyz, 0.0)).xyz);			// Bonus
	//fViewNormal = normalize((view * model * vNormal).xyz);							// no-Bonus
	fViewSunDirection = normalize((view * vec4(worldSunDirection, 0.0)).xyz);
}

