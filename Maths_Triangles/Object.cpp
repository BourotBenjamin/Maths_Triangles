#include "Object.h"
# define M_PI           3.14159265358979323846


Object::Object()
{
	pyramids = std::vector<std::shared_ptr<Pyramid>>();
	/*
	points.push_back(std::shared_ptr<Point>(new Point(0.f, 0.f, 0.f)));
	points.push_back(std::shared_ptr<Point>(new Point(10000.f, 0.f, 0.f)));
	points.push_back(std::shared_ptr<Point>(new Point(5000.f, 10000.f, 0.f)));
	*/
}
/*
void Object::addTriangle(std::shared_ptr<Triangle>& triangle)
{
	//triangles.push_back(triangle);
}
*/

float maxZ = 400.f;

void Object::generateTriangulation3D(std::vector<float>& vboCoords, std::vector<unsigned int>& eboIndices, unsigned int firstIndex, bool voronoi)
{
	triangulation.insert(pts3D.begin(), pts3D.end());
	if (triangulation.is_valid())
	{
		int i = 0;
		auto face = triangulation.finite_cells_begin();
		while (face != triangulation.finite_cells_end())
		{
			vboCoords.push_back(face->vertex(0)->point().x());
			vboCoords.push_back(face->vertex(0)->point().y());
			if (voronoi)
				vboCoords.push_back(face->vertex(0)->point().z());
			else
				vboCoords.push_back(0.0f);
			vboCoords.push_back((face->vertex(0)->point().z() + maxZ) / (maxZ * 2.0f));
			vboCoords.push_back(1.0f);
			vboCoords.push_back(1.0f);
			vboCoords.push_back(face->vertex(1)->point().x());
			vboCoords.push_back(face->vertex(1)->point().y());
			if (voronoi)
				vboCoords.push_back(face->vertex(1)->point().z());
			else
				vboCoords.push_back(0.0f);
			vboCoords.push_back((face->vertex(1)->point().z() + maxZ) / (maxZ * 2.0f));
			vboCoords.push_back(1.0f);
			vboCoords.push_back(1.0f);
			vboCoords.push_back(face->vertex(2)->point().x());
			vboCoords.push_back(face->vertex(2)->point().y());
			if (voronoi)
				vboCoords.push_back(face->vertex(2)->point().z());
			else
				vboCoords.push_back(0.0f);
			vboCoords.push_back((face->vertex(2)->point().z() + maxZ) / (maxZ * 2.0f));
			vboCoords.push_back(1.0f);
			vboCoords.push_back(1.0f);
			eboIndices.push_back(i * 3 + firstIndex);
			eboIndices.push_back(i * 3 + 1 + firstIndex);
			eboIndices.push_back(i * 3 + 2 + firstIndex);
			++i;
			++face;
		}
	}
	else
		std::cout << "Invalid" << std::endl;
}

float getRandomCoords(int maxDist, float middle)
{
	int dist = maxDist / 10;
	while (dist <= maxDist)
	{
		if (rand() % 3 > 1)
			return rand() % (dist * 2) + middle - dist;
		dist *= 1.5;
	}
	return rand() % (dist * 2) + middle - dist;
}

void Object::generatePoints(float x, float y, float z, int maxDist, int nbPoints)
{
	points.push_back(std::shared_ptr<Point>(new Point(x, y, z)));
	int i = 0;
	float x2, y2, z2;
	bool ok;
	maxZ = maxDist * 1.0f;
	while (i < nbPoints)
	{
		++i;
		x2 = getRandomCoords(maxDist, x);
		y2 = getRandomCoords(maxDist, y);
		z2 = getRandomCoords(maxDist, z);
		points.push_back(std::shared_ptr<Point>(new Point(x2, y2, z2)));
		pts3D.push_back(Point_3(x2, y2, z2));
	}
	std::cout << " Points " << std::endl;
}

