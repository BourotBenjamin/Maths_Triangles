#include "Triangle.h"
#include <algorithm>
#include <iostream>
#include <chrono>
#include <ctime>


struct UsedEdge
{
	unsigned int v1, v2;
	std::shared_ptr<Triangle> t1 = nullptr, t2 = nullptr;
};


class Object
{
public:
	Object();
	std::shared_ptr<Point> findNearestPoint(float x, float y);
	void removePoint(std::shared_ptr<Point>& point);
	void addTriangle(std::shared_ptr<Triangle>& triangle);
	void addPoint(std::shared_ptr<Point>& point);
	std::shared_ptr<Point> getNearestPoint(float x, float y);
	unsigned short Object::getPoints(std::vector<float>& vboCoords);
	unsigned short getEnveloppeJarvis(std::vector<float>& vboCoords);
	unsigned short getEnveloppeGrahamScan(std::vector<float>& vboCoords);
	std::shared_ptr<Triangle> addUsedEdgesToVector(std::vector<std::shared_ptr<UsedEdge>>& usedEdges, unsigned int indice1, unsigned int indice2, unsigned int indice3);
	unsigned short Object::simpleTriangulation(std::vector<float>& vboCoords, std::vector<unsigned int>& eboIndices, bool flipping, bool voronoi, unsigned int firstIndex);
	void Object::flipping(std::vector<std::shared_ptr<UsedEdge>>& usedEdges);
	std::shared_ptr<UsedEdge> Object::findEdgeInUsedEdges(std::vector<std::shared_ptr<UsedEdge>>& usedEdges, unsigned int indice1, unsigned int indice2);
	std::shared_ptr<UsedEdge> Object::addEdge(std::vector<std::shared_ptr<UsedEdge>>& usedEdges, unsigned int indice1, unsigned int indice2, unsigned int indice3, std::shared_ptr<Triangle> triangle);
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

