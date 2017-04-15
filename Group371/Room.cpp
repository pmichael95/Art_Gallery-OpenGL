#include "Room.h"
#include "Painting.h"
#include "Color.h"
#include "Pedestal.h"
#include <iostream>

Room::Room(std::unordered_map<std::string, Texture>* textureMap, float length, float width, float height, float x, float y) : width(width), length(length), height(height), position(x, y), textureMap(textureMap){
	center.setSides(false, false, true, true, false, false);
	setColors(center);
	center.localScaleMesh(glm::vec3(1.0f-sides_size, 1.0f, 1.0f-sides_size));
	manager.addMesh(&center);

	Cube* leftc = new Cube(true, false, true, true, false, false);
	setColors(*leftc);
	left = leftc;
	manager.addMesh(left);

	Cube* rightc = new Cube(false, true, true, true, false, false);
	setColors(*rightc);
	right = rightc;
	manager.addMesh(right);

	Cube* frontc = new Cube(false, false, true, true, true, false);
	setColors(*frontc);
	front = frontc;
	manager.addMesh(front);

	Cube* backc = new Cube(false, false, true, true, false, true);
	setColors(*backc);
	back = backc;
	manager.addMesh(back);

	onChangeLeft();
	onChangeRight();
	onChangeFront();
	onChangeBack();
	onChange();
}

Room::~Room() {
	//delete left, right, front, back;
}

void Room::onChange() {
	//recompute mesh on changes
	manager.computeMergedMesh();
	Mesh computedMesh = manager.finalMesh;
	computedMesh.localScaleMesh(glm::vec3(width, height, length));
	computedMesh.translateMesh(glm::vec3(position.y, 0.0f, position.x));
	vertices = computedMesh.getVertices();
	indices = computedMesh.getIndices();
}

void Room::onChangeLeft() {
	left->localScaleMesh(glm::vec3(sides_size, 1.0f, 1.0f));
	left->translateMesh(glm::vec3(0.5f - (sides_size / 2.0f), 0.0f, 0.0f));
}
void Room::onChangeRight() {
	right->localScaleMesh(glm::vec3(sides_size, 1.0f, 1.0f));
	right->translateMesh(glm::vec3(-0.5f + (sides_size / 2.0f), 0.0f, 0.0f));
}
void Room::onChangeFront() {
	front->localScaleMesh(glm::vec3(1.0f, 1.0f, sides_size));
	front->translateMesh(glm::vec3(0.0f, 0.0f, 0.5f - (sides_size / 2.0f)));
}
void Room::onChangeBack() {
	back->localScaleMesh(glm::vec3(1.0f, 1.0f, sides_size));
	back->translateMesh(glm::vec3(0.0f, 0.0f, -0.5f + (sides_size / 2.0f)));
}

bool Room::intersects(Room room) {
	float aLeft = position.x - (length / 2.0f);
	float aRight = position.x + (length / 2.0f);
	float aTop = position.y + (width / 2.0f);
	float aBottom = position.y - (width / 2.0f);

	float bLeft = room.position.x - (room.length / 2.0f);
	float bRight = room.position.x + (room.length / 2.0f);
	float bTop = room.position.y + (room.width / 2.0f);
	float bBottom = room.position.y - (room.width / 2.0f);

	return (aLeft <= bRight && aRight >= bLeft && aTop >= bBottom && aBottom <= bTop);
}

Mesh Room::getXOpening(float from, float to, bool isLeft) {
	//limit from/to to object length
	from = from > length ? length : from;
	to = to > length ? length : to;

	//normalize to 0-1
	from = from / length;
	to = to / length;

	Cube hole(false, false, true, true, false, false);
	Cube c1(isLeft, !isLeft, true, true, false, false);
	Cube c2(isLeft, !isLeft, true, true, false, false);

	setColors(hole);
	setColors(c1);
	setColors(c2);

	float holeSize = to - from;
	float c1size = from;
	float c2size = 1.0f - to;

	hole.localScaleMesh(glm::vec3(1.0f, 1.0f, holeSize));
	c1.localScaleMesh(glm::vec3(1.0f, 1.0f, c1size));
	c2.localScaleMesh(glm::vec3(1.0f, 1.0f, c2size));

	hole.translateMesh(glm::vec3(0.0f, 0.0f, -0.5 + from + holeSize / 2.0f));
	c1.translateMesh(glm::vec3(0.0f, 0.0f, -0.5 + c1size / 2.0f));
	c2.translateMesh(glm::vec3(0.0f, 0.0f, -0.5 + to + c2size / 2.0f));

	MeshManager manager;
	manager.addMesh(hole);
	manager.addMesh(c1);
	manager.addMesh(c2);
	manager.computeMergedMeshCopies();
	return manager.finalMesh;
}

