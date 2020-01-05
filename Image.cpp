#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

#include "Image.h"

using namespace glm;

// create a new image: allocate pixel memory and clear image
Image::Image(int width, int height)
{
	mPixels = new float[width * height * 3];
	mWidth = width;
	mHeight = height;
	clear(vec3(0.0f, 0.0f, 0.0f));
}

// free pixel memory when deleting image
Image::~Image()
{
	delete[] mPixels;
}

// set pixel color at (x,y) to color
void Image::putPixel(int x, int y, vec3 color)
{
	int offset = 3 * (y * mWidth + x);
	mPixels[offset] = color[0];
	mPixels[offset+1] = color[1];
	mPixels[offset+2] = color[2];
}

// set pixel color at (x,y) to color, round to closest pixel position
void Image::putPixel(vec2 point, vec3 color)
{
	int x = (int)(point[0] + 0.5f);
	int y = (int)(point[1] + 0.5f);
	int offset = 3 * (y * mWidth + x);
	mPixels[offset] = color[0];
	mPixels[offset+1] = color[1];
	mPixels[offset+2] = color[2];
}

// draw a line from startPoint to endPoint in color
void Image::drawLine(vec2 startPoint, vec2 endPoint, vec3 color)
{
	int numSteps;
	int deltaX = (int)( fabs(endPoint.x - startPoint.x) );
	int deltaY = (int)( fabs(endPoint.y - startPoint.y) );

	if (deltaX > deltaY)
		numSteps = deltaX;
	else
		numSteps = deltaY;

	for (int i = 0 ; i < numSteps ; i++) {

		float t = (float)i / (float)numSteps;
		vec2 point = (1.0f - t) * startPoint + t * endPoint;
		putPixel(point, color);
	}
}

// slow image clear: call putPixel for all pixels
void Image::clear(vec3 color)
{
	for (int y = 0 ; y < mHeight ; y++)
		for (int x = 0 ; x < mWidth ; x++)
			putPixel(x, y, color);
}

// copy the complete image to the GPU
void Image::copyToFramebuffer()
{
	// this the short version of this function (unfortunately deprecated)
	//
	// glDrawPixels(mWidth, mHeight, GL_RGB, GL_FLOAT, mPixels);
	// return;

	// this is the long version for modern OpenGL...

	static bool first = true;
	static GLuint vao = 0;
	static GLuint vboPos = 0;
	static GLuint vboTexCoord = 0;
	static GLuint texID = 0;
	GLint infoLogLength = 0;
	GLsizei charsWritten  = 0;
	char *infoLog;

	// make several initalizations when calling the first time...
	if (first) {

		first = false;

		// Create empty shader object (vertex shader)
		GLuint mVertexShader = glCreateShader(GL_VERTEX_SHADER);

		// vertex shader source 
		const char* sourcePtr = {
			"#version 130\n"
			"#extension GL_ARB_explicit_attrib_location : enable\n"
			"layout(location = 0) in vec2 vPos;"
			"layout(location = 3) in vec2 vTexCoord;"
			"out vec2 fTexCoord;"
			"void main() {"
			"	gl_Position = vec4(vPos, 0.0, 1.0);"
			"	fTexCoord = vTexCoord; }"
		};

		// Attach shader code
		glShaderSource(mVertexShader, 1, &sourcePtr, NULL);	

		// Compile
		glCompileShader(mVertexShader);
		// printShaderInfoLog(mVertexShader);

		// check for errors 
		glGetShaderiv(mVertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);	
		if(infoLogLength > 0)
		{	
			infoLog = (char *)malloc(infoLogLength);
			glGetShaderInfoLog(mVertexShader, infoLogLength, &charsWritten, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}

		// Create empty shader object (fragment shader)
		GLuint mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// fragment shader source 
		const char* sourcePtr2 = {
			"#version 130\n"
			"#extension GL_ARB_explicit_attrib_location : enable\n"
			"uniform sampler2D backgroundPixels;"
			"in vec2 fTexCoord;"
			"out vec4 color;"
			"void main() { color = texture2D(backgroundPixels, fTexCoord); }"
		};

		// Attach shader code
		glShaderSource(mFragmentShader, 1, &sourcePtr2, NULL);	

		// Compile
		glCompileShader(mFragmentShader);

		// check for errors
		glGetShaderiv(mFragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);	
		if(infoLogLength > 0)
		{	
			infoLog = (char *)malloc(infoLogLength);
			glGetShaderInfoLog(mFragmentShader, infoLogLength, &charsWritten, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}

		// Create shader program
		GLuint mShaderProgram = glCreateProgram();	

		// Attach shader
		glAttachShader(mShaderProgram, mVertexShader);
		glAttachShader(mShaderProgram, mFragmentShader);

		// Link program
		glLinkProgram(mShaderProgram);

		// check for linker errors
		glGetProgramiv(mShaderProgram, GL_INFO_LOG_LENGTH,&infoLogLength);
		if(infoLogLength > 0)
		{
			infoLog = (char *)malloc(infoLogLength);
			glGetProgramInfoLog(mShaderProgram, infoLogLength, &charsWritten, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}

		// activate shader program
		glUseProgram(mShaderProgram);

		// search texture sampler
		GLint mTextureSamplerLocation = glGetUniformLocation(mShaderProgram, "backgroundPixels");
		if (mTextureSamplerLocation == -1)
			printf("Texture sampler not found\n");

		// assign texture unit 0 to sampler
		glUniform1i(mTextureSamplerLocation, 0);

		printf("Simple Texture Shaders loaded\n");

		// vertices and texcoords for screen-filling quad
		GLfloat vertices[4][2] = {
			{-1.0f, -1.0f},
			{ 1.0f, -1.0f},
			{ 1.0f,  1.0f},
			{-1.0f,  1.0f}
		};	
		GLfloat texCoords[4][2] = {
			{ 0.0f,  0.0f},
			{ 1.0f,  0.0f},
			{ 1.0f,  1.0f},
			{ 0.0f,  1.0f}
		};

		// generate VAO for screen-filling quad
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// generate VBO for vertices
		glGenBuffers(1, &vboPos);
		glBindBuffer(GL_ARRAY_BUFFER, vboPos);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		// generate VBO for texcoords
		glGenBuffers(1, &vboTexCoord);
		glBindBuffer(GL_ARRAY_BUFFER, vboTexCoord);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);

		// generate a texture ID
		glGenTextures(1, &texID);
	}

	// activate texture unit 0 and bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	// set simple nearest filters since OpenGL default does not work
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// copy image to GPU as texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, mPixels);

	// draw screen-filling quad
	glBindVertexArray(vao);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindVertexArray(0);
}

float* Image::getData()
{
	return mPixels;
}

int Image::getWidth()
{
	return mWidth;
}

int Image::getHeight()
{
	return mHeight;
}
