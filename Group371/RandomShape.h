#ifndef RANDOMSHAPE_H
#define RANDOMSHAPE_H

#include "Mesh.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>

#include "Cube.h"
#include "Plane.h"
#include "Circle.h"
#include "Cylinder.h"
#include "Cone.h"
#include "MeshManager.h"



class RandomShape : public Mesh {
public:
	RandomShape();
	~RandomShape();
private:
	enum Shapes{CUBE,CYLINDER,CONE,SUPER};
	Mesh Superform();
	float superformula(double m, double n1, double n2, double n3, double angle, float a, float b);
	float map(float value, float istart, float istop, float ostart, float ostop);
};

#endif // !RANDOMSHAPE_H