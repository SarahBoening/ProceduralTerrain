#ifndef _BALLOON_H
#define _BALLOON_H

#include "VertexArrayObject.h"
#include <glm/glm.hpp>

using namespace std;

class Image;

class Balloon : public VertexArrayObject  {
public:
    explicit Balloon(float radius, int resolution);
    virtual ~Balloon() = default;
};

#endif

