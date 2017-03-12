#include "Cylinder.h"



const int Cylinder::DEFAULT_SIDES = 15;


Cylinder::Cylinder() : Cylinder(DEFAULT_SIDES) {
}

Cylinder::Cylinder(int sides) {
	if (sides < 3) {
		sides = 3;
	}
	float rotationAngle = glm::radians(360.0f) / sides;

	Vertex originTop(glm::vec3(0, 0.5f, 0));
	Vertex originBottom(glm::vec3(0, -0.5f, 0));

	Vertex rotationPointTop(glm::vec3(0.5f, 0.5f, 0));
	Vertex rotationPointBottom(glm::vec3(0.5f, -0.5f, 0));
	Vertex rotationPointTopDup(glm::vec3(0.5f, 0.5f, 0));
	Vertex rotationPointBottomDup(glm::vec3(0.5f, -0.5f, 0));
	

	int size = 0;
	for (int i = 0; i < sides; i++) {
		
		Vertex nextRotationPointTop;
		Vertex nextRotationPointBottom;
		Vertex nextRotationPointTopDup;
		Vertex nextRotationPointBottomDup;

		nextRotationPointTop.position = glm::rotateY(rotationPointTop.position, rotationAngle);
		nextRotationPointBottom.position = glm::rotateY(rotationPointBottom.position, rotationAngle);

		nextRotationPointTopDup.position = nextRotationPointTop.position;
		nextRotationPointBottomDup.position = nextRotationPointBottom.position;

		// Top face
		addVertex(originTop);
		addVertex(rotationPointTop);
		addVertex(nextRotationPointTop);
		addIndices(glm::ivec3(size, size + 1, size + 2));
		size += 3;
		
		// Bottom face
		addVertex(originBottom);
		addVertex(rotationPointBottom);
		addVertex(nextRotationPointBottom);
		addIndices(glm::ivec3(size + 2, size + 1, size));
		size += 3;

		// Side edges
		addVertex(rotationPointTopDup);
		addVertex(rotationPointBottomDup);
		addVertex(nextRotationPointBottomDup);
		addIndices(glm::ivec3(size, size + 1, size + 2));
		size += 3;

		addVertex(rotationPointTopDup);
		addVertex(nextRotationPointBottomDup);
		addVertex(nextRotationPointTopDup);
		addIndices(glm::ivec3(size, size + 1, size + 2));
		size += 3;
		
		rotationPointTop = nextRotationPointTop;
		rotationPointBottom = nextRotationPointBottom;
		rotationPointTopDup = nextRotationPointTopDup;
		rotationPointBottomDup = nextRotationPointBottomDup;

		
	}
}


Cylinder::~Cylinder() {
}
