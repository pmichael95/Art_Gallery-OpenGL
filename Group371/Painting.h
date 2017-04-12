#ifndef PAINTING_H
#define PAINTING_H

#include "Mesh.h"
#include "MeshManager.h"


class Painting : public Mesh {
public:
	Painting();
	~Painting();

	void setBack(bool);		//back picture frame
	void setCanvas(bool);	//blank canvas to be drawn on
	void setShape(bool);	//shape to be drawn on canvas
	void addRandomShape(float minusX, float x, float minusY, float y, float z);

	void setBackColor(glm::vec4);
	void setCanvasColor(glm::vec4);
	void setShapeColor(glm::vec4);

private:
	void onChange();

	MeshManager manager;

	Mesh* back;
	Mesh* canvas;
	Mesh* shape;

	std::vector<Mesh*> randomShapes;
};

#endif // !PAINTING_H