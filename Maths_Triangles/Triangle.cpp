#include "Triangle.h"


Triangle::Triangle(unsigned short indice1, unsigned short indice2, unsigned short indice3)
{
	this->indice1 = indice1;
	this->indice2 = indice2;
	this->indice3 = indice3;
}

void Triangle::setIndices(unsigned short indice1, unsigned short indice2, unsigned short indice3)
{
	this->indice1 = indice1;
	this->indice2 = indice2;
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
