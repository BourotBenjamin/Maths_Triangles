#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Point.h"

class Pyramid;

struct Circle
{
	float x, y, z, r, r2;
};

class Triangle
{
public:
	bool removed = false;
	Triangle();
	Triangle(unsigned short indice1, unsigned short indice2, unsigned short indice3);
	void setIndices(unsigned short indice1, unsigned short indice2, unsigned short indice3);
	void setIndice1(unsigned short indice1);
	void setIndice2(unsigned short indice2);
	void setIndice3(unsigned short indice3);
	void setCircumCenter(Circle& c);
	unsigned short getIndice1();
	unsigned short getIndice2();
	unsigned short getIndice3();
	void addIndice(unsigned short indice);
	Circle& getCircumCenter();
	bool hasCircumCircle();
	bool Triangle::operator==(Triangle& p);
	bool Triangle::operator!=(Triangle& p);
	~Triangle();
	std::shared_ptr<Pyramid> p1, p2;
	bool hasPoints(unsigned int p1, unsigned int p2, unsigned int p3);
	bool done = false;
private:
	unsigned short indice1, indice2, indice3;
	Circle circumCenter;
	bool circumCircle = false;
};

#endif
