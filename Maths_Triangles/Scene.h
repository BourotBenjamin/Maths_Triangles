#include "Object.h"
class Scene
{
public:
	Scene();
	void addObject(std::unique_ptr<Object> object);
	void draw();
	void getVertices(GLfloat* vertices, int* index);
	int getVerticesSize();
	int getIndicesSize();
	~Scene();
private:
	std::vector<std::unique_ptr<Object>> objects;
};

