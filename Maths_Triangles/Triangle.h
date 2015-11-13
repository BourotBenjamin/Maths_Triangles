#include "Point.h"
#include <vector>
#include <memory>

class Triangle
{
public:
	Triangle(unsigned short indice1, unsigned short indice2, unsigned short indice3);
	void setIndices(unsigned short indice1, unsigned short indice2, unsigned short indice3);
	unsigned short getIndice1();
	unsigned short getIndice2();
	unsigned short getIndice3();
	~Triangle();
private:
	unsigned short indice1, indice2, indice3;
};

