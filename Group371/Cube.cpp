
#include "Cube.h"



// Outside faces are culled
Cube::Cube() {
	// Bottom face
	addVertex(Vertex(glm::vec3(0.5f, -0.5f, 0.5f)));// bottom far right
	addVertex(Vertex(glm::vec3(0.5f, -0.5f, -0.5f))); // bottom near right
	addVertex(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f))); // bottom near left
	addVertex(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f))); // bottom far left
	addIndices(glm::vec3(0, 1, 2));
	addIndices(glm::vec3(0, 2, 3));

	// Top face
	addVertex(Vertex(glm::vec3(0.5f, 0.5f, 0.5f))); // top far right
	addVertex(Vertex(glm::vec3(0.5f, 0.5f, -0.5f))); // top near right
	addVertex(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f))); // top near left
	addVertex(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f))); // top far left
	addIndices(glm::vec3(6, 5, 4));
	addIndices(glm::vec3(7, 6, 4));

	// Left face
	addVertex(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f))); // top far left
	addVertex(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f))); // top near left
	addVertex(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f))); // bottom near left
	addVertex(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f))); // bottom far left
	addIndices(glm::vec3(8, 10, 9));
	addIndices(glm::vec3(11, 10, 8));

	// Right face
	addVertex(Vertex(glm::vec3(0.5f, 0.5f, 0.5f))); // top far right
	addVertex(Vertex(glm::vec3(0.5f, 0.5f, -0.5f))); // top near right
	addVertex(Vertex(glm::vec3(0.5f, -0.5f, -0.5f))); // bottom near right
	addVertex(Vertex(glm::vec3(0.5f, -0.5f, 0.5f))); // bottom far right
	addIndices(glm::vec3(13, 14, 12));
	addIndices(glm::vec3(12, 14, 15));

	// Top face
	addVertex(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f))); // top far left
	addVertex(Vertex(glm::vec3(0.5f, 0.5f, 0.5f))); // top far right
	addVertex(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f))); // bottom far left
	addVertex(Vertex(glm::vec3(0.5f, -0.5f, 0.5f))); // bottom far right
	addIndices(glm::vec3(17, 18, 16));
	addIndices(glm::vec3(19, 18, 17));

	// Bottom face
	addVertex(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f))); // top near left
	addVertex(Vertex(glm::vec3(0.5f, 0.5f, -0.5f))); // top near right
	addVertex(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f))); // bottom near left
	addVertex(Vertex(glm::vec3(0.5f, -0.5f, -0.5f))); // bottom near right
	addIndices(glm::vec3(20, 22, 21));
	addIndices(glm::vec3(21, 22, 23));

}


Cube::~Cube() {
}
