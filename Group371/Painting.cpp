#include <iostream>
#include "Painting.h"

// Outside faces are culled
Painting::Painting(std::unordered_map<std::string, Texture>* textureMap) {
	// TODO: find a better way to attach textures and stuff to objects
	this->textureMap = textureMap;
	setBack(true);
	setCanvas(true);
	setShape(true);
	onChange();
}



Painting::~Painting() {
}

void Painting::onChange() {
	std::cout << "onchange" << std::endl;
	//recompute mesh on changes
	manager.computeMergedMeshCopies();
	Mesh computedMesh = manager.finalMesh;
	vertices = computedMesh.getVertices();
	indices = computedMesh.getIndices();
}

void Painting::setBack(bool b) {
	if (b) {

		int repeat = 1;
		Mesh c1 = Cube();
		c1.localScaleMesh(glm::vec3(FRAME_WIDTH, 1.0f, FRAME_WIDTH));
		c1.translateMesh(glm::vec3(0.5f - FRAME_WIDTH/2.0f, 0.0f, 0.0f));
		c1.setTexture(textureMap->at("frame"), repeat, repeat);
		manager.addMesh(c1);

		Mesh c2 = Cube();
		c2.localScaleMesh(glm::vec3(FRAME_WIDTH, 1.0f, FRAME_WIDTH));
		c2.translateMesh(glm::vec3(-0.5f + FRAME_WIDTH / 2.0f, 0.0f, 0.0f));
		c2.setTexture(textureMap->at("frame"), repeat, repeat);
		manager.addMesh(c2);

		Mesh c3 = Cube();
		c3.localScaleMesh(glm::vec3(1.0f, FRAME_WIDTH, FRAME_WIDTH));
		c3.translateMesh(glm::vec3(0.0f, 0.5f - FRAME_WIDTH / 2.0f, 0.0f));
		c3.setTexture(textureMap->at("frame"), repeat, repeat);
		manager.addMesh(c3);

		Mesh c4 = Cube();
		c4.localScaleMesh(glm::vec3(1.0f, FRAME_WIDTH, FRAME_WIDTH));
		c4.translateMesh(glm::vec3(0.0f, -0.5f + FRAME_WIDTH / 2.0f, 0.0f));
		c4.setTexture(textureMap->at("frame"), repeat, repeat);
		manager.addMesh(c4);
	}
}
void Painting::setCanvas(bool b) {
	if (b) {
		Mesh plane = Plane();
		glm::vec3 amb = glm::vec3(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)), static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)), static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)));
		Material m{ 
			amb, amb, 
			glm::vec3(1.0f, 1.0f, 1.0f), 
			128};
		plane.setMaterial(m);
		plane.rotateMesh(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		manager.addMesh(plane);
	}
}
void Painting::setShape(bool b) {
	int count = 7; //maybe randomize
	for (int i = 1; i <= count; i++) {
		addRandomShape(-0.4f, 0.4f, -0.4f, 0.4f, 0.001f * i);
	}
}

void Painting::addRandomShape(float minusX, float x, float minusY, float y, float z)
{
	int maxVertices = 8;
	int minVertices = 3;
	Mesh paintingShape = Mesh();

	int numVertices = minVertices + (std::rand() % (maxVertices - minVertices + 1));
	int numTriangles = numVertices - 2;

	for (int i = 0; i < numVertices; i++)
	{
		float newX = minusX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (x - minusX)));
		float newY = minusY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (y - minusY)));
		paintingShape.addVertex(Vertex(glm::vec3(newX, newY, z)));
	}
	for (int j = 0; j < numTriangles; j++)
	{
		paintingShape.addIndices(glm::vec3(0+j, 1+j, 2+j));
		paintingShape.addIndices(glm::vec3(0 + j, 2 + j, 1 + j));
	}
	
	float newRed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
	float newGreen = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
	float newBlue = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
	glm::vec3 amb = glm::normalize(glm::vec3(newRed, newGreen, newBlue));
	Material m{
		amb, amb,
		glm::vec3(1.0f, 1.0f, 1.0f),
		128 };
	paintingShape.setMaterial(m);
	manager.addMesh(paintingShape);
}