Mesh Room::getYOpening(float from, float to, bool isFront) {
	//limit from/to to object width
	from = from > width ? width : from;
	to = to > width ? width : to;

	//normalize to 0-1
	from = from / width;
	to = to / width;

	Cube hole(false, false, true, true, false, false);
	Cube c1(false, false, true, true, isFront, !isFront);
	Cube c2(false, false, true, true, isFront, !isFront);

	setColors(hole);
	setColors(c1);
	setColors(c2);

	float holeSize = to - from;
	float c1size = from;
	float c2size = 1.0f - to;

	hole.localScaleMesh(glm::vec3(holeSize, 1.0f, 1.0f));
	c1.localScaleMesh(glm::vec3(c1size, 1.0f, 1.0f));
	c2.localScaleMesh(glm::vec3(c2size, 1.0f, 1.0f));

	hole.translateMesh(glm::vec3(-0.5 + from + holeSize / 2.0f, 0.0f, 0.0f));
	c1.translateMesh(glm::vec3(-0.5 + c1size / 2.0f, 0.0f, 0.0f));
	c2.translateMesh(glm::vec3(-0.5 + to + c2size / 2.0f, 0.0f, 0.0f));

	MeshManager manager;
	manager.addMesh(hole);
	manager.addMesh(c1);
	manager.addMesh(c2);
	manager.computeMergedMeshCopies();
	return manager.finalMesh;
}

void Room::setLeftOpening(float from, float to){
	//if from and to are the same, we have nothing to do
	if (from == to)
		return;

	Mesh mesh = getXOpening(from, to, true);
	hasLeftOpen = true;
	leftOpening = glm::vec2(from, to);

	left->reset();
	left->addVertices(mesh.getVertices());
	left->addIndices(mesh.getIndices());

	onChangeLeft();
	onChange();
}

void Room::setRightOpening(float from, float to){
	//if from and to are the same, we have nothing to do
	if (from == to)
		return;

	Mesh mesh = getXOpening(from, to, false);
	hasRightOpen = true;
	rightOpening = glm::vec2(from, to);

	right->reset();
	right->addVertices(mesh.getVertices());
	right->addIndices(mesh.getIndices());

	onChangeRight();
	onChange();
}

void Room::setFrontOpening(float from, float to){
	//if from and to are the same, we have nothing to do
	if (from == to)
		return;

	Mesh mesh = getYOpening(from, to, true);
	hasFrontOpen = true;
	frontOpening = glm::vec2(from, to);

	front->reset();
	front->addVertices(mesh.getVertices());
	front->addIndices(mesh.getIndices());

	onChangeFront();
	onChange();
}

void Room::setBackOpening(float from, float to){
	//if from and to are the same, we have nothing to do
	if (from == to)
		return;

	Mesh mesh = getYOpening(from, to, false);
	hasBackOpen = true;
	backOpening = glm::vec2(from, to);

	back->reset();
	back->addVertices(mesh.getVertices());
	back->addIndices(mesh.getIndices());

	onChangeBack();
	onChange();
}

void Room::setColors(Cube& cube) {
	int mix = 80;
	float repeat = 2;
	Material m = MATERIAL_EMERALD;
	Material m2 = MATERIAL_CHROME;

	cube.setFaceTexture(CubeFace::front, textureMap->at("wall"), repeat, repeat);
	cube.setFaceTexture(CubeFace::back, textureMap->at("wall"), repeat, repeat);
	cube.setFaceMix(CubeFace::left, m, textureMap->at("wall"), repeat, repeat, mix);
	cube.setFaceMix(CubeFace::right, m2, textureMap->at("wall"), repeat, repeat, mix);
	cube.setFaceTexture(CubeFace::bottom, textureMap->at("floor"), 8, 16);
	cube.setFaceTexture(CubeFace::top, textureMap->at("wall"));
}

