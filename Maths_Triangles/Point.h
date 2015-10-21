#include "GL/glew.h"

class Point
{
public:
	Point(float x, float y);
	void draw();
	void getVertices(GLfloat* vertices, int* index);
	float getX();
	float getY();
	bool operator==(Point& p);
	bool operator!=(Point& p);
	~Point();
private:
	float x;
	float y;
};

