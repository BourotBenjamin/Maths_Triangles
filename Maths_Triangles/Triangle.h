#include <vector>
#include <memory>
#include "Point.h"

class Triangle
{
public:
	Triangle(std::shared_ptr<Point> p0, std::shared_ptr<Point> p1, std::shared_ptr<Point> p2);
	void setPoints(std::shared_ptr<Point> p0, std::shared_ptr<Point> p1, std::shared_ptr<Point> p2);
	std::vector<std::shared_ptr<Point>> getPoints();
	void draw();
	void getVertices(GLfloat* vertices, int* index);
	~Triangle();
private:
	std::vector<std::shared_ptr<Point>> points;
};

