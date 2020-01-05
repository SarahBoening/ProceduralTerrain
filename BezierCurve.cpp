#include "BezierCurve.h"
#include <stdio.h>
#include <GL/freeglut.h>

using namespace glm;

BezierCurve::BezierCurve()
{
}

vec3 BezierCurve::deCasteljau(float t) const
{
	// -----------------------------------
	//Kopie der Kontrollpunkte
	vector<glm::vec3> controlCopy = mControlPoints;
	//Iterativer deCasteljau aus Vorlesungsskript
	for (int n = controlCopy.size() - 2; n >= 0; n--)
	{
		for (int i = 0; i <= n; i++)
		{
			controlCopy[i] = mix(controlCopy[i], controlCopy[i+1],t);
		}
	}
	return vec3(controlCopy[0]);
	// -----------------------------------
}

void BezierCurve::addControlPoint(const glm::vec3& newPoint)
{
    mControlPoints.push_back(newPoint);
}

void BezierCurve::createVAO(int resolution)
{
	
	// -----------------------------------
	begin(GL_LINE_LOOP); //Linienzug
	for (int i = 0; i <= resolution; i++)
	{
		float t = float(i) / float(resolution); //Punkt auf Kurve, resolution mal viele
		vec3 point = deCasteljau(t);
		addNormal3f(0.0f, 1.0f, 0.0f);
		addVertex3f(point.x, point.y, point.z);
	}
	end();
	// -----------------------------------
}


