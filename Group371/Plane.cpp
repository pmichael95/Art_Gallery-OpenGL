
#include "Plane.h"


Plane::Plane() {
	// Bottom face
	addVertex(Vertex(glm::vec3(0.5f, 0, 0.5f)));// bottom far right
	addVertex(Vertex(glm::vec3(0.5f, 0, -0.5f))); // bottom near right
	addVertex(Vertex(glm::vec3(-0.5f,0, -0.5f))); // bottom near left
	addVertex(Vertex(glm::vec3(-0.5f,0, 0.5f))); // bottom far left
	addIndices(glm::vec3(0, 1, 2));
	addIndices(glm::vec3(0, 2, 3));
}


Plane::~Plane() {
}
