#include "Triangle.h"


Triangle::Triangle(std::shared_ptr<Point> p0, std::shared_ptr<Point> p1, std::shared_ptr<Point> p2)
{ 
	points = std::vector<std::shared_ptr<Point>>();
	points.push_back(std::move(p0));
	points.push_back(std::move(p1));
	points.push_back(std::move(p2));
}

void Triangle::setPoints(std::shared_ptr<Point> p0, std::shared_ptr<Point> p1, std::shared_ptr<Point> p2) 
{ 
	points.empty();
	points.push_back(std::move(p0));
	points.push_back(std::move(p1));
	points.push_back(std::move(p2));
}

void Triangle::draw()
{
	auto currentPoint = points.begin();
	while (currentPoint != points.end())
	{
		(*currentPoint)->draw();
		currentPoint++;
	}
}



void Triangle::getVertices(GLfloat* vertices, int* index)
{
	auto currentPoint = points.begin();
	while (currentPoint != points.end())
	{
		(*currentPoint)->getVertices(vertices, index);
		currentPoint++;
	}
}

std::vector<std::shared_ptr<Point>> Triangle::getPoints()
{
	return points; 
}

Triangle::~Triangle()
{
	points.empty();
}