#include "Object.h"
class Scene
{
public:
	Scene();
	void addObject(std::shared_ptr<Object>& object);
    std::shared_ptr<Point> getNearestPoint(float x, float y);
	unsigned short Scene::getPoints(std::vector<float>& vboCoords);
	void getJarvisEnveloppes(std::vector<float>& vboCoords, std::vector<unsigned short>& sizesEnveloppes);
	~Scene();
private:
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::shared_ptr<Point>> points;
};

