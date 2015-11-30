#include "Object.h"
# define M_PI           3.14159265358979323846


Object::Object()
{
	triangles = std::vector<std::shared_ptr<Triangle>>();
}

void Object::addTriangle(std::shared_ptr<Triangle>& triangle)
{
	triangles.push_back(triangle);
}

void Object::addPoint(std::shared_ptr<Point>& p)
{
	points.push_back(p);
}

std::shared_ptr<Point> Object::getNearestPoint(float x, float y)
{
	auto pointsPtr = points.begin();
	float min = FLT_MAX;
	std::shared_ptr<Point> bestPoint = nullptr;
	while (pointsPtr != points.end())
	{
		std::shared_ptr<Point> curPoint = (*pointsPtr);
		float dist = sqrt(((curPoint->getX()) - x) * ((curPoint)->getX() - x) + ((curPoint)->getY() - y) * ((curPoint)->getY() - y));
		if (dist < min)
		{
			bestPoint = curPoint;
		}
		pointsPtr++;
	}
	return bestPoint;
}


unsigned short Object::getPoints(std::vector<float>& vboCoords)
{
	unsigned short i = 0;
	auto ptr = points.begin();
	while (ptr != points.end())
	{

		vboCoords.push_back((*ptr)->getX());
		vboCoords.push_back((*ptr)->getY());
		vboCoords.push_back(1.0f);
		i++;
		ptr++;
	}
	return i;
}


unsigned short Object::getEnveloppeJarvis(std::vector<float>& vboCoords)
{
	if (points.size() > 2)
	{
		std::vector<std::shared_ptr<Point>> enveloppe;
		std::shared_ptr<Point> first = points.at(0);
		auto ptr = points.begin();
		while (ptr != points.end())
		{
			if (first)
			{
				if (first->getX() > (*ptr)->getX())
					first = (*ptr);
				else if (first->getX() == (*ptr)->getX() && first->getY() > (*ptr)->getY())
					first = (*ptr);
			}
			ptr++;
		}
		enveloppe.push_back(first);
		vboCoords.push_back(first->getX());
		vboCoords.push_back(first->getY());
		vboCoords.push_back(1.0f);
		bool isFirst = true;
		std::shared_ptr<Point> lastP = std::shared_ptr<Point>(new Point(first->getX(), 0.0f)), current = first, best = nullptr;
		float angle = 0.0f, bestAngle = 0.0f;
		while (isFirst || *current != *first)
		{
			bestAngle = 0.0f;
			isFirst = false;
			float v1X = current->getX() - lastP->getX(), v1Y = current->getY() - lastP->getY(), v2X = 0.0f, v2Y = 0.0f;
			float l1 = sqrt(v1X*v1X + v1Y*v1Y), l2 = 0.0;
			auto ptr = points.begin();
			while (ptr != points.end())
			{
				if (*(*ptr) != *current)
				{
					v2X = current->getX() - (*ptr)->getX(), v2Y = current->getY() - (*ptr)->getY();
					l2 = sqrt(v2X*v2X + v2Y*v2Y);
					angle = acos((v1X*v2X + v1Y*v2Y) / (l1*l2));	// calc angle between lastP->current current->ptr
					if (angle > bestAngle)
					{
						bestAngle = angle;
						best = (*ptr);
					}
				}
				ptr++;
			}
			lastP = current;
			current = best;
			enveloppe.push_back(best);
			vboCoords.push_back(best->getX());
			vboCoords.push_back(best->getY());
			vboCoords.push_back(1.0f);
		}
		return enveloppe.size();
	}
	return 0;
}

