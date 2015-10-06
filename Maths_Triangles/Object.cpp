#include "Object.h"


Object::Object()
{
	triangles = std::vector<std::shared_ptr<Triangle>>();
}

void Object::addTriangle(std::shared_ptr<Triangle> triangle)
{
	triangles.push_back(std::move(triangle));
}

void Object::draw()
{
	auto currentTriangle = triangles.begin();
	while (currentTriangle != triangles.end())
	{
		(*currentTriangle)->draw();
		currentTriangle++;
	}
}

int Object::getVerticesSize()
{
	return triangles.size() * 9;
}

int Object::getIndicesSize()
{
	return triangles.size() * 3;
}

void Object::getVertices(GLfloat* vertices, int* index)
{
	auto currentTriangle = triangles.begin();
	while (currentTriangle != triangles.end())
	{
		(*currentTriangle)->getVertices(vertices, index);
		currentTriangle++;
	}
}

Object::~Object()
{
}
