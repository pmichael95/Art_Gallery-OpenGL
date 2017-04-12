#include <iostream>
#include "Painting.h"

// Outside faces are culled
Painting::Painting() {
	back = new Mesh();
	canvas = new Mesh();
	shape = new Mesh();

	//manager compute called for nothing in these, the function doesnt do much at least, sorry ~raz :)
	setBack(true);
	setCanvas(true);
	setShape(true);

	manager.addMesh(back);
	manager.addMesh(canvas);
	manager.addMesh(shape);

	onChange();
}

Painting::~Painting() {
}

void Painting::onChange() {
	std::cout << "onchange" << std::endl;
	//recompute mesh on changes
	manager.computeMergedMesh();
	Mesh computedMesh = manager.finalMesh;
	vertices = computedMesh.getVertices();
	indices = computedMesh.getIndices();
}

void Painting::setBack(bool b) {
	back->reset();
	if (b) {
		//bottom frame
		back->addVertex(Vertex(glm::vec3(-0.75f, -0.5f, 0.5f)));
		back->addVertex(Vertex(glm::vec3(-0.75f, -0.4f, 0.5f)));
		back->addVertex(Vertex(glm::vec3(0.75f, -0.5f, 0.5f)));
		back->addVertex(Vertex(glm::vec3(0.75f, -0.4f, 0.5f)));
		back->addIndices(glm::vec3(0, 2, 1));
		back->addIndices(glm::vec3(1, 2, 3));
		//left frame
		back->addVertex(Vertex(glm::vec3(-0.75f, -0.5f, 0.5f)));
		back->addVertex(Vertex(glm::vec3(-0.65f, -0.5f, 0.5f)));
		back->addVertex(Vertex(glm::vec3(-0.75f, 0.5f, 0.5f)));
		back->addVertex(Vertex(glm::vec3(-0.65f, 0.5f, 0.5f)));
		back->addIndices(glm::vec3(4, 5, 6));
		back->addIndices(glm::vec3(5, 7, 6));
		//top frame
		back->addVertex(Vertex(glm::vec3(-0.75f, 0.5f, 0.5f)));
		back->addVertex(Vertex(glm::vec3(-0.75f, 0.4f, 0.5f)));
		back->addVertex(Vertex(glm::vec3(0.75f, 0.5f, 0.5f)));
		back->addVertex(Vertex(glm::vec3(0.75f, 0.4f, 0.5f)));
		back->addIndices(glm::vec3(8, 9, 10));
		back->addIndices(glm::vec3(9, 11, 10));
		//right frame
		back->addVertex(Vertex(glm::vec3(0.75f, -0.5f, 0.5f)));
		back->addVertex(Vertex(glm::vec3(0.65f, -0.5f, 0.5f)));
		back->addVertex(Vertex(glm::vec3(0.75f, 0.5f, 0.5f)));
		back->addVertex(Vertex(glm::vec3(0.65f, 0.5f, 0.5f)));
		back->addIndices(glm::vec3(12, 14, 13));
		back->addIndices(glm::vec3(13, 14, 15));
	}
	onChange();
}
void Painting::setCanvas(bool b) {
	canvas->reset();
	if (b) {
		canvas->addVertex(Vertex(glm::vec3(-0.65f, 0.4f, 0.5f))); // top far left
		canvas->addVertex(Vertex(glm::vec3(-0.65f, -0.4f, 0.5f))); // top near left
		canvas->addVertex(Vertex(glm::vec3(0.65f, 0.4f, 0.5f))); // bottom near left
		canvas->addVertex(Vertex(glm::vec3(0.65f, -0.4f, 0.5f))); // bottom far left
		canvas->addIndices(glm::vec3(0, 1, 2));
		canvas->addIndices(glm::vec3(1, 3, 2));
	}
	onChange();
}
void Painting::setShape(bool b) {
	addRandomShape(-0.65f, 0.65f, -0.4f, 0.4f, 0.505f);
	addRandomShape(-0.65f, 0.65f, -0.4f, 0.4f, 0.506f);
	addRandomShape(-0.65f, 0.65f, -0.4f, 0.4f, 0.507f);
	addRandomShape(-0.65f, 0.65f, -0.4f, 0.4f, 0.508f);
	addRandomShape(-0.65f, 0.65f, -0.4f, 0.4f, 0.509f);
	addRandomShape(-0.65f, 0.65f, -0.4f, 0.4f, 0.510f);
	addRandomShape(-0.65f, 0.65f, -0.4f, 0.4f, 0.511f);

	onChange();
}

void Painting::addRandomShape(float minusX, float x, float minusY, float y, float z)
{
	int maxVertices = 8;
	int minVertices = 3;
	Mesh* paintingShape = new Mesh();
	paintingShape->reset();

	int numVertices = minVertices + (std::rand() % (maxVertices - minVertices + 1));
	int numTriangles = numVertices - 2;

	for (int i = 0; i < numVertices; i++)
	{
		float newX = minusX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (x - minusX)));
		float newY = minusY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (y - minusY)));
		paintingShape->addVertex(Vertex(glm::vec3(newX, newY, z)));
	}
	for (int j = 0; j < numTriangles; j++)
	{
		paintingShape->addIndices(glm::vec3(0+j, 1+j, 2+j));
		paintingShape->addIndices(glm::vec3(0 + j, 2 + j, 1 + j));
	}
	
	float newRed = 0.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - 0.0f)));;
	float newGreen = 0.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - 0.0f)));;
	float newBlue = 0.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - 0.0f)));;
	paintingShape->setColor(glm::vec4(newRed, newBlue, newGreen, 1.0f));
	manager.addMesh(paintingShape);
	onChange();

	paintingShape = nullptr;
}

void Painting::setBackColor(glm::vec4 color) {
	back->setColor(color);
	onChange();
}
void Painting::setCanvasColor(glm::vec4 color) {
	canvas->setColor(color);
	onChange();
}
void Painting::setShapeColor(glm::vec4 color) {
	shape->setColor(color);
	onChange();
}