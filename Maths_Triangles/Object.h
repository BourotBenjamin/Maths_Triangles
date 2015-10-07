#include "Triangle.h"
class Object
{
public:
	Object();
	void addTriangle(std::unique_ptr<Triangle> triangle);
	void draw();
	void getVertices(GLfloat* vertices, int* index);
	int getVerticesSize();
	int getIndicesSize();
	~Object();
private:
	std::vector<std::unique_ptr<Triangle>> triangles;
};

