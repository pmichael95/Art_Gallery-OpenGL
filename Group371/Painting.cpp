#include <iostream>
#include "Painting.h"

const float Painting::FRAME_LENGTH = 0.1f;

// Outside faces are culled
Painting::Painting(float length, float height, float width)
	: length(length), height(height), width(width){
	addFrame();
	addCanvas();
	addShapes();
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

void Painting::addFrame() {
		int repeat = 1;
		Mesh rightFrame = Cube();
		rightFrame.localScaleMesh(glm::vec3(FRAME_LENGTH, height - FRAME_LENGTH*2, width));
		rightFrame.translateMesh(glm::vec3(length/2.0f - FRAME_LENGTH / 2.0f, 0.0f, 0.0f));
		Texture t = Texture::getTexture("floor");
		rightFrame.setTexture(t, repeat, repeat);
		manager.addMesh(rightFrame);

		Mesh leftFrame = Cube();
		leftFrame.localScaleMesh(glm::vec3(FRAME_LENGTH, height - FRAME_LENGTH*2, width));
		leftFrame.translateMesh(glm::vec3(-length / 2.0f + FRAME_LENGTH / 2.0f, 0.0f, 0.0f));
		leftFrame.setTexture(Texture::getTexture("floor"), repeat, repeat);
		manager.addMesh(leftFrame);

		Mesh topFrame = Cube();
		topFrame.localScaleMesh(glm::vec3(length, FRAME_LENGTH, width));
		topFrame.translateMesh(glm::vec3(0.0f, height / 2.0f - FRAME_LENGTH / 2.0f, 0.0f));
		topFrame.setTexture(Texture::getTexture("floor"), repeat, repeat);
		manager.addMesh(topFrame);

		Mesh bottomFrame = Cube();
		bottomFrame.localScaleMesh(glm::vec3(length, FRAME_LENGTH, width));
		bottomFrame.translateMesh(glm::vec3(0.0f, -height / 2.0f + FRAME_LENGTH / 2.0f, 0.0f));
		bottomFrame.setTexture(Texture::getTexture("floor"), repeat, repeat);
		manager.addMesh(bottomFrame);
}
void Painting::addCanvas() {
	Mesh plane = Plane();
	glm::vec3 amb = glm::vec3(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)), static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)), static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)));
	Material m{ 
		amb, amb, 
		glm::vec3(1.0f, 1.0f, 1.0f), 
		128};
	plane.setMaterial(m);
	plane.rotateMesh(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	plane.localScaleMesh(glm::vec3(length - FRAME_LENGTH, height - FRAME_LENGTH, 1.0f));
	manager.addMesh(plane);
}
void Painting::addShapes() {
	int max_shapes = 35;
	int min_shapes = 3;
	int count = rand() % (max_shapes - min_shapes) + min_shapes;
	for (int i = 1; i <= count; i++) {
		addRandomShape(-length/2.0f + FRAME_LENGTH*1.5f, length / 2.0f - FRAME_LENGTH*1.5f, -height/2.0f + FRAME_LENGTH*1.5f, height / 2.0f - FRAME_LENGTH*1.5f, 0.001f * i);
	}
}

void Painting::addRandomShape(float minusX, float x, float minusY, float y, float z)
{
	
	Mesh paintingShape = Mesh();

	glm::vec3 coords[3];

	for (int i = 0; i < 3; ++i) {
		float newX = minusX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (x - minusX)));
		float newY = minusY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (y - minusY)));
		coords[i] = glm::vec3(newX, newY, z);
	}
	Vertex v1 = Vertex(coords[0]);
	Vertex v2 = Vertex(coords[1]);
	Vertex v3 = Vertex(coords[2]);

	v1.uv = glm::vec2(0, 0);
	v2.uv = glm::vec2(1, 0);
	v3.uv = glm::vec2(0, 1);

	paintingShape.addVertex(v1);
	paintingShape.addVertex(v2);
	paintingShape.addVertex(v3);
	paintingShape.addIndices(glm::vec3(0, 1, 2));
	
		
	float newRed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
	float newGreen = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
	float newBlue = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
	glm::vec3 amb = glm::normalize(glm::vec3(newRed, newGreen, newBlue));
	Material m{
		amb, amb,
		glm::vec3(1.0f, 1.0f, 1.0f),
		128.0f };
	paintingShape.setMaterial(m);
	manager.addMesh(paintingShape);
}

std::vector<BoundingBox> Painting::getBoundingBox() {
	float extraSpace = 0.135f;
	std::vector<BoundingBox> boxes;
	glm::vec3 max_XYZ = this->manager.finalMesh.maxXYZ;
	glm::vec3 min_XYZ = this->manager.finalMesh.minXYZ;

	BoundingBox l;
	l.max = glm::vec3(maxXYZ.x + extraSpace, maxXYZ.y + extraSpace, maxXYZ.z + extraSpace);
	l.min = glm::vec3(maxXYZ.x - extraSpace, min_XYZ.y - extraSpace, min_XYZ.z - extraSpace);
	boxes.push_back(l);

	BoundingBox r;
	r.max = glm::vec3(min_XYZ.x + extraSpace, max_XYZ.y + extraSpace, max_XYZ.z + extraSpace);
	r.min = glm::vec3(min_XYZ.x - extraSpace, min_XYZ.y - extraSpace, min_XYZ.z - extraSpace);
	boxes.push_back(r);

	BoundingBox f;
	f.max = glm::vec3(max_XYZ.x + extraSpace, max_XYZ.y + extraSpace, max_XYZ.z + extraSpace);
	f.min = glm::vec3(min_XYZ.x - extraSpace, min_XYZ.y - extraSpace, max_XYZ.z - extraSpace);
	boxes.push_back(f);

	BoundingBox b;
	b.max = glm::vec3(max_XYZ.x + extraSpace, max_XYZ.y + extraSpace, min_XYZ.z + extraSpace);
	b.min = glm::vec3(min_XYZ.x - extraSpace, min_XYZ.y - extraSpace, min_XYZ.z - extraSpace);
	boxes.push_back(b);

	BoundingBox t;
	t.max = glm::vec3(max_XYZ.x + extraSpace, max_XYZ.y + extraSpace, max_XYZ.z + extraSpace);
	t.min = glm::vec3(min_XYZ.x - extraSpace, max_XYZ.y - extraSpace, min_XYZ.z - extraSpace);
	boxes.push_back(t);

	BoundingBox bot;
	bot.max = glm::vec3(max_XYZ.x + extraSpace, min_XYZ.y + extraSpace, max_XYZ.z + extraSpace);
	bot.min = glm::vec3(min_XYZ.x - extraSpace, min_XYZ.y - extraSpace, min_XYZ.z - extraSpace);
	boxes.push_back(bot);

	return boxes;
}