void Object::addPoint(std::shared_ptr<Point>& p)
{
	for each (auto p2 in points)
	{
		if (p2->getX() == p->getX() && p2->getY() == p->getY())
			return;
	}
	points.push_back(p);
	pts3D.push_back(Point_3(p->getX(), p->getY(), p->getZ()));
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
				else if (first->getX() == (*ptr)->getX() && first->getY() == (*ptr)->getY() && first->getZ() > (*ptr)->getZ())
					first = (*ptr);
			}
			ptr++;
		}
		enveloppe.push_back(first);
		vboCoords.push_back(first->getX());
		vboCoords.push_back(first->getY());
		vboCoords.push_back(first->getZ());
		vboCoords.push_back(1.0f);
		if (!first->getSelected())
			vboCoords.push_back(1.0f);
		else
			vboCoords.push_back(0.0f);
		vboCoords.push_back(1.0f);
		std::shared_ptr<Point> lastP = std::shared_ptr<Point>(new Point(first->getX(), 0.0f, 0.0f)), current = first, best = nullptr;
		float angle = 0.0f, bestAngle = 0.0f;
		do
		{
			bestAngle = 0.0f;
			float v1X = current->getX() - lastP->getX(), v1Y = current->getY() - lastP->getY(), v1Z = current->getZ() - lastP->getZ(), v2X = 0.0f, v2Y = 0.0f, v2Z = 0.0f;
			float l1 = sqrt(v1X*v1X + v1Y*v1Y + v1Z*v1Z), l2 = 0.0;
			auto ptr = points.begin();
			while (ptr != points.end())
			{
				if (*(*ptr) != *current)
				{
					v2X = current->getX() - (*ptr)->getX(), v2Y = current->getY() - (*ptr)->getY(), v2Z = current->getZ() - (*ptr)->getZ();
					l2 = sqrt(v2X*v2X + v2Y*v2Y + v2Z*v2Z);
					angle = acos((v1X*v2X + v1Y*v2Y + v1Z*v2Z) / (l1*l2));	// calc angle between lastP->current current->ptr
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
			vboCoords.push_back(best->getZ());
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
		xBarycentre = 0.0f, yBarycentre = 0.0f, zBarycentre = 0.0f;
		auto ptr = points.begin();
		int pointsSize = 0;
		while (ptr != points.end())
		{
			pointsSize++;
			xBarycentre += (*ptr)->getX();
			yBarycentre += (*ptr)->getY();
			zBarycentre += (*ptr)->getZ();
			enveloppe.push_back((*ptr));
			ptr++;
		}
		xBarycentre /= pointsSize;
		yBarycentre /= pointsSize;
		zBarycentre /= pointsSize;
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
			vboCoords.push_back((*ptr)->getZ());
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
		vboCoords.push_back((*ptr)->getZ());
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

// Return wich  side of triangle is D 
// > 0 left / < 0 right
float Object::calcAngle3D(std::shared_ptr<Triangle> triangle, std::shared_ptr<Point> D)
{
	float aX = points.at(triangle->getIndice1())->getX(), aY = points.at(triangle->getIndice1())->getX(), aZ = points.at(triangle->getIndice1())->getZ();
	float bX = points.at(triangle->getIndice2())->getX(), bY = points.at(triangle->getIndice2())->getX(), bZ = points.at(triangle->getIndice2())->getZ();
	float cX = points.at(triangle->getIndice3())->getX(), cY = points.at(triangle->getIndice3())->getX(), cZ = points.at(triangle->getIndice3())->getZ();
	float nX = (cY - aY) * (bZ - aZ) - (cZ - aZ) * (bY - aY);
	float nY = (cZ - aZ) * (bX - aX) - (cX - aX) * (bZ - aZ);
	float nZ = (cX - aX) * (bY - aY) - (cY - aY) * (bX - aX);
	// Vecteur normal N

	// Angle D CFace N
	// CFaceNx = nX car la normal part de CFace

	float dCFaceX = ((aX + bX + cX) / 3) - D->getX();
	float dCFaceY = ((aY + bY + cY) / 3) - D->getY();
	//float dX = D->getX() , dY = D->getY();
	float angle = (atan2(dCFaceY, dCFaceX) - atan2(nY, nX));
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

/*
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
*/

/*
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
*/

Circle Object::circumCircle(std::shared_ptr<Point>& a, std::shared_ptr<Point>& b, std::shared_ptr<Point>& c)
{
	Circle circle;
	float aX = a->getX(), aY = a->getY(), aZ = a->getZ(), bX = b->getX(), bY = b->getY(), bZ = b->getZ(), cX = c->getX(), cY = c->getY(), cZ = c->getZ();
	float acX = cX - aX, acY = cY - aY, acZ = cZ - aZ;
	float abX = bX - aX, abY = bY - aY, abZ = bZ - aZ;
	float abXacX = (abY * acZ) - (abZ * acY);
	float abXacY = (abZ * acX) - (abX * acZ);
	float abXacZ = (abX * acY) - (abY * acX);
	float acLen2 = acX * acX + acY * acY + acZ * acZ;
	float abLen2 = abX * abX + abY * abY + abZ * abZ;
	float abXacLen2 = abXacX * abXacX + abXacY * abXacY + abXacZ * abXacZ;

	circle.x = (((abXacY * abZ) - (abXacZ * abY)) * acLen2 + ((acY * abXacZ) - (acZ * abXacY)) * abLen2) / (2.f * abXacLen2);
	circle.y = (((abXacZ * abX) - (abXacX * abZ)) * acLen2 + ((acZ * abXacX) - (acX * abXacZ)) * abLen2) / (2.f * abXacLen2);
	circle.z = (((abXacX * abY) - (abXacY * abX)) * acLen2 + ((acX * abXacY) - (acY * abXacX)) * abLen2) / (2.f * abXacLen2);
	circle.r2 = circle.x * circle.x + circle.y * circle.y + circle.z * circle.z;
	circle.x += aX;
	circle.y += aY;
	circle.z += aZ;
	return circle;
}

Circle baryCentre(std::shared_ptr<Point>& a, std::shared_ptr<Point>& b, std::shared_ptr<Point>& c)
{
	Circle circle;
	float aX = a->getX(), aY = a->getY(), aZ = a->getZ(), bX = b->getX(), bY = b->getY(), bZ = b->getZ(), cX = c->getX(), cY = c->getY(), cZ = c->getZ();
	circle.x = (aX + bX + cX) / 3;
	circle.y = (aY + bY + cY) / 3;
	circle.z = (aZ + bZ + cZ) / 3;
	circle.r2 = 0;
	return circle;
}

std::shared_ptr<Triangle> Object::findUsedTriangle(std::vector<std::shared_ptr<Triangle>>& usedTriangles, unsigned int p1, unsigned int p2, unsigned int p3)
{
	for each(auto triangle in usedTriangles)
	{
		if (triangle->hasPoints(p1, p2, p3))
		{
			return triangle;
		}
	}
	return nullptr;
}

void Object::addPyramidToTriangleUsed(std::shared_ptr<Triangle> triangle, unsigned int pointNotOnTriangle, std::shared_ptr<Pyramid> pyramid)
{
	if (calcAngle3D(triangle, points.at(pointNotOnTriangle)) > 0)
	{
		triangle->p1 = pyramid;
	}
	else
	{
		triangle->p2 = pyramid;
	}
}

void Object::createPyramidsAndAddUsedTriangles(std::vector<std::shared_ptr<Triangle>>& usedTriangles, unsigned int common1, unsigned int common2, unsigned int common3, unsigned int alone1, unsigned int alone2)
{
	// Two pyramids formed by t1 t2 t3 t4 && t4 t5 t6 t7
	std::shared_ptr<Triangle> t1, t2, t3, t4, t5, t6, t7;
	t1 = findUsedTriangle(usedTriangles, common1, common2, alone1);
	if (t1 == nullptr)
	{
		t1 = std::shared_ptr<Triangle>(new Triangle(common1, common2, alone1));
		usedTriangles.push_back(t1);
	}
	t2 = findUsedTriangle(usedTriangles, common3, common2, alone1);
	if (t2 == nullptr)
	{
		t2 = std::shared_ptr<Triangle>(new Triangle(common3, common2, alone1));
		usedTriangles.push_back(t2);
	}
	t3 = findUsedTriangle(usedTriangles, common1, common3, alone1);
	if (t3 == nullptr)
	{
		t3 = std::shared_ptr<Triangle>(new Triangle(common1, common3, alone1));
		usedTriangles.push_back(t3);
	}
	t4 = findUsedTriangle(usedTriangles, common1, common2, common3);
	if (t4 == nullptr)
	{
		t4 = std::shared_ptr<Triangle>(new Triangle(common1, common2, common3));
		usedTriangles.push_back(t4);
	}
	std::shared_ptr<Pyramid> newP1 = std::shared_ptr<Pyramid>(new Pyramid(common1, common2, common3, alone1, t1, t2, t3, t4));
	pyramids.push_back(newP1);
	addPyramidToTriangleUsed(t1, common3, newP1);
	addPyramidToTriangleUsed(t2, common1, newP1);
	addPyramidToTriangleUsed(t3, common2, newP1);
	addPyramidToTriangleUsed(t4, alone1, newP1);
	if (alone2 != -1)
	{
		t5 = findUsedTriangle(usedTriangles, common1, common2, alone2);
		if (t5 == nullptr)
		{
			t5 = std::shared_ptr<Triangle>(new Triangle(common1, common2, alone2));
			usedTriangles.push_back(t5);
		}
		t6 = findUsedTriangle(usedTriangles, common3, common2, alone2);
		if (t6 == nullptr)
		{
			t6 = std::shared_ptr<Triangle>(new Triangle(common3, common2, alone2));
			usedTriangles.push_back(t6);
		}
		t7 = findUsedTriangle(usedTriangles, common1, common3, alone2);
		if (t7 == nullptr)
		{
			t7 = std::shared_ptr<Triangle>(new Triangle(common1, common3, alone2));
			usedTriangles.push_back(t7);
		}
		std::shared_ptr<Pyramid> newP2 = std::shared_ptr<Pyramid>(new Pyramid(common1, common2, common3, alone2, t4, t5, t6, t7));
		pyramids.push_back(newP2);
		addPyramidToTriangleUsed(t5, common3, newP2);
		addPyramidToTriangleUsed(t6, common1, newP2);
		addPyramidToTriangleUsed(t7, common2, newP2);
		addPyramidToTriangleUsed(t4, alone2, newP2);
	}
}

void Object::createNewPyramids(std::vector<std::shared_ptr<Triangle>>& usedTriangles, std::shared_ptr<Pyramid> p1, std::shared_ptr<Pyramid> p2)
{
	p1->removed = true;
	p2->removed = true;
	// Recherche des points communs entre les deux pyramides
	// a b c : Points en commun
	// d : Point de la pyramide 1 uniquement
	// e : Point de la pyramide 2 uniquement
	unsigned int a = -1, b = -1, c = -1, d = -1, e = -1;
	if (p1->p1 == p2->p1 || p1->p1 == p2->p2 || p1->p1 == p2->p3 || p1->p1 == p2->p4)
		a = p1->p1;
	if (p1->p2 == p2->p1 || p1->p2 == p2->p2 || p1->p2 == p2->p3 || p1->p2 == p2->p4)
	{
		if (a == -1)
			a = p1->p2;
		else
			b = p1->p2;
	}
	if (p1->p3 == p2->p1 || p1->p3 == p2->p2 || p1->p3 == p2->p3 || p1->p3 == p2->p4)
	{
		if (b == -1)
			b = p1->p3;
		else
			c = p1->p3;
	}
	if (p1->p4 == p2->p1 || p1->p4 == p2->p2 || p1->p4 == p2->p3 || p1->p4 == p2->p4)
		c = p1->p4;

	d = (p1->p1 == a) ? ((p1->p2 == a || p1->p2 == b) ? ((p1->p3 == b || p1->p3 == c) ? p1->p4 : p1->p3) : p1->p2) : p1->p1;
	e = (p1->p1 == a || p1->p1 == b || p1->p1 == c) ? ((p1->p2 == a || p1->p2 == b || p1->p2 == c) ? ((p1->p3 == a || p1->p3 == b || p1->p3 == c) ? p1->p4 : p1->p3) : p1->p2) : p1->p1;

	// Creation des nouvelles pyramides
	// Nouveaux points en communs : d + e + (a b ou c en fonction du rand)
	// Point de la pyramide 1 uniquement : a b ou c en fonction du rand
	// Point de la pyramide 2 uniquement : a b ou c en fonction du rand
	int random = rand() % 3;
	if (random == 0)
		createPyramidsAndAddUsedTriangles(usedTriangles, d, e, a, b, c);
	else if (random == 1)
		createPyramidsAndAddUsedTriangles(usedTriangles, d, e, b, c, a);
	else
		createPyramidsAndAddUsedTriangles(usedTriangles, d, e, c, a, b);
}


void Object::flipping3D()
{
	int loops = 0;
	bool changed = false;
	usedTrianglesOrig.clear();
	std::vector<std::shared_ptr<Triangle>> usedTrianglesCopy;
	std::vector<std::shared_ptr<Pyramid>> newPyramids;
	for each (auto triangle in usedTrianglesOrig)
	{
		usedTrianglesCopy.push_back(triangle);
	}
	while (usedTrianglesCopy.size() > 0 && loops < 5000)
	{
		++loops;
		std::shared_ptr<Triangle> face = usedTrianglesCopy.back();
		usedTrianglesCopy.pop_back();
		if (face->p1 != nullptr && face->p2 != nullptr)
		{
			std::shared_ptr<Circle> circle = getCircumsphere(face->p1), circle2 = getCircumsphere(face->p2);
			if (containsPyramid(circle, face->p2) || containsPyramid(circle2, face->p1))
			{
				createNewPyramids(usedTrianglesCopy, face->p1, face->p2);
			}
		}
	}
	if (loops >= 5000)
		std::cout << "Lots of loop" << std::endl;
	usedTrianglesOrig.clear();
	std::shared_ptr<Triangle> t1, t2, t3, t4;
	for each (auto p in pyramids)
	{
		if (!p->removed)
		{
			newPyramids.push_back(p);
			if (std::find(usedTrianglesOrig.begin(), usedTrianglesOrig.end(), p->t1) == usedTrianglesOrig.end()) {
				usedTrianglesOrig.push_back(p->t1);
			}
			if (std::find(usedTrianglesOrig.begin(), usedTrianglesOrig.end(), p->t2) == usedTrianglesOrig.end()) {
				usedTrianglesOrig.push_back(p->t2);
			}
			if (std::find(usedTrianglesOrig.begin(), usedTrianglesOrig.end(), p->t3) == usedTrianglesOrig.end()) {
				usedTrianglesOrig.push_back(p->t3);
			}
			if (std::find(usedTrianglesOrig.begin(), usedTrianglesOrig.end(), p->t4) == usedTrianglesOrig.end()) {
				usedTrianglesOrig.push_back(p->t4);
			}
		}
	}
	pyramids = newPyramids;
}

bool Object::containsPyramid(std::shared_ptr<Circle> circle, std::shared_ptr<Pyramid> pyramid)
{
	float p1X = points.at(pyramid->p1)->getX(), p1Y = points.at(pyramid->p1)->getY(), p1Z = points.at(pyramid->p1)->getZ();
	float p2X = points.at(pyramid->p2)->getX(), p2Y = points.at(pyramid->p2)->getY(), p2Z = points.at(pyramid->p2)->getZ();
	float p3X = points.at(pyramid->p3)->getX(), p3Y = points.at(pyramid->p3)->getY(), p3Z = points.at(pyramid->p3)->getZ();
	float p4X = points.at(pyramid->p4)->getX(), p4Y = points.at(pyramid->p4)->getY(), p4Z = points.at(pyramid->p4)->getZ();
	if (((circle->x - p1X) * (circle->x - p1X) + (circle->y - p1Y) * (circle->y - p1Y) + (circle->z - p1Z) * (circle->z - p1Z)) > circle->r2
	&& ((circle->x - p2X) * (circle->x - p2X) + (circle->y - p2Y) * (circle->y - p2Y) + (circle->z - p2Z) * (circle->z - p2Z)) > circle->r2
	&& ((circle->x - p3X) * (circle->x - p3X) + (circle->y - p3Y) * (circle->y - p3Y) + (circle->z - p3Z) * (circle->z - p3Z)) > circle->r2
	&& ((circle->x - p4X) * (circle->x - p4X) + (circle->y - p4Y) * (circle->y - p4Y) + (circle->z - p4Z) * (circle->z - p4Z)) > circle->r2)
		return true;
	return false;
}

std::shared_ptr<Circle> Object::getCircumsphere(std::shared_ptr<Pyramid> p)
{
	if (p->circumSphere != nullptr)
		return p->circumSphere;
	double a[3];
	float p1X = points.at(p->p1)->getX(), p1Y = points.at(p->p1)->getY(), p1Z = points.at(p->p1)->getZ();
	float p2X = points.at(p->p2)->getX(), p2Y = points.at(p->p2)->getY(), p2Z = points.at(p->p2)->getZ();
	float p3X = points.at(p->p3)->getX(), p3Y = points.at(p->p3)->getY(), p3Z = points.at(p->p3)->getZ();
	float p4X = points.at(p->p4)->getX(), p4Y = points.at(p->p4)->getY(), p4Z = points.at(p->p4)->getZ();
	a[0] = pow(p2X - p1X, 2) + pow(p2Y - p1Y, 2) + pow(p2Z - p1Z, 2);
	a[1] = pow(p3X - p1X, 2) + pow(p3Y - p1Y, 2) + pow(p3Z - p1Z, 2);
	a[2] = pow(p4X - p1X, 2) + pow(p4Y - p1Y, 2) + pow(p4Z - p1Z, 2);
	std::shared_ptr<Circle> c(new Circle());
	c->r = 0.5 * sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	c->r2 = c->r * c->r;
	c->x = p1X + 0.5 * a[0];
	c->y = p1Y + 0.5 * a[1];
	c->z = p1Z + 0.5 * a[2];
	p->circumSphere = c;
	return c;
}
/*
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
				circle = circumsedCircle(points.at(i1), points.at(i2), points.at(i3));
				edge->t1->setCircumCenter(circle);
			}
			if (edge->t2->hasCircumCircle())
				circle2 = edge->t2->getCircumCenter();
			else
			{
				circle2 = circumsedCircle(points.at(it1), points.at(it2), points.at(it3));
				edge->t2->setCircumCenter(circle2);
			}
			auto point = points.at(c), point2 = points.at(d);
			if (sqrt((circle.x - point->getX()) * (circle.x - point->getX()) + (circle.y - point->getY()) * (circle.y - point->getY()) + (circle.z - point->getZ()) * (circle.z - point->getZ())) <= circle.r
				|| sqrt((circle2.x - point2->getX()) * (circle2.x - point2->getX()) + (circle2.y - point2->getY()) * (circle2.y - point2->getY()) + (circle2.z - point2->getZ()) * (circle2.z - point2->getZ())) <= circle2.r)
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
*/

std::shared_ptr<Triangle> superTriangle(std::vector<std::shared_ptr<Point>> points)
{
	return std::shared_ptr<Triangle>(new Triangle(0, 1, 2));
}

bool noEdgeInCommon(std::shared_ptr<Triangle> triangle, std::shared_ptr<Triangle> triangle2)
{
	unsigned int v11 = triangle->getIndice1(), v12 = triangle->getIndice2(), v13 = triangle->getIndice3(), v21 = triangle2->getIndice1(), v22 = triangle2->getIndice2(), v23 = triangle2->getIndice3();
	return (v11 != v21 && v11 != v22 && v11 != v23) && (v12 != v21 && v12 != v22 && v12 != v23) && (v13 != v21 && v13 != v22 && v13 != v23);
}

bool notIn(std::shared_ptr<Triangle> triangle, std::vector<std::shared_ptr<Triangle>> triangles)
{
	for each (auto triangle2 in triangles)
	{
		if (triangle->hasPoints(triangle2->getIndice1(), triangle2->getIndice2(), triangle2->getIndice3()))
			return false;
	}
	return true;
}

unsigned short Object::delaunayBowyerWatson(std::vector<float>& vboCoords, std::vector<unsigned int>& eboIndices, bool voronoi, unsigned int firstIndex)
{
	int indice = 0;
	usedTrianglesOrig.clear();
	std::vector<std::shared_ptr<Triangle>> toKeep;
	std::vector<SimpleEdge> enclosing;
	std::shared_ptr<Triangle> supertri = superTriangle(points); // Un triangle contenant tous les points à trianguler.
	usedTrianglesOrig.push_back(supertri);
	for each (auto sommet in points)
	{
		vboCoords.push_back(sommet->getX());
		vboCoords.push_back(sommet->getY());
		vboCoords.push_back(sommet->getZ());
		vboCoords.push_back(0.f);
		if (!sommet->getSelected())
			vboCoords.push_back(1.0f);
		else
			vboCoords.push_back(0.0f);
		vboCoords.push_back(0.f);
		if (indice > 2)
		{
			// Tous les triangles dont le cercle circonscrit contient le sommet à ajouter sont identifiés,
			// l'ensemble de leurs arêtes forment un polygone englobant.

			// Démarrer avec un polygone englobant vide.
			enclosing.clear();
			// Pour l'instant, il n'y a aucun triangle subdivisé y supprimer.
			toKeep.clear();
			for each (auto triangle in usedTrianglesOrig)
			{
				if (!triangle->hasCircumCircle())
					triangle->setCircumCenter(this->circumCircle(points.at(triangle->getIndice1()), points.at(triangle->getIndice2()), points.at(triangle->getIndice3())));
				Circle circle = triangle->getCircumCenter();
				if (circle.x < sommet->getX() && ((circle.x - sommet->getX())*(circle.x - sommet->getX())) > circle.r2)
					triangle->done = true;
				// Si le sommet courant est dans le cercle circonscrit du triangle courant,
				if (((circle.x - sommet->getX())*(circle.x - sommet->getX())) + ((circle.y - sommet->getY())*(circle.y - sommet->getY())) + ((circle.z - sommet->getZ())*(circle.z - sommet->getZ())) < circle.r2)
				{
					// ajouter les arêtes de ce triangle au polygone candidat,
					enclosing.push_back(SimpleEdge(triangle->getIndice1(), triangle->getIndice2()));
					enclosing.push_back(SimpleEdge(triangle->getIndice2(), triangle->getIndice3()));
					enclosing.push_back(SimpleEdge(triangle->getIndice3(), triangle->getIndice1()));
				}
				else
					toKeep.push_back(triangle);
			}
			usedTrianglesOrig.clear();
			for each (auto triangle in toKeep)
			{
				if (notIn(triangle, usedTrianglesOrig))
				{
					usedTrianglesOrig.push_back(triangle);
				}
			}
			for each (auto edge in enclosing)
			{
				std::shared_ptr<Triangle> triangle(new Triangle(edge.v1, edge.v2, indice));
				triangle->done = false;
				if (notIn(triangle, usedTrianglesOrig))
				{
					usedTrianglesOrig.push_back(triangle);
				}
			}
		}
		++indice;
		std::cout << indice << std::endl;
	}

	for each (auto triangle in usedTrianglesOrig)
	{
		if (noEdgeInCommon(triangle, supertri))
		{
			eboIndices.push_back(triangle->getIndice1() + firstIndex);
			eboIndices.push_back(triangle->getIndice2() + firstIndex);
			eboIndices.push_back(triangle->getIndice3() + firstIndex);
		}
	}
	return 0;
}

unsigned short Object::simpleTriangulation3D(std::vector<float>& vboCoords, std::vector<unsigned int>& eboIndices, bool flipping, bool voronoi, unsigned int firstIndex)
{
	generateTriangulation3D(vboCoords, eboIndices, firstIndex, voronoi);
	return 0;
}

unsigned short Object::triangulation3D(std::vector<float>& vboCoords, std::vector<unsigned int>& eboIndices, bool flipping, bool voronoi, unsigned int firstIndex)
{
	pyramids.clear();
	usedTrianglesOrig.clear();
	std::sort(points.begin(), points.end(), cmpPointsAbsAndOrd);

	StatesimpleTriangulation state = INIT_FRIST_POINT;
	int currentIndice = 0;
	std::vector<std::shared_ptr<Triangle>> oldUsedTriangles;
	bool found = false;
	for each (auto point in points)
	{
		vboCoords.push_back(point->getX());
		vboCoords.push_back(point->getY());
		vboCoords.push_back(point->getZ());
		vboCoords.push_back(point->getZ());
		if (!point->getSelected())
			vboCoords.push_back(1.0f);
		else
			vboCoords.push_back(0.0f);
		vboCoords.push_back(1.f-point->getZ());
		if (currentIndice > 3)
		{
			oldUsedTriangles.clear();
			for each (auto triangle in usedTrianglesOrig)
				oldUsedTriangles.push_back(triangle);
			for each (auto triangle in oldUsedTriangles)
			{
				if (triangle->p1 == nullptr || triangle->p2 == nullptr)
				{
					found = false;
					float angle = calcAngle3D(triangle, point);
					if (angle > 0 && triangle->p1 != nullptr)
						found = true;
					else if (angle <= 0 && triangle->p2 != nullptr)
						found = true;
					if (!found)
						createPyramidsAndAddUsedTriangles(usedTrianglesOrig, triangle->getIndice1(), triangle->getIndice2(), triangle->getIndice3(), currentIndice, -1);
				}
			}
		}
		else if (currentIndice == 3)
			createPyramidsAndAddUsedTriangles(usedTrianglesOrig, 0, 1, 2, 3, -1);
		currentIndice++;
	}

	if (flipping)
		this->flipping3D();
	for each (auto pyramid in pyramids)
	{
		if (!pyramid->removed)
		{
			eboIndices.push_back(pyramid->p1 + firstIndex);
			eboIndices.push_back(pyramid->p2 + firstIndex);
			eboIndices.push_back(pyramid->p3 + firstIndex);
			eboIndices.push_back(pyramid->p1 + firstIndex);
			eboIndices.push_back(pyramid->p3 + firstIndex);
			eboIndices.push_back(pyramid->p4 + firstIndex);
			eboIndices.push_back(pyramid->p2 + firstIndex);
			eboIndices.push_back(pyramid->p3 + firstIndex);
			eboIndices.push_back(pyramid->p4 + firstIndex);
		}
	}
	int size = 0;
	// TODO Voronoi
	/*
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
				vboCoords.push_back(c1.z);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(c2.x);
				vboCoords.push_back(c2.y);
				vboCoords.push_back(c2.z);
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
				vboCoords.push_back(c1.z);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(0.0f);
				float x = points.at(edge->v2)->getX() - points.at(edge->v1)->getX();
				float y = points.at(edge->v2)->getY() - points.at(edge->v1)->getY();
				vboCoords.push_back(c1.x + y);
				vboCoords.push_back(c1.y - x);
				vboCoords.push_back(c1.z - x);
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
				vboCoords.push_back(c2.z - x);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(1.0f);
			}
		}
	}
	*/
	//return size;
	return 0;
}

/*

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
		vboCoords.push_back(point->getZ());
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
				vboCoords.push_back(c1.z);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(c2.x);
				vboCoords.push_back(c2.y);
				vboCoords.push_back(c2.z);
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
				vboCoords.push_back(c1.z);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(0.0f);
				float x = points.at(edge->v2)->getX() - points.at(edge->v1)->getX();
				float y = points.at(edge->v2)->getY() - points.at(edge->v1)->getY();
				vboCoords.push_back(c1.x + y);
				vboCoords.push_back(c1.y - x);
				vboCoords.push_back(c1.z - x);
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
				vboCoords.push_back(c2.z - x);
				vboCoords.push_back(0.0f);
				vboCoords.push_back(1.0f);
				vboCoords.push_back(1.0f);
			}
		}
	}
	return size;
}
*/

Object::~Object()
{
}

bool cmpPointsAbsAndOrd(const std::shared_ptr<Point>& a, const std::shared_ptr<Point>& b)
{
	return (a->getX() < b->getX() || (a->getX() == b->getX() && a->getY() < b->getY()) || (a->getX() == b->getX() && a->getY() == b->getY() && a->getZ() < b->getZ()));
}

bool cmpPointsAngleFromBarycentre(const std::shared_ptr<Point>& a, const std::shared_ptr<Point>& b)
{
	return (a->getAngleFromBary() < b->getAngleFromBary());
}

std::shared_ptr<Point> Object::findNearestPoint(float x, float y, float z)
{
	std::shared_ptr<Point> best = nullptr;
	float min = FLT_MAX, dist;
	for each (auto p in points)
	{
		dist = sqrt((x - p->getX()) * (x - p->getX()) + (y - p->getY()) * (y - p->getY()) + (z - p->getZ()) * (z - p->getZ()));
		if (dist < min)
		{
			min = dist;
			best = p;
		}
	}
	return best;
}