#include <cmath>
#include <vector>
#include <memory>

struct VoronoiEdge
{
	unsigned int v1X, v1Y, v2X, v2Y;
};

class Point
{
public:
	Point(float x, float y);
	void calcAngleFromBary(float xBarycentre, float yBarycentre);
	const float getX() const;
	const float getY() const;
	const float Point::getAngleFromBary() const;
	bool operator==(Point& p);
	bool operator!=(Point& p);
	void addVoronoiEdgeToRegion(std::shared_ptr<VoronoiEdge> edge);
	~Point();
private:
	float x;
	float y;
	float angleFromBary;
	std::vector<std::shared_ptr<VoronoiEdge>> region;
};

