#ifndef PYRAMID_H
#define PYRAMID_H

#include "Triangle.h"


class Pyramid
{
public:
	Pyramid::Pyramid(unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4, std::shared_ptr<Triangle> t1, std::shared_ptr<Triangle> t2, std::shared_ptr<Triangle> t3, std::shared_ptr<Triangle> t4);
	~Pyramid();
	std::shared_ptr<Triangle> t1, t2, t3, t4;
	unsigned int p1, p2, p3, p4;
	bool removed;
	std::shared_ptr<Circle> circumSphere;
	void Pyramid::getPoints(std::vector<unsigned int>& points);
};
#endif
