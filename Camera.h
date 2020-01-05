#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:

	Camera();
	~Camera();

	void mouseMove(int x, int y);
	void mouseButton(int button, int state, int x, int y);

	void update(); // update der view matrix

	const glm::mat4& getViewMatrix() const;
	const glm::mat4& getProjectionMatrix() const;

private:
	int mOldX;              // Position Mauszeiger
	int mOldY;
	int mMotionState;       // bewegen oder drehen
	float mTheta;           // aktueller theta-Winkel fuer Blickrichtung
	float mPhi;             // aktueller phi-Winkel fuer Blickrichtung
	float mSpeed;           // aktuelle Geschwindigkeit
	float mMotionStep;      // Mausempfindlichkeit
	float mThetaStep;
	float mPhiStep;
	glm::vec3 mPosition;    // aktuelle Kameraposition
	glm::vec3 mDirection;   // aktuelle Flug/Blickrichtung

	glm::mat4 mViewMatrix;
	glm::mat4 mProjectionMatrix;
};

#endif
