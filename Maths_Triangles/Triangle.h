#include "Point.h"
#include <vector>
#include <memory>

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
	void setCircumCenter(float x, float y);
	unsigned short getIndice1();
	unsigned short getIndice2();
	unsigned short getIndice3();
	void addIndice(unsigned short indice);
	float Triangle::getCircumCenterX();
	float Triangle::getCircumCenterY();
	~Triangle();
private:
	unsigned short indice1, indice2, indice3;
	float circumCenterX, circumCenterY;
};

