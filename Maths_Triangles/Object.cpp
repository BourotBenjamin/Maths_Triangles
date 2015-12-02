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
		vboCoords.push_back(1.0f);
		vboCoords.push_back(1.0f);
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
		vboCoords.push_back(1.0f);
		vboCoords.push_back(1.0f);
		vboCoords.push_back(1.0f);
		std::shared_ptr<Point> lastP = std::shared_ptr<Point>(new Point(first->getX(), 0.0f)), current = first, best = nullptr;
		float angle = 0.0f, bestAngle = 0.0f;
		do
		{
			bestAngle = 0.0f;
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
			vboCoords.push_back(1.0f);
			vboCoords.push_back(1.0f);
			vboCoords.push_back(1.0f);
		} while (*current != *first);
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
			vboCoords.push_back(1.0f);
			vboCoords.push_back(1.0f);
			vboCoords.push_back(1.0f);
			ptr++;
		}
		ptr = enveloppe.begin();
		vboCoords.push_back((*ptr)->getX());
		vboCoords.push_back((*ptr)->getY());
		vboCoords.push_back(1.0f);
		vboCoords.push_back(1.0f);
		vboCoords.push_back(1.0f);
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

std::shared_ptr<UsedEdge> Object::findEdgeInUsedEdges(std::vector<std::shared_ptr<UsedEdge>>& usedEdges, unsigned int indice1, unsigned int indice2)
{
	for each (auto e in usedEdges)
	{
		if ((e->v1 == indice1 && e->v2 == indice2) || (e->v2 == indice1 && e->v1 == indice2))
			return e;
	}
	return nullptr;
}

std::shared_ptr<UsedEdge> Object::addEdge(std::vector<std::shared_ptr<UsedEdge>>& usedEdges, unsigned int indice1, unsigned int indice2, unsigned int indice3, std::shared_ptr<Triangle> triangle)
{
	std::shared_ptr<UsedEdge> e = std::shared_ptr<UsedEdge>(new UsedEdge());
	e->v1 = indice1;
	e->v2 = indice2;
	if (((calcAngle(points.at(indice1), points.at(indice2), points.at(indice3))) < 0))
		e->t1 = triangle;
	else
		e->t2 = triangle;
	usedEdges.push_back(e);
	return e;
}

