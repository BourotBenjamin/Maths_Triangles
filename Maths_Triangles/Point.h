#include "GL/glew.h"

class Point
{
public:
	Point(float x, float y);
	void draw();
	void getVertices(GLfloat* vertices, int* index);
	~Point();
private:
	float x;
	float y;
};