unsigned short Object::getEnveloppeGrahamScan(std::vector<float>& vboCoords)
{
	if (points.size() > 2)
	{
		std::vector<std::shared_ptr<Point>> enveloppe;
		xBarycentre = 0.0f, yBarycentre = 0.0f;
		auto ptr = points.begin();
		int pointsSize = 0;
		while (ptr != points.end())
		{
			pointsSize++;
			xBarycentre += (*ptr)->getX();
			yBarycentre += (*ptr)->getY();
			enveloppe.push_back((*ptr));
			ptr++;
		}
		xBarycentre /= pointsSize;
		yBarycentre /= pointsSize;
		ptr = enveloppe.begin();
		while (ptr != enveloppe.end())
		{
			(*ptr)->calcAngleFromBary(xBarycentre, yBarycentre);
			ptr++;
		}

		std::sort(enveloppe.begin(), enveloppe.end(), cmpPointsAngleFromBarycentre);

		auto first = enveloppe.begin(), current = enveloppe.begin() + 1;
		bool continueLoop = false;
		while (continueLoop || *first != *current)
		{
			float angle = 0.0f;
			if (*current == enveloppe.front())
				angle = calcAngle(enveloppe.back(), (*current), (*(current + 1)));
			else if (*current == enveloppe.back())
				angle = calcAngle((*(current - 1)), (*current), enveloppe.front());
			else
				angle = calcAngle((*(current - 1)), (*current), (*(current + 1)));
			if (angle < 0.001f)
			{
				continueLoop = false;
				++current;
			}
			else
			{
				if (current == enveloppe.begin())
				{
					enveloppe.erase(current);
					first = (enveloppe.end()) - 1;
				}
				else
				{
					first = current - 1;
					enveloppe.erase(current);
				}
				continueLoop = true;
				current = first;
			}
			if (current == enveloppe.end())
				current = enveloppe.begin();
		}
		ptr = enveloppe.begin();
		while (ptr != enveloppe.end())
		{
			vboCoords.push_back((*ptr)->getX());
			vboCoords.push_back((*ptr)->getY());
			vboCoords.push_back(1.0f);
			ptr++;
		}
		ptr = enveloppe.begin();
		vboCoords.push_back((*ptr)->getX());
		vboCoords.push_back((*ptr)->getY());
		vboCoords.push_back(1.0f);
		return enveloppe.size() + 1;
	}
	return 0;
}

enum StatesimpleTriangulation{
	INIT_FRIST_POINT,
	INIT_SECOND_POINT
};

float calcAngle(std::shared_ptr<Point> A, std::shared_ptr<Point> B, std::shared_ptr<Point> C)
{
	float ABx = B->getX() - A->getX();
	float ABy = B->getY() - A->getY();
	float BCx = C->getX() - B->getX();
	float BCy = C->getY() - B->getY();
	float angle = (atan2(ABy, ABx) - atan2(BCy, BCx));
	while (angle > M_PI)
		angle -= M_PI * 2;
	while (angle < -M_PI)
		angle += M_PI * 2;
	return angle;
}

void Object::addUsedEdgesToVector(std::vector<UsedEdge>& usedEdges, unsigned int indice1, unsigned int indice2, unsigned int indice3, std::vector<unsigned int>& eboIndices)
{
	UsedEdge e1, e2, e3;
	e1.v1 = indice1;
	e1.v2 = indice2;
	e1.side = ((calcAngle(points.at(indice1), points.at(indice2), points.at(indice3))) < 0);
	usedEdges.push_back(e1);
	e2.v1 = indice2;
	e2.v2 = indice3;
	e2.side = ((calcAngle(points.at(indice2), points.at(indice3), points.at(indice1))) < 0);
	usedEdges.push_back(e2);
	e3.v1 = indice3;
	e3.v2 = indice1;
	e3.side = ((calcAngle(points.at(indice3), points.at(indice1), points.at(indice2))) < 0);
	usedEdges.push_back(e3);
	triangles.push_back(std::shared_ptr<Triangle>(new Triangle(indice1, indice2, indice3)));
}

Circle circumsedCircleconst(std::shared_ptr<Point>& a, std::shared_ptr<Point>& b, std::shared_ptr<Point>& c)
{
	Circle circle;
	float aX = a->getX(), aY = a->getY(), bX = b->getX(), bY = b->getY(), cX = c->getX(), cY = c->getY();
	float D = 2 * (aX  * (bY - cY) + bX * (cY - aY) + cX * (aY - bY));
	circle.x = (((aX * aX) + (aY * aY)) * (bY - cY) + ((bX * bX) + (bY * bY)) * (cY - aY) + ((cX * cX) + (cY * cY)) * (aY - bY)) / D;
	circle.y = (((aX * aX) + (aY * aY)) * (cX - bX) + ((bX * bX) + (bY * bY)) * (aX - cX) + ((cX * cX) + (cY * cY)) * (bX - aX)) / D;
	circle.r = sqrt((bX - circle.x) * (bX - circle.x) + (bY - circle.y) * (bY - circle.y));
	return circle;
}



