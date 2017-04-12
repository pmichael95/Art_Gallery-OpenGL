#include "Cube.h"

// Outside faces are culled
Cube::Cube(bool leftside, bool rightside, bool topside, bool bottomside, bool frontside, bool backside) {
	left = new Mesh();
	right = new Mesh();
	top = new Mesh();
	bottom = new Mesh();
	front = new Mesh();
	back = new Mesh();

	//manager compute called for nothing in these, the function doesnt do much at least, sorry ~raz :)
	setLeft(leftside);
	setRight(rightside);
	setTop(topside);
	setBottom(bottomside);
	setFront(frontside);
	setBack(backside);
	
	manager.addMesh(left);
	manager.addMesh(right);
	manager.addMesh(top);
	manager.addMesh(bottom);
	manager.addMesh(front);
	manager.addMesh(back);

	onChange();
}

Cube::~Cube() {
	//delete left, right, front, back, top, bottom;
}

void Cube::setSides(bool left, bool right, bool top, bool bottom, bool front, bool back) {
	setLeft(left);
	setRight(right);
	setTop(top);
	setBottom(bottom);
	setFront(front);
	setBack(back);
}

void Cube::onChange() {
	//recompute mesh on changes
	manager.computeMergedMesh();
	Mesh computedMesh = manager.finalMesh;
	vertices = computedMesh.getVertices();
	indices = computedMesh.getIndices();
}

void Cube::setLeft(bool b) {
	left->reset();
	if (b) {
		left->addVertex(Vertex(glm::vec3(0.5f, 0.5f, 0.5f))); // top far left
		left->addVertex(Vertex(glm::vec3(0.5f, 0.5f, -0.5f))); // top near left
		left->addVertex(Vertex(glm::vec3(0.5f, -0.5f, -0.5f))); // bottom near left
		left->addVertex(Vertex(glm::vec3(0.5f, -0.5f, 0.5f))); // bottom far left
		left->addIndices(glm::vec3(1, 2, 0));
		left->addIndices(glm::vec3(0, 2, 3));
	}
	onChange();
}
void Cube::setRight(bool b) {
	right->reset();
	if (b) {
		right->addVertex(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f))); // top far right
		right->addVertex(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f))); // top near right
		right->addVertex(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f))); // bottom near right
		right->addVertex(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f))); // bottom far right
		right->addIndices(glm::vec3(0, 2, 1));
		right->addIndices(glm::vec3(3, 2, 0));
	}
	onChange();
}
void Cube::setTop(bool b) {
	top->reset();
	if (b) {
		top->addVertex(Vertex(glm::vec3(0.5f, 0.5f, 0.5f))); // top far right
		top->addVertex(Vertex(glm::vec3(0.5f, 0.5f, -0.5f))); // top near right
		top->addVertex(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f))); // top near left
		top->addVertex(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f))); // top far left
		top->addIndices(glm::vec3(2, 1, 0));
		top->addIndices(glm::vec3(3, 2, 0));
	}
	onChange();
}
void Cube::setBottom(bool b) {
	bottom->reset();
	if (b) {
		bottom->addVertex(Vertex(glm::vec3(0.5f, -0.5f, 0.5f)));// bottom far right
		bottom->addVertex(Vertex(glm::vec3(0.5f, -0.5f, -0.5f))); // bottom near right
		bottom->addVertex(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f))); // bottom near left
		bottom->addVertex(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f))); // bottom far left
		bottom->addIndices(glm::vec3(0, 1, 2));
		bottom->addIndices(glm::vec3(0, 2, 3));
	}
	onChange();
}
void Cube::setFront(bool b) {
	front->reset();
	if (b) {
		front->addVertex(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f))); // top far left
		front->addVertex(Vertex(glm::vec3(0.5f, 0.5f, 0.5f))); // top far right
		front->addVertex(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f))); // bottom far left
		front->addVertex(Vertex(glm::vec3(0.5f, -0.5f, 0.5f))); // bottom far right
		front->addIndices(glm::vec3(1, 2, 0));
		front->addIndices(glm::vec3(3, 2, 1));
	}
	onChange();
}
void Cube::setBack(bool b) {
	back->reset();
	if (b) {
		back->addVertex(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f))); // top near left
		back->addVertex(Vertex(glm::vec3(0.5f, 0.5f, -0.5f))); // top near right
		back->addVertex(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f))); // bottom near left
		back->addVertex(Vertex(glm::vec3(0.5f, -0.5f, -0.5f))); // bottom near right
		back->addIndices(glm::vec3(0, 2, 1));
		back->addIndices(glm::vec3(1, 2, 3));
	}
	onChange();
}

void Cube::setLeftColor(glm::vec4 color) {
	left->setColor(color);
	onChange();
}
void Cube::setRightColor(glm::vec4 color) {
	right->setColor(color);
	onChange();
}
void Cube::setTopColor(glm::vec4 color) {
	top->setColor(color);
	onChange();
}
void Cube::setBottomColor(glm::vec4 color) {
	bottom->setColor(color);
	onChange();
}
void Cube::setFrontColor(glm::vec4 color) {
	front->setColor(color);
	onChange();
}
void Cube::setBackColor(glm::vec4 color) {
	back->setColor(color);
	onChange();
}