#include "Triangle.h"
class Object
{
public:
	Object();
	void addTriangle(std::shared_ptr<Triangle> triangle);
	void draw();
	void getVertices(GLfloat* vertices, int* index);
	int getVerticesSize();
	int getIndicesSize();
	~Object();
private:
	std::vector<std::shared_ptr<Triangle>> triangles;
};

