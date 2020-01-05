#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject()
{
	mVAO = 0;
}

// begin a Vertex Array Object: store drawMode, generate VAO ID, and bind VAO
void VertexArrayObject::begin(unsigned int drawMode)
{
	if (mPositions.size() > 0)
		mPositions.clear();
	if (mNormals.size() > 0)
		mNormals.clear();
	if (mColors.size() > 0)
		mColors.clear();
	if (mTexCoords.size() > 0)
		mTexCoords.clear();
	if (mIndices.size() > 0)
		mIndices.clear();

	mDrawMode = drawMode;

	if (mVAO == 0)
		glGenVertexArrays(1, &mVAO);

	glBindVertexArray(mVAO);

}

// add 2D point to positions array (as 4D vector, fill components 3 and 4 with default values)
void VertexArrayObject::addVertex2f(float x, float y)
{
	mPositions.push_back(x);
	mPositions.push_back(y);
	mPositions.push_back(0.0f);
	mPositions.push_back(1.0f);
}

// add 3D point to positions array (as 4D vector, fill component 4 with default value)
void VertexArrayObject::addVertex3f(float x, float y, float z)
{
	mPositions.push_back(x);
	mPositions.push_back(y);
	mPositions.push_back(z);
	mPositions.push_back(1.0f);
}

// add color to color array (as 4D vector, fill alpha component with default value)
void VertexArrayObject::addColor3f(float r, float g, float b)
{
	mColors.push_back(r);
	mColors.push_back(g);
	mColors.push_back(b);
	mColors.push_back(1.0f);
}

// add normal to normal array (as 4D vector, set forth component to zero)
void VertexArrayObject::addNormal3f(float x, float y, float z)
{
	mNormals.push_back(x);
	mNormals.push_back(y);
	mNormals.push_back(z);
	mNormals.push_back(0.0f);
}

// add texcoord to texcoord array (as 4D vector, set component 3 and 4 to zero)
void VertexArrayObject::addTexCoord2f(float s, float t)
{
	mTexCoords.push_back(s);
	mTexCoords.push_back(t);
	mTexCoords.push_back(0.0f);
	mTexCoords.push_back(0.0f);
}

// add index to index array 
void VertexArrayObject::addIndex1ui(unsigned int i)
{
	mIndices.push_back(i);
}

// end a Vertex Array Object: check if attribute arrays contain values, then generate VBOs and set attrib pointers
void VertexArrayObject::end()
{
	glGenBuffers(1, &mPositionBufferHandle); 
	glBindBuffer(GL_ARRAY_BUFFER, mPositionBufferHandle);
	//glBufferData(GL_ARRAY_BUFFER, mPositions.size() * sizeof(float), mPositions.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, mPositions.size() * sizeof(float), &mPositions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	if (mColors.size() > 0) {
		glGenBuffers(1, &mColorBufferHandle); 
		glBindBuffer(GL_ARRAY_BUFFER, mColorBufferHandle);
		// glBufferData(GL_ARRAY_BUFFER, mColors.size() * sizeof(float), mColors.data(), GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, mColors.size() * sizeof(float), &mColors[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);
	}

	if (mNormals.size() > 0) {
		glGenBuffers(1, &mNormalBufferHandle); 
		glBindBuffer(GL_ARRAY_BUFFER, mNormalBufferHandle);
		//glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(float), mNormals.data(), GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(float), &mNormals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);
	}

	if (mTexCoords.size() > 0) {
		glGenBuffers(1, &mTexCoordBufferHandle); 
		glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBufferHandle);
		// glBufferData(GL_ARRAY_BUFFER, mTexCoords.size() * sizeof(float), mTexCoords.data(), GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, mTexCoords.size() * sizeof(float), &mTexCoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);
	}

	if (mIndices.size() > 0) {
		glGenBuffers(1, &mIndexBufferHandle); 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), mIndices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

// draw Function: check if VAO contains indices, then call glDrawArrays or glDrawElements
void VertexArrayObject::draw()
{
	if (mIndices.size() == 0) {

		glBindVertexArray(mVAO);
		glDrawArrays(mDrawMode, 0, mPositions.size() / 4);
		glBindVertexArray(0);
	}
	else {

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle);
		glBindVertexArray(mVAO);
		glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}
}


