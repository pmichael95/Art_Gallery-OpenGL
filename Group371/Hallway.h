#ifndef HALLWAY_H
#define HALLWAY_H

#include "Cube.h"
#include "MeshManager.h"
#include <unordered_map>
#include "Texture.h"

class Hallway : public Mesh {
public:
	Hallway(std::unordered_map<std::string, Texture>* textureMap, float length, float width, float height, bool front, bool left = true, bool right = true);
	~Hallway();

	void attach(Hallway);
private:
	void setColors(Cube&);
	float length;
	float width;
	float height;
	bool left;
	bool right;

	std::unordered_map<std::string, Texture> * textureMap;
};

#endif // !HALLWAY_H