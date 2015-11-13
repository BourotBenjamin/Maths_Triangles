#include "Triangle.h"
#include <algorithm>
class Object
{
public:
	Object();
	void addTriangle(std::shared_ptr<Triangle>& triangle);
	void addPoint(std::shared_ptr<Point>& point);
	std::shared_ptr<Point> getNearestPoint(float x, float y);
	unsigned short Object::getPoints(std::vector<float>& vboCoords);
	unsigned short getEnveloppeJarvis(std::vector<float>& vboCoords);
	unsigned short getEnveloppeGrahamScan(std::vector<float>& vboCoords);
	~Object();
private:
	std::vector<std::shared_ptr<Triangle>> triangles;
	std::vector<std::shared_ptr<Point>> points;
	float xBarycentre;
	float yBarycentre;
};
bool cmpPointsAngleFromBarycentre(const std::shared_ptr<Point>& a, const std::shared_ptr<Point>& b);

