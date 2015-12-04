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
	for each (auto p2 in points)
	{
		if (p2->getX() == p->getX() && p2->getY() == p->getY())
			return;
	}
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
		if (!(*ptr)->getSelected())
			vboCoords.push_back(1.0f);
		else
			vboCoords.push_back(0.0f);
		vboCoords.push_back(1.0f);
		i++;
		ptr++;
	}
	return i;
}
void Object::removePoint(std::shared_ptr<Point>& point)
{
	auto ptr = points.begin();
	while (ptr != points.end())
	{
		if (point->getX() == (*ptr)->getX() && point->getY() == (*ptr)->getY())
			points.erase(ptr);
		else
			ptr++;
	}
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
		if (!first->getSelected())
			vboCoords.push_back(1.0f);
		else
			vboCoords.push_back(0.0f);
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
			if (!best->getSelected())
				vboCoords.push_back(1.0f);
			else
				vboCoords.push_back(0.0f);
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
			if (!(*ptr)->getSelected())
				vboCoords.push_back(1.0f);
			else
				vboCoords.push_back(0.0f);
			vboCoords.push_back(1.0f);
			ptr++;
		}
		ptr = enveloppe.begin();
		vboCoords.push_back((*ptr)->getX());
		vboCoords.push_back((*ptr)->getY());
		vboCoords.push_back(1.0f);
		vboCoords.push_back(1.0f);
		if (!(*ptr)->getSelected())
			vboCoords.push_back(1.0f);
		else
			vboCoords.push_back(0.0f);
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

std::shared_ptr<Triangle>  Object::addUsedEdgesToVector(std::vector<std::shared_ptr<UsedEdge>>& usedEdges, unsigned int indice1, unsigned int indice2, unsigned int indice3)
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
	triangles.push_back(triangle);
	return triangle;
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
	int loops = 0;
	bool changed = false;
	std::vector<std::shared_ptr<UsedEdge>> usedEdgesCopy;
	std::vector<std::shared_ptr<Triangle>> newTrList;
	for each (auto edge in usedEdges)
	{
		usedEdgesCopy.push_back(edge);
	}
	while (usedEdgesCopy.size() > 0 && loops < 5000)
	{
		loops++;
		std::shared_ptr<UsedEdge> edge = usedEdgesCopy.back();
		usedEdgesCopy.pop_back();
		if (edge->t1 == nullptr && edge->t2 != nullptr)
		{
			for each (auto tr in triangles)
			{
				if (tr->removed == false)
				{
					unsigned int i1 = tr->getIndice1(), i2 = tr->getIndice2(), i3 = tr->getIndice3(), it1 = edge->t2->getIndice1(), it2 = edge->t2->getIndice2(), it3 = edge->t2->getIndice3();
					if ((edge->v1 == i1 || edge->v1 == i2 || edge->v1 == i3) && (edge->v2 == i1 || edge->v2 == i2 || edge->v2 == i3) &&
						((it1 != i1 && it1 != i2 && it1 != i3) || (it2 != i1 && it2 != i2 && it2 != i3) || (it3 != i1 && it3 != i2 && it3 != i3)))
					{
						edge->t1 = tr;
						break;
					}
				}
			}
		}
		else if (edge->t1 != nullptr && edge->t2 == nullptr)
		{
			for each (auto tr in triangles)
			{
				if (tr->removed == false)
				{
					unsigned int i1 = tr->getIndice1(), i2 = tr->getIndice2(), i3 = tr->getIndice3(), it1 = edge->t1->getIndice1(), it2 = edge->t1->getIndice2(), it3 = edge->t1->getIndice3();
					if ((edge->v1 == i1 || edge->v1 == i2 || edge->v1 == i3) && (edge->v2 == i1 || edge->v2 == i2 || edge->v2 == i3) &&
						((it1 != i1 && it1 != i2 && it1 != i3) || (it2 != i1 && it2 != i2 && it2 != i3) || (it3 != i1 && it3 != i2 && it3 != i3)))
					{
						edge->t2 = tr;
						break;
					}
				}
			}
		}
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

			Circle circle, circle2;
			if (edge->t1->hasCircumCircle())
				circle = edge->t1->getCircumCenter();
			else
			{
				circle = circumsedCircleconst(points.at(i1), points.at(i2), points.at(i3));
				edge->t1->setCircumCenter(circle);
			}
			if (edge->t2->hasCircumCircle())
				circle2 = edge->t2->getCircumCenter();
			else
			{
				circle2 = circumsedCircleconst(points.at(it1), points.at(it2), points.at(it3));
				edge->t2->setCircumCenter(circle2);
			}
			auto point = points.at(c), point2 = points.at(d);
			if (sqrt((circle.x - point->getX()) * (circle.x - point->getX()) + (circle.y - point->getY()) * (circle.y - point->getY())) <= circle.r 
				|| sqrt((circle2.x - point2->getX()) * (circle2.x - point2->getX()) + (circle2.y - point2->getY()) * (circle2.y - point2->getY())) <= circle2.r)
			{
				edge->t1->removed = true;
				edge->t2->removed = true;
				addUsedEdgesToVector(usedEdgesCopy, c, d, a);
				addUsedEdgesToVector(usedEdgesCopy, c, d, b);
			}
		}


	}
	if (loops >= 5000)
		std::cout << "Lots of loop" << std::endl;
	for each (auto t in triangles)
	{
		if (!t->removed)
			newTrList.push_back(t);
	}
	triangles.clear();
	usedEdges.clear();
	for each (auto t in newTrList)
	{
		std::shared_ptr<Triangle> t2 = addUsedEdgesToVector(usedEdges, t->getIndice1(), t->getIndice2(), t->getIndice3());
		t2->setCircumCenter(t->getCircumCenter());
	}
}

