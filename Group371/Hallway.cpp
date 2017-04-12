#include "Hallway.h"


Hallway::Hallway(float length, float width, float height, bool front, bool left, bool right) : length(length), width(width), height(height), left(left), right(right) {
	MeshManager manager;

	if (length > width) {
		Cube hallway;
		hallway.setFront(false);
		hallway.setBack(false);
		setColors(hallway);
		hallway.localScaleMesh(glm::vec3(width, height, length - width));
		manager.addMesh(hallway);
	}

	Cube edge;
	edge.setBack(false);
	edge.setFront(!front);
	if (left)
		edge.setLeft(!left);
	if (right)
		edge.setRight(!right);
	edge.setLeftColor(glm::vec4(BLUE, 1.0f));
	edge.setRightColor(glm::vec4(BLUE, 1.0f));
	edge.setTopColor(glm::vec4(BLUE, 1.0f));
	edge.setFrontColor(glm::vec4(BLUE, 1.0f));
	edge.setBottomColor(glm::vec4(DARKORANGE, 1.0f));
	edge.localScaleMesh(glm::vec3(width, height, length > width ? width : length));
	if (length > width)
		edge.translateMesh(glm::vec3(0.0f, 0.0f, (length - width) / 2.0f + (width / 2.0f)));

	manager.addMesh(edge);

	manager.computeMergedMeshCopies();
	Mesh computedMesh = manager.finalMesh;
	if (length > width)
		computedMesh.translateMesh(glm::vec3(0.0f, 0.0f, -(width / 2.0f)));
	vertices = computedMesh.getVertices();
	indices = computedMesh.getIndices();
}

Hallway::~Hallway() {
}

void Hallway::setColors(Cube& cube) {
	cube.setTopColor(glm::vec4(BEIGE, 1.0f));
	cube.setBottomColor(glm::vec4(DARKORANGE, 1.0f));
	cube.setLeftColor(glm::vec4(LAVENDER, 1.0f));
	cube.setRightColor(glm::vec4(LAVENDER, 1.0f));
	cube.setBackColor(glm::vec4(FORESTGREEN, 1.0f));
	cube.setFrontColor(glm::vec4(BLUE, 1.0f));
}

void Hallway::attach(Hallway h) {
	MeshManager manager;

	if (left) {
		h.localRotateMesh(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		h.translateMesh(glm::vec3(width / 2.0f + h.length / 2.0f, 0.0f, length / 2.0f - width / 2.0f));
	}
	else {
		h.localRotateMesh(glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		h.translateMesh(glm::vec3(-(width / 2.0f + h.length / 2.0f), 0.0f, length / 2.0f - width / 2.0f));
	}

	manager.addMesh(*this);
	manager.addMesh(h);
	manager.computeMergedMeshCopies();
	Mesh final = manager.finalMesh;
	vertices = final.getVertices();
	indices = final.getIndices();
}
