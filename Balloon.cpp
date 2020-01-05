#include "Balloon.h"

using namespace glm;

#ifndef PI
#define PI 3.141592f
#endif

int calcIndex(int res, int i, int j)
{
	return i*(res - 1) + j;
}

Balloon::Balloon(float radius, int resolution)
{
	float deltaTheta = PI / (float)(resolution-1);
	float deltaPhi = 2.0f * PI / (float)(resolution-1);

	float theta = 0.0f;
	float phi = 0.0f;

	float deltaS = 1.0f / (float)(resolution-1);
	float deltaT = 1.0f / (float)(resolution-1);

	float s = 0.0f;
	float t = 0.0f;

	begin(GL_TRIANGLES);

    for (int i = 0 ; i < resolution ; i++) {
                      
        for (int j = 0 ; j < resolution ; j++) {

			float nx = sin(theta) * cos(phi);
			float ny = cos(theta);
			float nz = sin(theta) * sin(phi);
			
			addNormal3f( nx, ny, nz );

			float x = radius * nx;
			float y = radius * ny;
			float z = radius * nz;

			addVertex3f(x, y, z);

			addTexCoord2f(s, t);

			phi += deltaPhi;
			s += deltaS;
		}
		theta += deltaTheta;
		phi = 0.0f;

		t += deltaT;
		s = 0.0f;
    }

    for (int i = 0 ; i < resolution-1 ; i++) 
	{
        for (int j = 0 ; j < resolution ; j++) 
		{
			addIndex1ui( calcIndex(resolution, i, j) );
			addIndex1ui( calcIndex(resolution, i+1, j) );
			addIndex1ui( calcIndex(resolution, i+1, j+1) );

			addIndex1ui( calcIndex(resolution, i, j) );
			addIndex1ui( calcIndex(resolution, i+1, j+1) );
			addIndex1ui( calcIndex(resolution, i, j+1) );
		}
    }
	
	end();
}








