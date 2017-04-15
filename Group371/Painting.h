#ifndef PAINTING_H
#define PAINTING_H

#include "Mesh.h"
#include "MeshManager.h"
#include "Cube.h"
#include "Plane.h"
#include <unordered_map>


class Painting : public Mesh {
public:
	Painting(std::unordered_map<std::string, Texture>* textureMap);
	~Painting();

	void setBack(bool);		//back picture frame
	void setCanvas(bool);	//blank canvas to be drawn on
	void setShape(bool);	//shape to be drawn on canvas
	void addRandomShape(float minusX, float x, float minusY, float y, float z);

	const float FRAME_WIDTH = 0.1f;
private:
	void onChange();

	MeshManager manager;

	Mesh* back;
	Mesh* canvas;
	Mesh* shape;

	std::unordered_map<std::string, Texture>* textureMap;
	std::vector<Mesh*> randomShapes;
};

#endif // !PAINTING_H