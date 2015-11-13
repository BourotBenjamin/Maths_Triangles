#include "Point.h"


Point::Point(float x, float y)
{
	this->x = x; 
	this->y = y;
}

void Point::calcAngleFromBary(float xBarycentre, float yBarycentre)
{
	float vX = this->x - xBarycentre, vY = this->y - yBarycentre;
	this->angleFromBary = atan2(vY, vX) - atan2(0.0, 1.0f);
}

const float Point::getAngleFromBary() const
{
	return this->angleFromBary;
}

const float Point::getX() const
{
	return this->x;
}

const float Point::getY() const
{
	return this->y;
}


bool Point::operator==(Point& p)
{
	return (this->x == p.getX() && this->y == p.getY());
}

bool Point::operator!=(Point& p)
{
	return !this->operator==(p);
}

Point::~Point()
{
}
