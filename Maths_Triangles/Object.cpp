﻿#include "Object.h"
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
			float ABx = 0.0f, CBx = 0.0f, ABy = 0.0f, CBy = 0.0f;
			if (*current == enveloppe.front())
			{
				ABx = (*current)->getX() - enveloppe.back()->getX();
				ABy = (*current)->getY() - enveloppe.back()->getY();
				CBx = (*(current + 1))->getX() - (*(current))->getX();
				CBy = (*(current + 1))->getY() - (*(current))->getY();
			}
			else if (*current == enveloppe.back())
			{
				ABx = (*(current))->getX() - (*(current - 1))->getX();
				ABy = (*(current))->getY() - (*(current - 1))->getY();
				CBx = enveloppe.front()->getX() - (*(current))->getX();
				CBy = enveloppe.front()->getY() - (*(current))->getY();
			}
			else
			{
				ABx = (*(current))->getX() - (*(current - 1))->getX();
				ABy = (*(current))->getY() - (*(current - 1))->getY();
				CBx = (*(current + 1))->getX() - (*(current))->getX();
				CBy = (*(current + 1))->getY() - (*(current))->getY();
			}

			float angle = (atan2(ABy, ABx) - atan2(CBy, CBx));
			while (angle > M_PI)
				angle -= M_PI * 2;
			while (angle < -M_PI)
				angle += M_PI * 2;
			//if (angle > (M_PI / -2) && angle < (M_PI / 2))
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

Object::~Object()
{
}


bool cmpPointsAngleFromBarycentre(const std::shared_ptr<Point>& a, const std::shared_ptr<Point>& b)
{
	return (a->getAngleFromBary() < b->getAngleFromBary());
}