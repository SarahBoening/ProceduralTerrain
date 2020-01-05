#include "SimpleShaders.h"
#include <iostream>
#include <fstream>

using namespace std;

SimpleShaders::SimpleShaders()
{

}

SimpleShaders::~SimpleShaders()
{

}

// load vertex and fragment shaders, create and activate shader program, check for errors
bool SimpleShaders::loadVertexFragmentShaders(const char* vertexShaderFilename, const char* fragmentShaderFilename)
{
	// Create empty shader object (vertex shader)
	mVertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Read vertex shader source 
	string shaderSource = readFile( vertexShaderFilename );
	const char* sourcePtr = shaderSource.c_str();

	// Attach shader code
	glShaderSource(mVertexShader, 1, &sourcePtr, NULL);	

	// Compile
	glCompileShader(mVertexShader);
	printShaderInfoLog(mVertexShader);

	// Create empty shader object (fragment shader)
	mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read vertex shader source 
	shaderSource = readFile( fragmentShaderFilename );
	sourcePtr = shaderSource.c_str();

	// Attach shader code
	glShaderSource(mFragmentShader, 1, &sourcePtr, NULL);	

	// Compile
	glCompileShader(mFragmentShader);
	printShaderInfoLog(mFragmentShader);

	// Create shader program
	mShaderProgram = glCreateProgram();	

	// Attach shader
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragmentShader);

	// Link program
	glLinkProgram(mShaderProgram);
	printProgramInfoLog();


	printf("Vertex/Fragment Shaders loaded\n");
	return true;
}

void SimpleShaders::activate()
{
	glUseProgram(mShaderProgram);
}

void SimpleShaders::deactivate()
{
	glUseProgram(0);
}

// Reads a file and returns the content as a string
string SimpleShaders::readFile(string fileName)
{
	string fileContent;
	string line;

	ifstream file(fileName.c_str());
	if (file.is_open()) {
		while (!file.eof()){
			getline (file,line);
			line += "\n";
			fileContent += line;					
		}
		file.close();
	}
	else
		cout << "ERROR: Unable to open file " << fileName << endl;

	return fileContent;
}

// Print information about the compiling step
void SimpleShaders::printShaderInfoLog(GLuint shader)
{
    GLint infologLength = 0;
    GLsizei charsWritten  = 0;
    char *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

// Print information about the linking step
void SimpleShaders::printProgramInfoLog()
{
	GLint infoLogLength = 0;
	GLsizei charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(mShaderProgram, GL_INFO_LOG_LENGTH,&infoLogLength);
	if (infoLogLength > 0)
	{
		infoLog = (char *)malloc(infoLogLength);
		glGetProgramInfoLog(mShaderProgram, infoLogLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

