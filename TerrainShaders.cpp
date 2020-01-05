#include "TerrainShaders.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;



TerrainShaders::TerrainShaders() : 
	mModelLocation(-1),
	mModelInvTLocation(-1),
	mViewLocation(-1),
	mProjectionLocation(-1),
	mWorldSunDirectionLocation(-1)
{
}

void TerrainShaders::locateUniforms()
{
	glUseProgram(mShaderProgram);
	// uniform Variablen in Shadern suchen

	mModelLocation = glGetUniformLocation(mShaderProgram, "model");
	if (mModelLocation == -1)
		printf("[TerrainShaders] Model location not found\n");

	// Bonus
	mModelInvTLocation = glGetUniformLocation(mShaderProgram, "modelInvT");
	if (mModelInvTLocation == -1)
		printf("[TerrainShaders] ModelInvT location not found\n");


	mViewLocation = glGetUniformLocation(mShaderProgram, "view");
	if (mViewLocation == -1)
		printf("[TerrainShaders] View location not found\n");

	
	mProjectionLocation = glGetUniformLocation(mShaderProgram, "projection");
	if (mProjectionLocation == -1)
		printf("[TerrainShaders] Projection location not found\n");


	mWorldSunDirectionLocation = glGetUniformLocation(mShaderProgram, "worldSunDirection");
	if (mWorldSunDirectionLocation == -1)
		printf("[TerrainShaders] WorldSunDirection location not found\n");

	vec3 worldSunDirection = normalize(vec3(0.25f, 1.0f, 0.25f));
	glUniform3fv(mWorldSunDirectionLocation, 1, &worldSunDirection[0]);
}

void TerrainShaders::activate()
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	SimpleShaders::activate(); // rufe activate der vererbenden Klasse auf
}


void TerrainShaders::setModelMatrix(const mat4& transformMatrix)
{
	if (mModelLocation < 0)
		printf("[TerrainShaders] uniform location for 'model' not known\n");

	glUseProgram(mShaderProgram);												// Programm muss erst aktiviert werden
	glUniformMatrix4fv(mModelLocation, 1, GL_FALSE, &transformMatrix[0][0]);	// Matrix an Shader reichen

	// Bonus
	if (mModelInvTLocation < 0)
		printf("[TerrainShaders] uniform location for 'modelInvT' not known\n");

	mat3 normalMatrix = mat3(transformMatrix); // upper 3x3 matrix
	normalMatrix = glm::transpose(glm::inverse(normalMatrix));
	glUniformMatrix3fv(mModelInvTLocation, 1, GL_FALSE, &normalMatrix[0][0]);
}


// View Matrix
void TerrainShaders::setViewMatrix(const glm::mat4& viewMatrix)
{
	if (mViewLocation < 0)
		printf("[TerrainShaders] uniform location for 'view' not known\n");

	glUseProgram(mShaderProgram);
	glUniformMatrix4fv(mViewLocation, 1, GL_FALSE, &viewMatrix[0][0]);

}

// Projection Matrix
void TerrainShaders::setProjectionMatrix(const glm::mat4& projMatrix)
{
	if (mProjectionLocation < 0)
		printf("[TerrainShaders] uniform location for 'projection' not known\n");

	glUseProgram(mShaderProgram);
	glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, &projMatrix[0][0]);
}
