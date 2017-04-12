#ifndef PEDESTAL_H
#define PEDESTAL_H

#include  "Mesh.h"
#include "Cube.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Circle.h"
#include "RandomShape.h"
#include "MeshManager.h"

class Pedestal : public Mesh {
public:
	Pedestal();
	Pedestal(Mesh* top);
	~Pedestal();

	// Set the top mesh type
	void setTop(Mesh* top);

	// Set the base's color only
	void setBaseColor(glm::vec4 color);
	// Set the top part's color only
	void setTopColor(glm::vec4 color);
	// Set all colors of the pedestal's parts together
	void setAllColor(glm::vec4 color);

	// Compute the final mesh
	void finalize();
private:
	Cube base;
	Mesh* top;
	MeshManager manager;
};

#endif