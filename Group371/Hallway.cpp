#include "Hallway.h"


Hallway::Hallway(std::unordered_map<std::string, Texture>* textureMap, float length, float width, float height, bool front, bool left, bool right) : length(length), width(width), height(height), left(left), right(right), textureMap(textureMap) {
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
	setColors(edge);
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
	int mix = 50;
	float repeat = 3;
	Material m = MATERIAL_EMERALD;
	Material m2 = MATERIAL_RED_PLASTIC;

	cube.setFaceTexture(CubeFace::front, textureMap->at("wall"), repeat, repeat);
	cube.setFaceTexture(CubeFace::back, textureMap->at("wall"), repeat, repeat);
	cube.setFaceMix(CubeFace::left, m, textureMap->at("wall"), repeat, repeat, 50);
	cube.setFaceMix(CubeFace::right, m2, textureMap->at("wall"), repeat, repeat, 50);
	cube.setFaceTexture(CubeFace::bottom, textureMap->at("floor"), length / 3.0f, 1);
	cube.setFaceTexture(CubeFace::top, textureMap->at("wall"));
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