unsigned short Object::simpleTriangulation(std::vector<float>& vboCoords, std::vector<unsigned int>& eboIndices, bool flipping, bool voronoi, unsigned int firstIndex)
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
		if (!point->getSelected())
			vboCoords.push_back(1.0f);
		else
			vboCoords.push_back(0.0f);
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
						addUsedEdgesToVector(usedEdges, edge->v1, edge->v2, currentIndice);
				}
			}
		}
		else if (currentIndice == 2)
			addUsedEdgesToVector(usedEdges, 0, 1, 2);
		currentIndice++;
	}

	if (flipping)
		this->flipping(usedEdges);
	for each (auto triangle in triangles)
	{
		eboIndices.push_back(triangle->getIndice1() + firstIndex);
		eboIndices.push_back(triangle->getIndice2() + firstIndex);
		eboIndices.push_back(triangle->getIndice3() + firstIndex);
	}
	int size = 0;
	if (voronoi)
	{
		for each (auto edge in usedEdges)
		{
			if (edge->t1 != nullptr && edge->t2 != nullptr)
			{
				Circle c1 = edge->t1->getCircumCenter(), c2 = edge->t2->getCircumCenter();
				size += 2;
				vboCoords.push_back(c1.x);
				vboCoords.push_back(c1.y);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(c2.x);
				vboCoords.push_back(c2.y);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(0.0f);
			}
			else if (edge->t1 != nullptr)
			{
				Circle c1 = edge->t1->getCircumCenter();
				size += 2;
				vboCoords.push_back(c1.x);
				vboCoords.push_back(c1.y);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(0.0f);
				float x = points.at(edge->v2)->getX() - points.at(edge->v1)->getX();
				float y = points.at(edge->v2)->getY() - points.at(edge->v1)->getY();
				vboCoords.push_back(c1.x + y);
				vboCoords.push_back(c1.y - x);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(1.0f);
			}
			else if (edge->t2 != nullptr)
			{
				Circle  c2 = edge->t2->getCircumCenter();
				size += 2;
				vboCoords.push_back(c2.x);
				vboCoords.push_back(c2.y);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(0.0f);
				float x = points.at(edge->v2)->getX() - points.at(edge->v1)->getX();
				float y = points.at(edge->v2)->getY() - points.at(edge->v1)->getY();
				vboCoords.push_back(c2.x - y);
				vboCoords.push_back(c2.y + x);
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

std::shared_ptr<Point> Object::findNearestPoint(float x, float y)
{
	std::shared_ptr<Point> best = nullptr;
	float min = FLT_MAX, dist;
	for each (auto p in points)
	{
		dist = sqrt((x - p->getX()) * (x - p->getX()) + (y - p->getY()) * (y - p->getY()));
		if (dist < min)
		{
			min = dist;
			best = p;
		}
	}
	return best;
}