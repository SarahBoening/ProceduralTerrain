#ifndef TERRAIN_SHADERS_H
#define TERRAIN_SHADERS_H

#include "SimpleShaders.h"

#include <glm/gtc/matrix_transform.hpp>

class TerrainShaders : public SimpleShaders
{
public:
	explicit TerrainShaders();
	virtual ~TerrainShaders() = default;

	void locateUniforms();
	void activate() override;

	void setModelMatrix(const glm::mat4& transformMatrix);
	void setViewMatrix(const glm::mat4& viewMatrix);
	void setProjectionMatrix(const glm::mat4& projMatrix);

	
private:
	GLint mModelLocation;
	GLint mModelInvTLocation; // Bonus
	GLint mViewLocation;
	GLint mProjectionLocation;
	
	GLint mWorldSunDirectionLocation;
};

#endif
