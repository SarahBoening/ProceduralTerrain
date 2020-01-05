#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <vector>

// we use this class to make VAOs with OpenGL easier
class VertexArrayObject {

public:
	explicit VertexArrayObject();
	virtual ~VertexArrayObject() = default;

	void begin(unsigned int drawMode);

	void addVertex2f(float x, float y);
	void addVertex3f(float x, float y, float z);
	void addColor3f(float r, float g, float b);
	void addNormal3f(float x, float y, float z);
	void addTexCoord2f(float s, float t);
	void addIndex1ui(unsigned int i);

	void end();

	void draw();

protected:

	GLuint mDrawMode;     // stores the vao drawmode, can be GL_TRIANGLES, GL_POINTS, ...
	GLuint mVAO;          // VAO ID

	GLuint mPositionBufferHandle;    // all IDs for the VBOs
	GLuint mColorBufferHandle;
	GLuint mNormalBufferHandle;
	GLuint mTexCoordBufferHandle;
	GLuint mIndexBufferHandle;

	std::vector<float> mPositions;        // the VBO data is stored in dynamic arrays
	std::vector<float> mColors;
	std::vector<float> mNormals;
	std::vector<float> mTexCoords;
	std::vector<unsigned int> mIndices;
};


#endif
