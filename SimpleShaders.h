#ifndef SIMPLE_SHADERS_H
#define SIMPLE_SHADERS_H

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <string>

class SimpleShaders
{
public:
	explicit SimpleShaders();
	virtual ~SimpleShaders();

	bool loadVertexFragmentShaders(const char* vertexShaderFilename, const char* fragmentShaderFilename);

	virtual void activate();
	virtual void deactivate();

protected:
	std::string readFile(std::string fileName);

	void printShaderInfoLog(GLuint shader);
	void printProgramInfoLog();

	GLuint mVertexShader;	
	GLuint mFragmentShader;
	GLuint mShaderProgram;
};

#endif
