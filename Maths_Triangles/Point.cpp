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

float Point::getX()
{
	return this->x;
}

float Point::getY()
{
	return this->y;
}


bool Point::operator==(Point& p)
{
	return (this->x == p.getX() && this->y == p.getY());
}

bool Point::operator!=(Point& p)
{
	return !this->operator==(p);
}

Point::~Point()
{
}