void Object::flipping()
{
	std::vector<std::shared_ptr<Triangle>> newTrList;
	for each (auto triangle in triangles)
	{
		unsigned int i1 = triangle->getIndice1(), i2 = triangle->getIndice2(), i3 = triangle->getIndice3();
		Circle c = circumsedCircleconst(points.at(i1), points.at(i2), points.at(i3));
		int i = 0;
		for each (auto point in points)
		{
			if (i != i1 && i != i2 && i != i3 && ( sqrt((c.x - point->getX()) * (c.x - point->getX()) + (c.y - point->getY()) * (c.y - point->getY())) <= c.r ) )
			{
				for each (auto triangle2 in triangles)
				{
					unsigned int iT1 = triangle2->getIndice1(), iT2 = triangle2->getIndice2(), iT3 = triangle2->getIndice3();
					unsigned int p1, p2, p3;
					bool found = false;
					if (iT1 == i || iT2 == i || iT3 == i)
					{
						if (iT1 == i1 || iT2 == i1 || iT3 == i1)
						{
							if (iT1 == i2 || iT2 == i2 || iT3 == i2)
							{
								found = true;
								p1 = i1;
								p2 = i2;
								p3 = i3;
							}
							else if (iT1 == i3 || iT2 == i3 || iT3 == i3)
							{
								found = true;
								p1 = i1;
								p2 = i3;
								p3 = i2;
							}
						}
						else if (iT1 == i2 || iT2 == i2 || iT3 == i2)
						{
							if (iT1 == i3 || iT2 == i3 || iT3 == i3)
							{
								found = true;
								p1 = i2;
								p2 = i3;
								p3 = i1;
							}
						}
					}
					if (found && !triangle->removed && !triangle2->removed)
					{
						triangle->removed = true;
						triangle2->removed = true;
						newTrList.push_back(std::shared_ptr<Triangle>(new Triangle(p3, p1, i)));
						newTrList.push_back(std::shared_ptr<Triangle>(new Triangle(p3, p2, i)));
					}
				}
			}
			i++;
		}
	}
	for each (auto t in triangles)
	{
		if (!t->removed)
			newTrList.push_back(t);
	}
	triangles.clear();
	for each (auto t in newTrList)
	{
		triangles.push_back(t);
	}
}

unsigned short Object::simpleTriangulation(std::vector<float>& vboCoords, std::vector<unsigned int>& eboIndices, bool flipping)
{
	triangles.clear();
	std::sort(points.begin(), points.end(), cmpPointsAbsAndOrd);
	
	StatesimpleTriangulation state = INIT_FRIST_POINT;
	int currentIndice = 0;
	std::vector<UsedEdge> usedEdges, oldUsedEdges;
	bool found = false;
	for each (auto point in points)
	{
		vboCoords.push_back(point->getX());
		vboCoords.push_back(point->getY());
		vboCoords.push_back(1.0f);
		if (currentIndice > 2)
		{
			oldUsedEdges.clear();
			for each (auto edge in usedEdges)
			{
				oldUsedEdges.push_back(edge);
			}
			for each (auto edge in oldUsedEdges)
			{
				found = false;
				if (calcAngle(points.at(edge.v1), points.at(edge.v2), point) < 0 != edge.side)
				{
					for each (auto edge2 in oldUsedEdges)
					{
						if (edge2.v1 == edge.v1 && edge2.v2 == edge.v2 && calcAngle(points.at(edge2.v1), points.at(edge2.v2), point) < 0 == edge2.side)
						{
							found = true;
						}
						if (edge2.v1 == edge.v2 && edge2.v2 == edge.v1 &&  edge.side == edge2.side)
						{
							found = true;
						}
					}
				}
				else
					found = true;
				if (!found)
					addUsedEdgesToVector(usedEdges, edge.v1, edge.v2, currentIndice, eboIndices);
			}
		}
		else if (currentIndice == 2)
		{
			addUsedEdgesToVector(usedEdges, 0, 1, 2, eboIndices);
		}
		currentIndice++;
	}
	if (flipping)
		this->flipping();
	for each (auto triangle in triangles)
	{
		eboIndices.push_back(triangle->getIndice1());
		eboIndices.push_back(triangle->getIndice2());
		eboIndices.push_back(triangle->getIndice3());
	}
	return eboIndices.size();
}

Object::~Object()
{
}

bool cmpPointsAbsAndOrd(const std::shared_ptr<Point>& a, const std::shared_ptr<Point>& b)
{
	return (a->getX() < b->getX() || (a->getX() == b->getX() && a->getY() < b->getY()));
}

bool cmpPointsAngleFromBarycentre(const std::shared_ptr<Point>& a, const std::shared_ptr<Point>& b)
{
	return (a->getAngleFromBary() < b->getAngleFromBary());
}