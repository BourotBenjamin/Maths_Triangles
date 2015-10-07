#include <vector>
#include <memory>
#include "Point.h"

class Triangle
{
public:
	Triangle(std::unique_ptr<Point>& p0, std::unique_ptr<Point>& p1, std::unique_ptr<Point>& p2);
	void setPoints(std::unique_ptr<Point>& p0, std::unique_ptr<Point>& p1, std::unique_ptr<Point>& p2);
	std::vector<std::unique_ptr<Point>>& getPoints();
	void draw();
	void getVertices(GLfloat* vertices, int* index);
	~Triangle();
private:
	std::vector<std::unique_ptr<Point>> points;
};

