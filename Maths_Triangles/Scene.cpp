#include "Scene.h"


Scene::Scene()
{
	objects = std::vector<std::unique_ptr<Object>>();
}

void Scene::draw()
{
	auto currentObject = objects.begin();
	while (currentObject != objects.end())
	{
		(*currentObject)->draw();
		currentObject++;
	}
}

int Scene::getVerticesSize()
{
	int size = 0;
	auto currentObject = objects.begin();
	while (currentObject != objects.end())
	{
		size += (*currentObject)->getVerticesSize();
		currentObject++;
	}
	return size;
}

int Scene::getIndicesSize()
{
	int size = 0;
	auto currentObject = objects.begin();
	while (currentObject != objects.end())
	{
		size += (*currentObject)->getIndicesSize();
		currentObject++;
	}
	return size;
}

void Scene::getVertices(GLfloat* vertices, int* index)
{
	auto currentObject = objects.begin();
	while (currentObject != objects.end())
	{
		(*currentObject)->getVertices(vertices, index);
		currentObject++;
	}
}



void Scene::addObject(std::unique_ptr<Object>& object)
{
	objects.push_back(std::move(object));
}

Scene::~Scene()
{
}
