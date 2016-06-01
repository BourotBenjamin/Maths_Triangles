#include "Pyramid.h"



Pyramid::Pyramid(unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4, std::shared_ptr<Triangle> t1, std::shared_ptr<Triangle> t2, std::shared_ptr<Triangle> t3, std::shared_ptr<Triangle> t4)
{
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->p4 = p4;
	this->t1 = t1;
	this->t2 = t2;
	this->t3 = t3;
	this->t4 = t4;
	this->removed = false;
	this->circumSphere = nullptr;
}

void Pyramid::getPoints(std::vector<unsigned int>& points)
{
	if (std::find(points.begin(), points.end(), this->t2->getIndice1()) == points.end()) {
		points.push_back(this->t2->getIndice1());
	}
	if (std::find(points.begin(), points.end(), this->t2->getIndice2()) == points.end()) {
		points.push_back(this->t2->getIndice2());
	}
	if (std::find(points.begin(), points.end(), this->t2->getIndice3()) == points.end()) {
		points.push_back(this->t2->getIndice3());
	}
	if (std::find(points.begin(), points.end(), this->t3->getIndice1()) == points.end()) {
		points.push_back(this->t3->getIndice1());
	}
	if (std::find(points.begin(), points.end(), this->t3->getIndice2()) == points.end()) {
		points.push_back(this->t3->getIndice2());
	}
	if (std::find(points.begin(), points.end(), this->t3->getIndice3()) == points.end()) {
		points.push_back(this->t3->getIndice3());
	}
	if (std::find(points.begin(), points.end(), this->t4->getIndice1()) == points.end()) {
		points.push_back(this->t4->getIndice1());
	}
	if (std::find(points.begin(), points.end(), this->t4->getIndice2()) == points.end()) {
		points.push_back(this->t4->getIndice2());
	}
	if (std::find(points.begin(), points.end(), this->t4->getIndice3()) == points.end()) {
		points.push_back(this->t4->getIndice3());
	}
}

Pyramid::~Pyramid()
{
}
