#include <GL/freeglut.h>
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#ifndef PI
#define PI 3.141592f
#endif

#define CAMERA_ROTATE 1
#define CAMERA_MOVE 2

using namespace glm;

Camera::Camera()
{
	mOldX = 0;
	mOldY = 0;
	mMotionState = CAMERA_ROTATE;										// drehen oder bewegen
	mTheta = 2.0;								// PI / 2.0f;			// initiale Blickrichtung in (theta,phi) Koordinaten
	mPhi = 4.0;									// 3.0f * PI / 2.0f;
	mSpeed = 0.0f;														// intiale Kamerageschwindigkeit

	mMotionStep = 0.001f;												// Empfindlichkeit der Steuerung
	mThetaStep = 0.001f;
	mPhiStep = 0.003f;
	mPosition = vec3(128.0f, 64.0f, 128.0f);	// vec4(0.0f);			// initiale Kameraposition

	// Es soll eine perspektivische Projektion zur Darstellung verwendet werden.
	// Da sich diese Matrix bei uns nicht �ndert, wird sie im Konstruktor der Camera-Klasse erstellt.
	mProjectionMatrix = perspective(60.0f, 1.0f, 0.01f, 100000.0f);

}

Camera::~Camera()
{

}


void Camera::mouseButton(int button, int state, int x, int y)
{
	/* 
	Bei gedr�ckter linker Maustaste sollen die Mausbewegungen in eine �nderung der Blickrichtung (=Flugrichtung) umgesetzt werden.
	Die Mausbewegung in x-Richtung soll dabei den \phi Winkel ver�ndern, die Bewegung in y-Richtung den \theta Winkel.

	Mit der rechten Maustaste soll die Geschwindigkeit der Kamera ver�ndert werden.
	Wird die Maus bei gedr�ckter rechter Maustaste nach unten bzw. oben bewegt, so soll die Kamera abbremsen oder beschleunigen.
	*/

	mOldX = x;
	mOldY = y;

	// Linke Maustaste: Kamera drehen
	if (button == GLUT_LEFT_BUTTON) 
	{
		if (state == GLUT_DOWN) 
		{
			mMotionState = CAMERA_ROTATE;
		}
	}

	// rechte Maustaste: Kamera bewegen
	else if (button == GLUT_RIGHT_BUTTON) 
	{
		if (state == GLUT_DOWN) 
		{
			mMotionState = CAMERA_MOVE;
		}
	}

}

void Camera::mouseMove(int x, int y)
{
	// Mausbewegung in Aenderung (theta,phi) umsetzen

	int deltaX = x - mOldX;
	int deltaY = y - mOldY;

	if (mMotionState == CAMERA_ROTATE) 
	{
		mTheta += mThetaStep * static_cast<float>(deltaY);

		if (mTheta < mThetaStep) 
			mTheta = mThetaStep;
		else if (mTheta > PI - mThetaStep) 
			mTheta = PI - mThetaStep;

		mPhi += mPhiStep * static_cast<float>(deltaX);

		if (mPhi < 0.0f) 
			mPhi += 2.0f*PI;
		else if (mPhi > 2.0f*PI) 
			mPhi -= 2.0f*PI;
	}
	else if (mMotionState == CAMERA_MOVE) 
	{
		mSpeed -= mMotionStep * static_cast<float>(deltaY);
	}

	// printf("theta %f phi %f distance %f\n",mTheta,mPhi,mDistance);

	mOldX = x;
	mOldY = y;
}

void Camera::update()
{
	/*
	aus (theta, phi) die Blickrichtung berechnen,
	Kamera entlang Blickrichtung bewegen und
	danach mit aktueller Position und Richtung mit lookat() eine view-Matrix konstruieren
	*/

	// aus (theta, phi) die Blickrichtung berechnen,
	// danach mit aktueller Position und Richtung mit lookat() eine view-Matrix konstruieren 
	mDirection.x = sin(mTheta) * cos(mPhi);
	mDirection.y = cos(mTheta);
	mDirection.z = sin(mTheta) * sin(mPhi);

	// Kamera entlang Blickrichtung bewegen
	mPosition += mSpeed * mDirection;

	mViewMatrix = lookAt(mPosition, mPosition + mDirection, vec3(0.0f, 1.0f, 0.0f));
}

const glm::mat4& Camera::getViewMatrix() const
{
	return mViewMatrix;
}

const glm::mat4& Camera::getProjectionMatrix() const
{
	return mProjectionMatrix;
}
