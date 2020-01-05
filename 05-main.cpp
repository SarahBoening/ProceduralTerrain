// *** fractal terrain ***
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>
#include <stdlib.h> 

#include "Terrain.h"
#include "SimpleShaders.h"
#include "TerrainShaders.h"
#include "Camera.h"
#include "BezierCurve.h"
#include "Balloon.h"
#include "BalloonShaders.h"

using namespace glm;

const int width = 768;
const int height = 768;

const int terrainResolution = 256;
const float terrainHeight = 80.0f;
const vec3 terrainCenter = vec3( static_cast<float>(terrainResolution/2), 0.0f, static_cast<float>(terrainResolution/2));

Terrain* terrain = nullptr;
TerrainShaders* terrainShaders = nullptr;

// ------------------------------------
BezierCurve* bezierCurve = nullptr;
Balloon* balloon = nullptr;
BalloonShaders* balloonShaders = nullptr;
float t = 0.0f;	
// ------------------------------------

// Kamera Steuerung
Camera* camera;

void mouseMotion(int x, int y)
{
	camera->mouseMove(x, y);
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	camera->mouseButton(button, state, x, y);
	glutPostRedisplay();
}

float rotationXAngle = 0.0f;
float rotationYAngle = 0.0f;

mat4 calcTerrainTransformation(float rotationXAngle, float rotationYAngle)
{
	mat4 transformMatrix = mat4(1.0f);
	transformMatrix = rotate( transformMatrix, rotationYAngle, vec3(0.0f, 1.0f, 0.0f) );
	transformMatrix = rotate( transformMatrix, rotationXAngle, vec3(1.0f, 0.0f, 0.0f) );
	transformMatrix = translate( transformMatrix, -terrainCenter );
	return transformMatrix;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update camera
	camera->update();

	// betrachte unsere transformation des terrains als eine welt transformation
	mat4 worldMatrix = calcTerrainTransformation(rotationXAngle, rotationYAngle);

	// render terrain
	terrainShaders->setModelMatrix(worldMatrix * mat4(1.0));	// relativ zur Welt bewegt sich das Terrain nicht 
																// die einheitsmatrix soll das Prinzip verdeutlichen 
																// (TIP: der Balloon bewegt sich)
	terrainShaders->setViewMatrix(camera->getViewMatrix());
	terrainShaders->setProjectionMatrix(camera->getProjectionMatrix());
	terrainShaders->activate();
	terrain->draw();

	// ------------------------------------
	bezierCurve->draw();
	// ------------------------------------


	// ------------------------------------
	//t bleibt im gültigen Wertebereich
	if (t > 1.00f || t < 0.00f)
		t = 0.00f;
	//Kurve an Position t auswerten
	vec3 position = bezierCurve->deCasteljau(t);
	t += 0.000009f;
	//Translation des Ballons um t = Bewegung, fließt in Modelmatrix mit ein
	balloonShaders->setModelMatrix(worldMatrix*translate(mat4(1.0),position));
	balloonShaders->setViewMatrix(camera->getViewMatrix());
	balloonShaders->setProjectionMatrix(camera->getProjectionMatrix());
	balloonShaders->activate();
	balloon->draw();
	// ------------------------------------

	glutSwapBuffers();
}

void special(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP : 
		rotationXAngle -= 3.0f;
		break;
	case GLUT_KEY_DOWN : 
		rotationXAngle += 3.0f;
		break;
	case GLUT_KEY_LEFT : 
		rotationYAngle -= 3.0f;
		break;
	case GLUT_KEY_RIGHT :
		rotationYAngle += 3.0f;
		break;
	}
	calcTerrainTransformation(rotationXAngle, rotationYAngle);
	glutPostRedisplay();
}


// ------------------------------------
void keyboard(unsigned char key, int x, int y)
{
	//Alle Möglichkeiten für MAG und MIN
	switch (key) {
	case '1':
		balloonShaders->setMagTextureFilter(GL_NEAREST);
		break;
	case '2':
		balloonShaders->setMagTextureFilter(GL_LINEAR);
		break;
	case '3':
		balloonShaders->setMinTextureFilter(GL_NEAREST);
		break;
	case '4':
		balloonShaders->setMinTextureFilter(GL_LINEAR);
		break;
	case '5':
		balloonShaders->setMinTextureFilter(GL_LINEAR_MIPMAP_NEAREST);
		break;
	case '6':
		balloonShaders->setMinTextureFilter(GL_LINEAR_MIPMAP_LINEAR);
		break;
	case '7':
		balloonShaders->setMinTextureFilter(GL_NEAREST_MIPMAP_LINEAR);
		break;
	case '8':
		balloonShaders->setMinTextureFilter(GL_NEAREST_MIPMAP_NEAREST);
		break;
	}
	glutPostRedisplay();
}

// ------------------------------------


int main(int argc, char** argv)
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitContextVersion(3,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	if(glutCreateWindow("Illuminated Terrain") == 0)
	{
	   printf("Glut init failed\n");
	   return -1;
	}

	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutIdleFunc(display);


	// ------------------------------------
	glutKeyboardFunc(keyboard);
	// ------------------------------------

	// Init glew so that the GLSL functionality will be available
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
	   printf("GLEW init failed!\n");
	   return -1;
	}

	// Terrain erstellen 
	terrain = new Terrain(terrainResolution, terrainHeight);
	terrainShaders = new TerrainShaders();
	terrainShaders->loadVertexFragmentShaders("Shaders/TerrainShader.vert", "Shaders/TerrainShader.frag");
	terrainShaders->locateUniforms();

	// ------------------------------------
	bezierCurve = new BezierCurve();
	//Kurve ungefähr Mitte Terrain, verschoben und verkleinert, damit sie nicht in möglichen Bergen hängt
	bezierCurve->addControlPoint(vec3(67.4f, 45.0f, 51.2f));	//P1
	bezierCurve->addControlPoint(vec3(121.0f, 55.0f, 51.2f));	//P2
	bezierCurve->addControlPoint(vec3(135.4f, 55.0f, 136.8f));	//P3
	bezierCurve->addControlPoint(vec3(67.4f, 85.0f, 102.4f));	//P4
	bezierCurve->addControlPoint(vec3(5.0f, 60.0f, 153.6f));	//P5
	bezierCurve->addControlPoint(vec3(-5.4f, 95.0f, 119.2f));	//P6
	bezierCurve->addControlPoint(vec3(16.2f, 35.0f, 51.2f));	//P7
	bezierCurve->addControlPoint(vec3(67.4f, 45.0f, 51.2f));	//P8
	bezierCurve->createVAO(128);
	// ------------------------------------



	// ------------------------------------
	balloon = new Balloon(10.0f,128);
	balloonShaders = new BalloonShaders(512);
	balloonShaders->loadVertexFragmentShaders("Shaders/BalloonShader.vert", "Shaders/BalloonShader.frag");
	balloonShaders->locateUniforms();
	// ------------------------------------

	// Kamera Steuerung
	camera = new Camera();
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouse);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}