void Object::addUsedEdgesToVector(std::vector<std::shared_ptr<UsedEdge>>& usedEdges, unsigned int indice1, unsigned int indice2, unsigned int indice3, bool addTriangle)
{
	std::shared_ptr<Triangle> triangle(new Triangle(indice1, indice2, indice3));
	std::shared_ptr<UsedEdge> e = findEdgeInUsedEdges(usedEdges, indice1, indice2);
	if (e == nullptr)
		e = addEdge(usedEdges, indice1, indice2, indice3, triangle);
	else if (((calcAngle(points.at(e->v1), points.at(e->v2), points.at(indice3))) < 0))
		e->t1 = triangle;
	else
		e->t2 = triangle;
	e = findEdgeInUsedEdges(usedEdges, indice2, indice3);
	if (e == nullptr)
		e = addEdge(usedEdges, indice2, indice3, indice1, triangle);
	else if (((calcAngle(points.at(e->v1), points.at(e->v2), points.at(indice1))) < 0))
		e->t1 = triangle;
	else
		e->t2 = triangle;
	e = findEdgeInUsedEdges(usedEdges, indice3, indice1);
	if (e == nullptr)
		e = addEdge(usedEdges, indice3, indice1, indice2, triangle);
	else if (((calcAngle(points.at(e->v1), points.at(e->v2), points.at(indice2))) < 0))
		e->t1 = triangle;
	else
		e->t2 = triangle;
	if (addTriangle)
		triangles.push_back(triangle);
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

Circle baryCentre(std::shared_ptr<Point>& a, std::shared_ptr<Point>& b, std::shared_ptr<Point>& c)
{
	Circle circle;
	float aX = a->getX(), aY = a->getY(), bX = b->getX(), bY = b->getY(), cX = c->getX(), cY = c->getY();
	circle.x = (aX + bX + cX) / 3;
	circle.y = (aY + bY + cY) / 3;
	circle.r = 0;
	return circle;
}



void Object::flipping(std::vector<std::shared_ptr<UsedEdge>>& usedEdges)
{
	bool changed = false;
	std::vector<std::shared_ptr<UsedEdge>> usedEdgesCopy;
	std::vector<std::shared_ptr<Triangle>> newTrList;
	for each (auto edge in usedEdges)
	{
		usedEdgesCopy.push_back(edge);
	}
	while (usedEdgesCopy.size() > 0)
	{
		std::shared_ptr<UsedEdge> edge = usedEdgesCopy.back();
		usedEdgesCopy.pop_back();
		if (edge->t1 != nullptr && edge->t2 != nullptr)
		{
			unsigned int i1 = edge->t1->getIndice1(), i2 = edge->t1->getIndice2(), i3 = edge->t1->getIndice3(), it1 = edge->t2->getIndice1(), it2 = edge->t2->getIndice2(), it3 = edge->t2->getIndice3();
			unsigned int p1, p2, p3, a = -1, b = -1, c = -1, d = -1;
			if (it1 == i1 || it1 == i2 || it1 == i3)
				a = it1;
			if (it2 == i1 || it2 == i2 || it2 == i3)
			{
				if (a == -1)
					a = it2;
				else
					b = it2;
			}
			if (it3 == i1 || it3 == i2 || it3 == i3)
			{
				if (a == -1)
					a = it3;
				else
					b = it3;
			}
			c = (it1 == a || it1 == b) ? ((it2 == a || it2 == b) ? it3 : it2) : it1;
			d = (i1 == a || i1 == b) ? ((i2 == a || i2 == b) ? i3 : i2) : i1;


			Circle circle = circumsedCircleconst(points.at(i1), points.at(i2), points.at(i3)), circle2 = circumsedCircleconst(points.at(it1), points.at(it2), points.at(it3));
			auto point = points.at(c), point2 = points.at(d);
			if (sqrt((circle.x - point->getX()) * (circle.x - point->getX()) + (circle.y - point->getY()) * (circle.y - point->getY())) <= circle.r 
				|| sqrt((circle2.x - point2->getX()) * (circle2.x - point2->getX()) + (circle2.y - point2->getY()) * (circle2.y - point2->getY())) <= circle2.r)
			{
				edge->t1->removed = true;
				edge->t2->removed = true;
				addUsedEdgesToVector(usedEdgesCopy, c, d, a, true);
				addUsedEdgesToVector(usedEdgesCopy, c, d, b, true);
			}
		}


	}
	for each (auto t in triangles)
	{
		if (!t->removed)
			newTrList.push_back(t);
	}
	triangles.clear();
	usedEdges.clear();
	for each (auto t in newTrList)
	{
		addUsedEdgesToVector(usedEdges, t->getIndice1(), t->getIndice2(), t->getIndice3(), true);
	}


	/*
	for each (auto triangle in triangles)
	{
		unsigned int i1 = triangle->getIndice1(), i2 = triangle->getIndice2(), i3 = triangle->getIndice3();
		Circle c = circumsedCircleconst(points.at(i1), points.at(i2), points.at(i3));
		int i = 0;
		for each (auto point in points)
		{
			if (i != i1 && i != i2 && i != i3 && (sqrt((c.x - point->getX()) * (c.x - point->getX()) + (c.y - point->getY()) * (c.y - point->getY())) <= c.r))
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
						changed = true;
					}
				}
			}
			i++;
		}
	}*/
	if (changed)
		printf("changed");
}

unsigned short Object::simpleTriangulation(std::vector<float>& vboCoords, std::vector<unsigned int>& eboIndices, bool flipping, bool voronoi)
{
	triangles.clear();
	std::sort(points.begin(), points.end(), cmpPointsAbsAndOrd);
	
	StatesimpleTriangulation state = INIT_FRIST_POINT;
	int currentIndice = 0;
	std::vector<std::shared_ptr<UsedEdge>> usedEdges, oldUsedEdges;
	bool found = false;
	for each (auto point in points)
	{
		vboCoords.push_back(point->getX());
		vboCoords.push_back(point->getY());
		vboCoords.push_back(1.0f);
		vboCoords.push_back(1.0f);
		vboCoords.push_back(1.0f);
		vboCoords.push_back(1.0f);
		if (currentIndice > 2)
		{
			oldUsedEdges.clear();
			for each (auto edge in usedEdges)
				oldUsedEdges.push_back(edge);
			for each (auto edge in oldUsedEdges)
			{
				if (edge->t1 != nullptr || edge->t2 != nullptr)
				{
					found = false;
					float angle = calcAngle(points.at(edge->v1), points.at(edge->v2), point);
					if ( angle < 0 && edge->t1 != nullptr)
						found = true;
					else if(angle >= 0 && edge->t2 != nullptr)
						found = true;
					if (!found)
						addUsedEdgesToVector(usedEdges, edge->v1, edge->v2, currentIndice, true);
				}
			}
		}
		else if (currentIndice == 2)
			addUsedEdgesToVector(usedEdges, 0, 1, 2, true);
		currentIndice++;
	}
	if (flipping)
		this->flipping(usedEdges);
	for each (auto triangle in triangles)
	{
		eboIndices.push_back(triangle->getIndice1());
		eboIndices.push_back(triangle->getIndice2());
		eboIndices.push_back(triangle->getIndice3());
	}
	int size = 0;
	if (voronoi)
	{
		for each (auto triangle in triangles)
		{
			Circle c = circumsedCircleconst(points.at(triangle->getIndice1()), points.at(triangle->getIndice2()), points.at(triangle->getIndice3()));
			triangle->setCircumCenter(c.x, c.y);
		}
		for each (auto edge in usedEdges)
		{
			if (edge->t1 != nullptr && edge->t2 != nullptr)
			{
				std::shared_ptr<VoronoiEdge> vEdge = std::shared_ptr<VoronoiEdge>(new VoronoiEdge());
				vEdge->v1X = edge->t1->getCircumCenterX();
				vEdge->v2X = edge->t2->getCircumCenterX();
				vEdge->v1Y = edge->t1->getCircumCenterY();
				vEdge->v2Y = edge->t2->getCircumCenterY();
				size += 2;
				vboCoords.push_back(edge->t1->getCircumCenterX());
				vboCoords.push_back(edge->t1->getCircumCenterY());
				vboCoords.push_back(1.0f);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(edge->t2->getCircumCenterX());
				vboCoords.push_back(edge->t2->getCircumCenterY());
				vboCoords.push_back(1.0f);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(0.0f);
				points.at(edge->v1)->addVoronoiEdgeToRegion(vEdge);
				points.at(edge->v2)->addVoronoiEdgeToRegion(vEdge);
			}
			else if (edge->t1 != nullptr)
			{
				size += 2;
				vboCoords.push_back(edge->t1->getCircumCenterX());
				vboCoords.push_back(edge->t1->getCircumCenterY());
				vboCoords.push_back(1.0f);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(0.0f);
				float x = points.at(edge->v2)->getX() - points.at(edge->v1)->getX();
				float y = points.at(edge->v2)->getY() - points.at(edge->v1)->getY();
				vboCoords.push_back(edge->t1->getCircumCenterX() + y);
				vboCoords.push_back(edge->t1->getCircumCenterY() - x);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(1.0f);
			}
			else if (edge->t2 != nullptr)
			{
				size += 2;
				vboCoords.push_back(edge->t2->getCircumCenterX());
				vboCoords.push_back(edge->t2->getCircumCenterY());
				vboCoords.push_back(1.0f);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(0.0f);
				float x = points.at(edge->v2)->getX() - points.at(edge->v1)->getX();
				float y = points.at(edge->v2)->getY() - points.at(edge->v1)->getY();
				vboCoords.push_back(edge->t2->getCircumCenterX() - y);
				vboCoords.push_back(edge->t2->getCircumCenterY() + x);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(1.0f);
			}
		}
	}
	return size;
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