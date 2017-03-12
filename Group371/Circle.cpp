
#include "Circle.h"

const int Circle::DEFAULT_SIDES = 15;


Circle::Circle() : Circle(DEFAULT_SIDES) {}
Circle::Circle(int sides) {
	if (sides < 3) {
		sides = 3;
	}
	Vertex origin(glm::vec3(0,0,0));
	Vertex rotationPoint(glm::vec3(0.5f, 0, 0));
	float rotationAngle = glm::radians(360.0f) / sides ;
	
	int size = 0;
	for (int i = 0; i < sides; i++) {
		Vertex nextRotationPoint;
		nextRotationPoint.position = glm::rotateY(rotationPoint.position, rotationAngle);	
		addVertex(origin);
		addVertex(rotationPoint);
		addVertex(nextRotationPoint);
		glm::ivec3 indice = glm::ivec3(size, size + 1, size + 2);
		addIndices(indice);
		size += 3;
		rotationPoint = nextRotationPoint;
	}

}


Circle::~Circle() {
}
