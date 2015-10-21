#include "Scene.h"


Scene::Scene()
{
	objects = std::vector<std::shared_ptr<Object>>();
}

unsigned short Scene::getPoints(std::vector<float>& vboCoords)
{
	unsigned short size = 0;
	auto currentObject = objects.begin();
	while (currentObject != objects.end())
	{
		size += (*currentObject)->getPoints(vboCoords);
		currentObject++;
	}
	return size;
}

void Scene::getJarvisEnveloppes(std::vector<float>& vboCoords, std::vector<unsigned short>& sizesEnveloppes)
{
	auto currentObject = objects.begin();
	while (currentObject != objects.end())
	{
		sizesEnveloppes.push_back((*currentObject)->getEnveloppeJarvis(vboCoords));
		currentObject++;
	}
}


void Scene::addObject(std::shared_ptr<Object>& object)
{
	objects.push_back(object);
}



Scene::~Scene()
{
}

