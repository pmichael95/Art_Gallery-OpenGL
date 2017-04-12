#include "Room.h"

Room::Room(float length, float width, float height, float x, float y) : width(width), length(length), height(height), position(x, y){
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
	leftOpening = true;

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
	rightOpening = true;

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
	frontOpening = true;

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
	backOpening = true;

	back->reset();
	back->addVertices(mesh.getVertices());
	back->addIndices(mesh.getIndices());

	onChangeBack();
	onChange();
}

void Room::setColors(Cube& cube) {
	cube.setTopColor(glm::vec4(BEIGE, 1.0f));
	cube.setBottomColor(glm::vec4(DARKORANGE, 1.0f));
	cube.setLeftColor(glm::vec4(LAVENDER, 1.0f));
	cube.setRightColor(glm::vec4(LAVENDER, 1.0f));
	cube.setBackColor(glm::vec4(FORESTGREEN, 1.0f));
	cube.setFrontColor(glm::vec4(BLUE, 1.0f));
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