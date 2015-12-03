#include "Triangle.h"


Triangle::Triangle()
{
	this->indice1 = 0;
	this->indice2 = 0;
	this->indice3 = 0;
}
Triangle::Triangle(unsigned short indice1, unsigned short indice2, unsigned short indice3)
{
	this->indice1 = indice1;
	this->indice2 = indice2;
	this->indice3 = indice3;
	this->removed = false;
}

void Triangle::setIndices(unsigned short indice1, unsigned short indice2, unsigned short indice3)
{
	this->indice1 = indice1;
	this->indice2 = indice2;
	this->indice3 = indice3;
}
void Triangle::setIndice1(unsigned short indice1)
{
	this->indice1 = indice1;
}
void Triangle::setIndice2(unsigned short indice2)
{
	this->indice2 = indice2;
}
void Triangle::setIndice3(unsigned short indice3)
{
	this->indice3 = indice3;
}

void Triangle::setCircumCenter(Circle& c)
{
	this->circumCenter = c;
	this->circumCircle = true;
}
bool Triangle::hasCircumCircle()
{
	return this->circumCircle;
}
Circle& Triangle::getCircumCenter()
{
	return circumCenter;
}


void Triangle::addIndice(unsigned short indice)
{
	if (this->indice1 == 0)
		this->indice1 = indice1;
	else if (this->indice2 == 0)
		this->indice2 = indice2;
	else if (this->indice3 == 0)
		this->indice3 = indice3;
}

unsigned short Triangle::getIndice1()
{
	return indice1;
}
unsigned short Triangle::getIndice2()
{
	return indice2;
}
unsigned short Triangle::getIndice3()
{
	return indice3;
}

Triangle::~Triangle()
{
}
