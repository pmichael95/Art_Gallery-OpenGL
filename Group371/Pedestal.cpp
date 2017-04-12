#include "Pedestal.h"

Pedestal::Pedestal() {
	// Defaults are: base is a cube, top is a cone
	this->base = Cube();
	this->top = new RandomShape();
	this->finalize();
}

Pedestal::Pedestal(Mesh* top) {
	// Default is that the base is a cube
	this->base = Cube();
	// Set the top part as the one we passed
	this->top = top;
}

Pedestal::~Pedestal() {
	delete this->top;
	this->top = nullptr;
}

// Set the top shape (will not always work due to scaling/translating)
void Pedestal::setTop(Mesh* top) {
	this->top = top;
	this->finalize();
}

// Set base color
void Pedestal::setBaseColor(glm::vec4 color) {
	this->base.setColor(color);
	this->finalize();
}

// Set top color
void Pedestal::setTopColor(glm::vec4 color) {
	this->top->setColor(color);
	this->finalize();
}

// Set all colors
void Pedestal::setAllColor(glm::vec4 color) {
	this->setBaseColor(color);
	this->setTopColor(color);
	this->finalize();
}

// Finalize & compute the merged mesh
void Pedestal::finalize() {
	std::cout << "Finalizing pedestal." << std::endl;
	manager.addMesh(&this->base);
	manager.addMesh(this->top);
	this->base.scaleMesh(glm::vec3(0.5f, 0.5f, 0.5f));
	this->top->scaleMesh(glm::vec3(0.5f, 0.5f, 0.5f));
	this->top->translateMesh(glm::vec3(0.0f, 0.07f, 0.0f));
	manager.computeMergedMesh();
	Mesh computedMesh = manager.finalMesh;
	vertices = computedMesh.getVertices();
	indices = computedMesh.getIndices();
}