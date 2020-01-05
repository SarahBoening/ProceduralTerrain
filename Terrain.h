#ifndef _TERRAIN_H
#define _TERRAIN_H

#include "VertexArrayObject.h"
#include <vector>

class Terrain : public VertexArrayObject {

public:
    Terrain(int resolution, float interval);
    ~Terrain();
    
    void generate(int x1, int z1, int x2, int z2, float interval);

    void setHeight(int x, int z, float height);
    float getHeight(int x, int z) const;
	int calcIndex(int x, int z);

	void setVAOPositions();

private:     
    int mResolution;         // number of points in x and z direction
	std::vector<float> mHeight;
};


#endif

