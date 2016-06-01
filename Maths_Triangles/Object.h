#include "Pyramid.h"
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

struct Edge
{
	unsigned int v1, v2;
	Edge(unsigned int v1, unsigned int v2) { this->v1 = v1; this->v2 = v2; }
};


class Object
{
public:
	Object();
	std::shared_ptr<Point> findNearestPoint(float x, float y, float z);
	void removePoint(std::shared_ptr<Point>& point);
	//void addTriangle(std::shared_ptr<Triangle>& triangle);
	void addPoint(std::shared_ptr<Point>& point);
	void Object::generatePoints(float x, float y, float z, int maxDist, int nbPoints);
	std::shared_ptr<Point> getNearestPoint(float x, float y);
	unsigned short Object::getPoints(std::vector<float>& vboCoords);
	unsigned short getEnveloppeJarvis(std::vector<float>& vboCoords);
	unsigned short getEnveloppeGrahamScan(std::vector<float>& vboCoords);
	//std::shared_ptr<Triangle> addUsedEdgesToVector(std::vector<std::shared_ptr<UsedEdge>>& usedEdges, unsigned int indice1, unsigned int indice2, unsigned int indice3);
	unsigned short Object::simpleTriangulation3D(std::vector<float>& vboCoords, std::vector<unsigned int>& eboIndices, bool flipping, bool voronoi, unsigned int firstIndex);
	//unsigned short Object::simpleTriangulation(std::vector<float>& vboCoords, std::vector<unsigned int>& eboIndices, bool flipping, bool voronoi, unsigned int firstIndex);
	//void flipping(std::vector<std::shared_ptr<UsedEdge>>& usedEdges);
	std::shared_ptr<UsedEdge> Object::findEdgeInUsedEdges(std::vector<std::shared_ptr<UsedEdge>>& usedEdges, unsigned int indice1, unsigned int indice2);
	std::shared_ptr<Triangle> findUsedTriangle(std::vector<std::shared_ptr<Triangle>>& usedTriangles, unsigned int p1, unsigned int p2, unsigned int p3);
	//std::shared_ptr<UsedEdge> Object::addEdge(std::vector<std::shared_ptr<UsedEdge>>& usedEdges, unsigned int indice1, unsigned int indice2, unsigned int indice3, std::shared_ptr<Triangle> triangle);
	void createPyramidsAndAddUsedTriangles(std::vector<std::shared_ptr<Triangle>>& usedTriangles, unsigned int common1, unsigned int common2, unsigned int common3, unsigned int alone1, unsigned int alone2);
	void Object::createNewPyramids(std::vector<std::shared_ptr<Triangle>>& usedTriangles, std::shared_ptr<Pyramid> p1, std::shared_ptr<Pyramid> p2);
	void Object::flipping3D();
	std::shared_ptr<Circle> Object::getCircumsphere(std::shared_ptr<Pyramid> p);
	Circle Object::circumCircle(std::shared_ptr<Point>& a, std::shared_ptr<Point>& b, std::shared_ptr<Point>& c);
	bool Object::containsPyramid(std::shared_ptr<Circle> circle, std::shared_ptr<Pyramid> pyramid);
	float Object::calcAngle3D(std::shared_ptr<Triangle> triangle, std::shared_ptr<Point> D);
	void addPyramidToTriangleUsed(std::shared_ptr<Triangle> triangle, unsigned int pointNotOnTriangle, std::shared_ptr<Pyramid> pyramid);
	unsigned short Object::delaunayBowyerWatson(std::vector<float>& vboCoords, std::vector<unsigned int>& eboIndices, bool voronoi, unsigned int firstIndex);
	~Object();
private:
	std::vector<std::shared_ptr<Triangle>> usedTrianglesOrig;
	std::vector<std::shared_ptr<Pyramid>> pyramids;
	std::vector<std::shared_ptr<Point>> points;
	float xBarycentre;
	float yBarycentre;
	float zBarycentre;
};
bool cmpPointsAbsAndOrd(const std::shared_ptr<Point>& a, const std::shared_ptr<Point>& b);
bool cmpPointsAngleFromBarycentre(const std::shared_ptr<Point>& a, const std::shared_ptr<Point>& b);
float calcAngle(std::shared_ptr<Point> A, std::shared_ptr<Point> B, std::shared_ptr<Point> C);

