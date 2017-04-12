
#ifndef CUBE_H
#define CUBE_H

#include "Mesh.h"
#include "MeshManager.h"


class Cube : public Mesh {
public:
	Cube(bool leftside = true, bool rightside = true, bool topside = true, bool bottomside = true, bool frontside = true, bool backside = true);
	~Cube();

	void setSides(bool left, bool right, bool top, bool bottom, bool front, bool back);
	void setLeft(bool);
	void setRight(bool);
	void setTop(bool);
	void setBottom(bool);
	void setFront(bool);
	void setBack(bool);

	void setLeftColor(glm::vec4);
	void setRightColor(glm::vec4);
	void setTopColor(glm::vec4);
	void setBottomColor(glm::vec4);
	void setFrontColor(glm::vec4);
	void setBackColor(glm::vec4);
private:
	void onChange();

	MeshManager manager;

	Mesh* left;
	Mesh* right;

	Mesh* front;
	Mesh* back;

	Mesh* top;
	Mesh* bottom;
};

#endif // !CUBE_H