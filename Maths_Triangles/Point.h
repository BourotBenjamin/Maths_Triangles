#include <cmath>

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
	~Point();
private:
	float x;
	float y;
	float angleFromBary;
};

