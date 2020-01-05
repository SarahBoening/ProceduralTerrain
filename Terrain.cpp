#include "Terrain.h"

#include <glm/glm.hpp>
#include <stdlib.h>
#include <GL/freeglut.h>

using namespace glm;

float random(float interval)
{
	return(static_cast<float>(rand())/static_cast<float>(RAND_MAX))*2*interval-interval;
}

Terrain::Terrain(int resolution, float interval)
{
    mResolution = resolution;
    mHeight.resize(resolution*resolution);

    for (int i = 0 ; i < resolution*resolution ; i++) {
        mHeight[i] = 0;
    }

	// init and generate terrain here
   	setHeight(0, 0, random(interval));
  	setHeight(0, resolution-1, random(interval));
  	setHeight(resolution-1, resolution-1, random(interval));
  	setHeight(resolution-1, 0, random(interval));
  	
	generate(0, 0, resolution-1, resolution-1, interval);

	setVAOPositions();
}

Terrain::~Terrain()
{
}

// VAO aus Terrainpositionen erzeugen,
// der Rand des Terrains wird hier nicht mit aufgenommen, um die Normalenberechnung zu erleichtern
void Terrain::setVAOPositions()
{
	vec3 n;

	begin(GL_TRIANGLES);

    for (int z = 1 ; z < mResolution-1 ; z++) {
                      
        for (int x = 1 ; x < mResolution-1 ; x++) {

			// Normalenberechnung
			n.x = getHeight(x - 1, z) - getHeight(x + 1, z);
			n.y = 2.0f;
			n.z = getHeight(x, z - 1) - getHeight(x, z + 1);
			n = normalize(n);
			addNormal3f(n.x, n.y, n.z);

 			addVertex3f(static_cast<float>(x), getHeight(x, z), static_cast<float>(z));
		}
    }

	for (int z = 0 ; z < mResolution-3 ; z++) {
                      
        for (int x = 0 ; x < mResolution-3 ; x++) {

			addIndex1ui( calcIndex(x, z) );
			addIndex1ui( calcIndex(x+1, z) );
			addIndex1ui( calcIndex(x+1, z+1) );

			addIndex1ui( calcIndex(x, z) );
			addIndex1ui( calcIndex(x+1, z+1) );
			addIndex1ui( calcIndex(x, z+1) );

		}
    }

	end();
}

void Terrain::generate(int x1, int z1, int x2, int z2, float interval)
{
	// rekursive Terrain Generierung
	
    float edgeHeight, centerHeight;
	int midX = (x1 + x2) / 2;
	int midY = (z1 + z2) / 2;

	if ((x2 - x1 < 2) && (z2 - z1 < 2))
	    return;
     
    centerHeight = (getHeight(x1,z1) + getHeight(x1,z2) + 
                          getHeight(x2,z1) + getHeight(x2,z2)) / 4.0f + 
                          random(interval);
    setHeight(midX, midY, centerHeight);
    
    edgeHeight = (getHeight(x1,z1) + getHeight(x1,z2)) / 2.0f + random(interval);                                     
    setHeight(x1, midY, edgeHeight);
    edgeHeight = (getHeight(x2,z1) + getHeight(x2,z2)) / 2.0f + random(interval);                                     
    setHeight(x2, midY, edgeHeight);
    edgeHeight = (getHeight(x1,z1) + getHeight(x2,z1)) / 2.0f + random(interval);                                     
    setHeight(midX, z1, edgeHeight);
    edgeHeight = (getHeight(x1,z2) + getHeight(x2,z2)) / 2.0f + random(interval);                                     
    setHeight(midX, z2, edgeHeight);
    
	interval /= 2;
	
	generate(x1, z1, midX, midY, interval);
	generate(midX, z1, x2, midY, interval);
	generate(x1, midY, midX, z2, interval);
	generate(midX, midY, x2, z2, interval);
}

void Terrain::setHeight(int x, int z, float height)
{
    if (getHeight(x, z) == 0)
        mHeight[z*mResolution+x] = height;
}

float Terrain::getHeight(int x, int z) const
{
    return mHeight[z*mResolution+x];
}

int Terrain::calcIndex(int x, int z)
{
    return z*(mResolution-2)+x;  // angepasst fuer kleinere Aufloesung
}





