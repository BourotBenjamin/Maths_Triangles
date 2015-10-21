#include "Triangle.h"
class Object
{
public:
	Object();
	void addTriangle(std::shared_ptr<Triangle>& triangle);
	void addPoint(std::shared_ptr<Point>& point);
	std::shared_ptr<Point> getNearestPoint(float x, float y);
	unsigned short Object::getPoints(std::vector<float>& vboCoords);
	unsigned short getEnveloppeJarvis(std::vector<float>& vboCoords);
	~Object();
private:
	std::vector<std::shared_ptr<Triangle>> triangles;
	std::vector<std::shared_ptr<Point>> points;
};

