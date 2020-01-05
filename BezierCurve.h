#ifndef _BEZIER_CURVE_H
#define _BEZIER_CURVE_H

#include "VertexArrayObject.h"
#include <glm/glm.hpp>
#include <vector>

using namespace std;

class BezierCurve : public VertexArrayObject  {
public:
    explicit BezierCurve();
    virtual ~BezierCurve() = default;

	// Wertet die Kurve an genau einer Position aus
    glm::vec3 deCasteljau(float t) const;

	// F�gt einen Kontrollpunkt hinzu
    void addControlPoint(const glm::vec3& newPoint);

	// Erzeugt die Visualisierung der Kurve
	void createVAO(int resolution);
    
private:
    vector<glm::vec3> mControlPoints;     
};

#endif

