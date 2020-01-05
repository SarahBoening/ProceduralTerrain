/*
Sarah Böning 442514
Computergrafik I Blatt 5
Aufgabe 5
*/
#include "BalloonShaders.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Image.h"

using namespace glm;


BalloonShaders::BalloonShaders(int textureResolution):
	mModelLocation(-1),
	mViewLocation(-1),
	mProjectionLocation(-1),
	mTextureSamplerLocation(-1),
	mTextureID(0) // solange die textur nicht erzeugt ist, gibt es keinen fehler
{
	generateTexture(textureResolution);
}

void BalloonShaders::activate()
{
	// gef�llte polygone
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// activate texture unit 0 and bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	SimpleShaders::activate(); // rufe activate der vererbenden Klasse auf
}

// ----------------------------------------------------------------------------
// Uniforms
// ----------------------------------------------------------------------------

void BalloonShaders::locateUniforms()
{
	mat4 identityMatrix;
	mModelLocation = glGetUniformLocation(mShaderProgram, "model");
	if (mModelLocation == -1)
		printf("Model location not found\n");
	glUniformMatrix4fv(mModelLocation, 1, GL_FALSE, &identityMatrix[0][0]);

	mViewLocation = glGetUniformLocation(mShaderProgram, "view");
	if (mViewLocation == -1)
		printf("View location not found\n");

	mProjectionLocation = glGetUniformLocation(mShaderProgram, "projection");
	if (mProjectionLocation == -1)
		printf("Projection location not found\n");

	mTextureSamplerLocation = glGetUniformLocation(mShaderProgram, "balloonTexture");
	if (mTextureSamplerLocation == -1)
		printf("Texture sampler not found\n");

	// assign texture unit 0 to sampler
	glUniform1i(mTextureSamplerLocation, 0); // siehe activate()-Methode: glActiveTexture(GL_TEXTURE0);
}


// Model Matrix
void BalloonShaders::setModelMatrix(const mat4& transformMatrix)
{
	if (mModelLocation < 0)
		printf("[BalloonShaders] uniform location for 'model' not known\n");

	glUseProgram(mShaderProgram);												// Programm muss erst aktiviert werden
	glUniformMatrix4fv(mModelLocation, 1, GL_FALSE, &transformMatrix[0][0]);	// Matrix an Shader reichen
}

// View Matrix
void BalloonShaders::setViewMatrix(const glm::mat4& viewMatrix)
{
	if (mViewLocation < 0)
		printf("[BalloonShaders] uniform location for 'view' not known\n");

	glUseProgram(mShaderProgram);
	glUniformMatrix4fv(mViewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

// Projection Matrix
void BalloonShaders::setProjectionMatrix(const glm::mat4& projMatrix)
{
	if (mProjectionLocation < 0)
		printf("[BalloonShaders] uniform location for 'projection' not known\n");

	glUseProgram(mShaderProgram);
	glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, &projMatrix[0][0]);
}


// ----------------------------------------------------------------------------
// Textur
// ----------------------------------------------------------------------------


void BalloonShaders::setMinTextureFilter(int filter)
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void BalloonShaders::setMagTextureFilter(int filter)
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//regelmäßige weiße Streifen setzen, 8 weiß, 8 bunt, imageResolution = 2*(8*x) => alle 32 Pixel abwechselnd
bool static isWhite(int i, int imageResolution)
{
	return (i >= 0 && i <= (imageResolution / 16 - 1) || i >= (imageResolution / 16 * 2) && i <= (imageResolution / 16 * 3 - 1) ||
		i >= (imageResolution / 16 * 4) && i <= (imageResolution / 16 * 5 - 1) || i >= (imageResolution / 16 * 6) && i <= (imageResolution / 16 * 7 - 1) ||
		i >= (imageResolution / 16 * 8) && i <= (imageResolution / 16 * 9 - 1) || i >= (imageResolution / 16 * 10) && i <= (imageResolution / 16 * 11 - 1) ||
		i >= (imageResolution / 16* 12) && i <= (imageResolution / 16 * 13 - 1) || i >= (imageResolution / 16 * 14) && i <= (imageResolution / 16 * 15 - 1));
}
void BalloonShaders::generateTexture(int imageResolution)
{
	/* TODO Balloon texture
	Hier muss zun�chst ein quadratisches Bild der Image-Klasse angelegt,
	und ein Farbverlauf (von links nach rechts) nach dem HSV-Modell generiert werden.
	Zum Testen der Texturfilter soll der Farbverlauf durch regelm��ige wei�e Streifen im Bild unterbrochen werden (siehe Abbildung).
	Weiterhin sollen die ben�tigten OpenGL-Kommandos f�r eine Textur angegeben werden:
	(TexturID generieren, Default-Filter setzen, Clamp-Mode setzen, Texturdaten an GPU schicken und MipMaps generieren).
	*/
	//------------------------------------
	Image* image = new Image(imageResolution, imageResolution);
	/*image->clear(vec3(0.0, 1.0, 0.0));
	for (int i = 0; i < imageResolution; i++)
	{
		for (int j = 0; j < imageResolution; j++)
		{
			//Rot-blau-Farbverlauf, nicht der HSV-Farbverlauf...
			image->putPixel(i,j, mix(vec3(1.0,0.0,0.0), vec3(0.0,0.0,1.0), float(i)/float(imageResolution)));
			//regelmäßige weiße Streifen, alle 32 Pixel abwechselnd
			if (isWhite(i, imageResolution))
				image->putPixel(i, j, vec3(1.0));
		}
	}
*/	vec3 color;
	for (int y = 0; y < imageResolution; y++)
	{
		for (int x = 0; x < imageResolution; x++)
		{
			int stripe = x / 16;
			if (stripe % 2)
			{
				float H = float(x) / float(imageResolution);
				color.r = fabs(H * 6.0 - 3.0);
				color.g = 2 - fabs(H * 6.0 - 2.0);
				color.b = 2 - fabs(H * 6.0 - 4.0);
				image->putPixel(x, y, color);
			}
			else
				image->putPixel(x, y, vec3(1.0));
		}
	}
	float* pixelData = image->getData(); // Adresse der Pixeldaten im Hauptspeicher
	//Alle benötigten OpenGL-Kommandos für Textur
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, imageResolution, imageResolution, 0, GL_RGB, GL_FLOAT, pixelData);
	glGenerateMipmap(GL_TEXTURE_2D);

	//------------------------------------
}

