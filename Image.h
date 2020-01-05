#ifndef IMAGE_H
#define IMAGE_H

#include <glm/glm.hpp>

class Image
{
public:
	Image(int width, int height);
	~Image();

	void putPixel(int x, int y, glm::vec3 color);
	void putPixel(glm::vec2 point, glm::vec3 color);
	void drawLine(glm::vec2 startPoint, glm::vec2 endPoint, glm::vec3 color);
	void clear(glm::vec3 color);
	void copyToFramebuffer();

	float* getData();
	int getWidth();
	int getHeight();

private:
	float* mPixels;
	int mWidth;
	int mHeight;
};

#endif
