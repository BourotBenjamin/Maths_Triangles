#include "Point.h"
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>


Point::Point(float x, float y)
{
	this->x = x; 
	this->y = y;
}


void Point::draw()
{
	glVertex2d(this->x, this->y);
}


void Point::getVertices(GLfloat* vertices, int* index)
{
	vertices[*index] = this->x;
	vertices[(*index) + 1] = this->y;
	vertices[(*index) + 2] = 0.0f;
	(*index) = (*index) + 3;
}

Point::~Point()
{
}
