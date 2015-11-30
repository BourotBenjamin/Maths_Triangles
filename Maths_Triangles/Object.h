#include "Triangle.h"
#include <algorithm>


struct UsedEdge
{
	unsigned int v1;
	unsigned int v2;
	bool side;
};
struct Circle
{
	float x, y, r;
};

class Object
{
public:
	Object();
	void addTriangle(std::shared_ptr<Triangle>& triangle);
	void addPoint(std::shared_ptr<Point>& point);
	std::shared_ptr<Point> getNearestPoint(float x, float y);
	unsigned short Object::getPoints(std::vector<float>& vboCoords);
	unsigned short getEnveloppeJarvis(std::vector<float>& vboCoords);
	unsigned short getEnveloppeGrahamScan(std::vector<float>& vboCoords);
	void addUsedEdgesToVector(std::vector<UsedEdge>& usedEdges, unsigned int indice1, unsigned int indice2, unsigned int indice3, std::vector<unsigned int>& eboIndices);
	unsigned short Object::simpleTriangulation(std::vector<float>& vboCoords, std::vector<unsigned int>& eboIndices, bool flipping);
	void Object::flipping();
	~Object();
private:
	std::vector<std::shared_ptr<Triangle>> triangles;
	std::vector<std::shared_ptr<Point>> points;
	float xBarycentre;
	float yBarycentre;
};
bool cmpPointsAbsAndOrd(const std::shared_ptr<Point>& a, const std::shared_ptr<Point>& b);
bool cmpPointsAngleFromBarycentre(const std::shared_ptr<Point>& a, const std::shared_ptr<Point>& b);
float calcAngle(std::shared_ptr<Point> A, std::shared_ptr<Point> B, std::shared_ptr<Point> C);

