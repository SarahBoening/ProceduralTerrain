#ifndef BALLOON_SHADERS_H
#define BALLOON_SHADERS_H

#include "SimpleShaders.h"
#include <glm/glm.hpp>

class BalloonShaders : public SimpleShaders
{
public:
	explicit BalloonShaders(int textureResolution);
	virtual ~BalloonShaders() = default;

	void locateUniforms();
	void activate() override;
	
	void setModelMatrix(const glm::mat4& transformMatrix);
	void setViewMatrix(const glm::mat4& viewMatrix);
	void setProjectionMatrix(const glm::mat4& projectionMatrix);

	void setMinTextureFilter(int filter);
	void setMagTextureFilter(int filter);
	 
private:

	void generateTexture(int resolution);

	GLint mModelLocation;	
	GLint mViewLocation;	
	GLint mProjectionLocation;			
	GLint mTextureSamplerLocation;	

	GLuint mTextureID;
};

#endif