const glm::vec2 Room::getPosition() {
	return position;
}
const float Room::getLength() {
	return length;
}
const float Room::getWidth() {
	return width;
}
const float Room::getHeight() {
	return height;
}

glm::vec2 Room::getTopLeft() {
	return glm::vec2(getPosition().x - getLength() / 2.0f, getPosition().y + getWidth() / 2.0f);
}

glm::vec2 Room::getBottomRight() {
	return glm::vec2(getPosition().x + getLength() / 2.0f, getPosition().y - getWidth() / 2.0f);
}

void Room::addArtPieces() {
	//vars to keep edges empty
	float lrSpace = width * 0.05f;
	float fbSpace = length * 0.05f;

	//min-max sizes
	float minPaintingHeight = height / 3.0f;
	float maxPaintingHeight = height / 2.0f;
	float minPaintingLength = 1.0f;
	float maxPaintingLength = 2.25f;

	float pedestalHeight = 2.5f;
	float pedestalWidth = 0.4f;
	float pedestalLength = 0.4f;

	//left wall
	std::vector<glm::vec2> openings; //empty space on wall
	if (hasLeftOpening()) {
		openings.push_back(glm::vec2(lrSpace, leftOpening.x - lrSpace));
		openings.push_back(glm::vec2(leftOpening.y + lrSpace, length - lrSpace));
	}
	else {
		openings.push_back(glm::vec2(lrSpace, length - lrSpace));
	}
	for (glm::vec2 openarea : openings) {
		std::cout << "openspace: " << openarea.x << "-" << openarea.y << std::endl;
		while (openarea.y - openarea.x >= minPaintingLength) {
			float openingLength = openarea.y - openarea.x;
			float maxLength = openingLength >= maxPaintingLength ? maxPaintingLength : openingLength;
			float length = minPaintingLength + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxLength - minPaintingLength)));
			float height = minPaintingHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxPaintingHeight - minPaintingHeight)));
			float width = 1.0f;

			Painting* p = new Painting(textureMap);
			
			objects.push_back(p);
			
			p->localScaleMesh(glm::vec3(length / this->length, height / this->height, width / this->width));
			p->localRotateMesh(glm::radians(-90.0f), glm::vec3(0, 1, 0));
			p->translateMesh(glm::vec3(0.4999f - p->FRAME_WIDTH/2.0f/this->length, 0.1f, -0.5f + (openarea.x + length / 2.0f) / this->length));

			openarea.x += length + 0.5f;

			manager.addMesh(p);

			std::cout << "Added painting of length: " << length << " at " << openarea.x + length / 2.0f << std::endl;

			//if we can add a pedestal and another painting
			if (openarea.y - openarea.x - pedestalLength - 0.5f >= minPaintingLength) {
				Pedestal* ped = new Pedestal();
				Material m = MATERIAL_OBSIDIAN;
				ped->setMaterial(m);
				objects.push_back(ped);

				ped->localScaleMesh(glm::vec3(pedestalWidth / this->width, pedestalHeight / this->height, pedestalLength / this->length));
				ped->translateMesh(glm::vec3(0.4999f - pedestalWidth / 2.0f / this->length, -pedestalHeight/4.0f/this->height, -0.5f + (openarea.x + pedestalLength / 2.0f) / this->length));

				openarea.x += pedestalLength + 0.5f;

				manager.addMesh(ped);
			}
		}
	}

	//right wall
	openings.clear();
	if (hasRightOpening()) {
		openings.push_back(glm::vec2(lrSpace, rightOpening.x - lrSpace));
		openings.push_back(glm::vec2(rightOpening.y + lrSpace, length - lrSpace));
	}
	else {
		openings.push_back(glm::vec2(lrSpace, length - lrSpace));
	}
	for (glm::vec2 openarea : openings) {
		std::cout << "openspace: " << openarea.x << "-" << openarea.y << std::endl;
		while (openarea.y - openarea.x >= minPaintingLength) {
			float openingLength = openarea.y - openarea.x;
			float maxLength = openingLength >= maxPaintingLength ? maxPaintingLength : openingLength;
			float length = minPaintingLength + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxLength - minPaintingLength)));
			float height = minPaintingHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxPaintingHeight - minPaintingHeight)));
			float width = 1.0f;

			Painting* p = new Painting(textureMap);
			objects.push_back(p);

			p->localScaleMesh(glm::vec3(length / this->length, height / this->height, width / this->width));
			p->localRotateMesh(glm::radians(90.0f), glm::vec3(0, 1, 0));
			p->translateMesh(glm::vec3(-0.4999f + p->FRAME_WIDTH / 2.0f / this->length, 0.1f, -0.5f + (openarea.x + length / 2.0f) / this->length));

			openarea.x += length + 0.5f;

			manager.addMesh(p);

			std::cout << "Added painting of length: " << length << " at " << openarea.x + length / 2.0f << std::endl;
		}
	}

	//front wall
	openings.clear();
	if (hasFrontOpening()) {
		openings.push_back(glm::vec2(fbSpace, frontOpening.x - fbSpace));
		openings.push_back(glm::vec2(frontOpening.y + fbSpace, width - fbSpace));
	}
	else {
		openings.push_back(glm::vec2(fbSpace, width - fbSpace));
	}
	for (glm::vec2 openarea : openings) {
		std::cout << "openspace: " << openarea.x << "-" << openarea.y << std::endl;
		while (openarea.y - openarea.x >= minPaintingLength) {
			float openingLength = openarea.y - openarea.x;
			float maxLength = openingLength >= maxPaintingLength ? maxPaintingLength : openingLength;
			float length = minPaintingLength + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxLength - minPaintingLength)));
			float height = minPaintingHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxPaintingHeight - minPaintingHeight)));
			float width = 1.0f;

			Painting* p = new Painting(textureMap);
			objects.push_back(p);

			p->localScaleMesh(glm::vec3(width / this->width, height / this->height, length / this->length));
			p->localRotateMesh(glm::radians(180.0f), glm::vec3(0, 1, 0));
			p->translateMesh(glm::vec3(-0.5f + (openarea.x + length / 2.0f) / this->width, 0.1f, 0.4999f - p->FRAME_WIDTH/2.0f / this->width));

			openarea.x += length + 0.5f;

			manager.addMesh(p);

			std::cout << "Added painting of length: " << length << " at " << openarea.x + length / 2.0f << std::endl;
		}
	}

	//back wall
	openings.clear();
	if (hasBackOpening()) {
		openings.push_back(glm::vec2(fbSpace, backOpening.x - fbSpace));
		openings.push_back(glm::vec2(backOpening.y + fbSpace, width - fbSpace));
	}
	else {
		openings.push_back(glm::vec2(fbSpace, width - fbSpace));
	}
	for (glm::vec2 openarea : openings) {
		std::cout << "openspace: " << openarea.x << "-" << openarea.y << std::endl;
		while (openarea.y - openarea.x >= minPaintingLength) {
			float openingLength = openarea.y - openarea.x;
			float maxLength = openingLength >= maxPaintingLength ? maxPaintingLength : openingLength;
			float length = minPaintingLength + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxLength - minPaintingLength)));
			float height = minPaintingHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxPaintingHeight - minPaintingHeight)));
			float width = 1.0f;

			Painting* p = new Painting(textureMap);
			objects.push_back(p);

			p->localScaleMesh(glm::vec3(width / this->width, height / this->height, length / this->length));
			//p->localRotateMesh(glm::radians(180.0f), glm::vec3(0, 1, 0));
			p->translateMesh(glm::vec3(-0.5f + (openarea.x + length / 2.0f) / this->width, 0.1f, -0.4999f + p->FRAME_WIDTH/2.0f / this->width));

			openarea.x += length + 0.5f;

			manager.addMesh(p);

			std::cout << "Added painting of length: " << length << " at " << openarea.x + length / 2.0f << std::endl;
		}
	}

	//center
	Pedestal* ped = new Pedestal();
	ped->localScaleMesh(glm::vec3(0.25f, 0.25f, 0.25f));
	ped->translateMesh(glm::vec3(0.0f, -0.5f, 0.0f));
	manager.addMesh(ped);
	objects.push_back(ped);

	onChange();
}