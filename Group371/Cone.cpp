#include "Cone.h"

const int Cone::DEFAULT_SIDES = 15;

Cone::Cone() : Cone(DEFAULT_SIDES) {

}

Cone::Cone(int sides) {
	if (sides < 3) {
		sides = 3;
	}
	float rotationAngle = glm::radians(360.0f) / sides;
	Vertex originTop(glm::vec3(0,0.5,0));
	Vertex originBottom(glm::vec3(0, -0.5, 0));
	Vertex rotationPoint(glm::vec3(0.5, -0.5, 0));

	addVertex(originTop);
	addVertex(originBottom);
	int size = 2;
	for (int i = 0; i < sides; i++) {
		Vertex nextRotationPoint;
		nextRotationPoint.position = glm::rotateY(rotationPoint.position, rotationAngle);
		addVertex(rotationPoint);
		addVertex(nextRotationPoint);
		addIndices(glm::ivec3(1, size, size + 1));
		addVertex(originTop);
		addIndices(glm::ivec3(size, size + 1, 0));
		size += 3;
		rotationPoint = nextRotationPoint;
	}



}


Cone::~Cone() {
}
