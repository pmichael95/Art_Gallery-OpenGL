#ifndef HALLWAY_H
#define HALLWAY_H

#include "Cube.h"
#include "MeshManager.h"

class Hallway : public Mesh {
public:
	Hallway(float length, float width, float height, bool front, bool left = true, bool right = true);
	~Hallway();

	void attach(Hallway);
private:
	void setColors(Cube&);
	float length;
	float width;
	float height;
	bool left;
	bool right;
};

#endif // !HALLWAY_H