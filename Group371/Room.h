#pragma once
#include "Mesh.h"
#include "Cube.h"
#include <unordered_map>
#include "Texture.h"

class Room : public Mesh {
public:
	Room(std::unordered_map<std::string, Texture>* textureMap, float length, float width, float height = 3.0f, float x = 0, float y = 0);
	~Room();

	bool intersects(Room);
	
	void setLeftOpening(float from, float to);
	void setRightOpening(float from, float to);
	void setFrontOpening(float from, float to);
	void setBackOpening(float from, float to);

	const glm::vec2 getPosition();
	const float getLength();
	const float getWidth();
	const float getHeight();
	glm::vec2 getTopLeft();
	glm::vec2 getBottomRight();

	bool hasLeftOpening() { return hasLeftOpen; };
	bool hasRightOpening() { return hasRightOpen; };
	bool hasFrontOpening() { return hasFrontOpen; };
	bool hasBackOpening() { return hasBackOpen; };

	void addArtPieces();
private:
	Mesh getXOpening(float from, float to, bool isLeft);
	Mesh getYOpening(float from, float to, bool isFront);

	void onChange();
	void onChangeLeft();
	void onChangeRight();
	void onChangeFront();
	void onChangeBack();

	//walls & center
	MeshManager manager;
	Mesh* left;
	Mesh* right;
	Mesh* front;
	Mesh* back;
	Cube center;
	bool hasLeftOpen, hasRightOpen, hasFrontOpen, hasBackOpen;
	glm::vec2 leftOpening, rightOpening, frontOpening, backOpening;

	float length, width, height;
	glm::vec2 position;

	std::vector<Mesh*> objects;

	void setColors(Cube&);

	float sides_size = 0.0000001f;
	std::unordered_map<std::string, Texture>* textureMap;
};

