#include "RoomGen.h"
#include <iostream>
#include <utility>

RoomGen::RoomGen()
{
	int maxSize = 14; // max width/length of a room
	int minSize = 7; // min width/length of a room

	//dimensions of area to fill with rooms
	int fillAreaWidth = 40;
	int fillAreaHeight = 40;

	int density = 80; // density of rooms, this is the # of attempts of placing a room.
	
	Room* a = new Room(7.0f, 4.0f, 4.0f, -6.5f, 3.0f);
	manager.addMesh(a);
	rooms.push_back(a);
	Room* b = new Room(6.0f, 3.5f, ROOM_HEIGHT, 4.0f, 2.0f);
	manager.addMesh(b);
	rooms.push_back(b);
	Room* c = new Room(10.0f, 6.0f, ROOM_HEIGHT, -8.0f, -8.0f);
	manager.addMesh(c);
	rooms.push_back(c);
	Room* d = new Room(5.0f, 5.0f, ROOM_HEIGHT, -13.0f, 11.0f);
	manager.addMesh(d);
	rooms.push_back(d);
	Room* e = new Room(7.0f, 4.0f, ROOM_HEIGHT, -15.5f, 3.0f);
	manager.addMesh(e);
	rooms.push_back(e);
	Room* f = new Room(8.0f, 8.0f, ROOM_HEIGHT, 17.0f, 14.0f);
	manager.addMesh(f);
	rooms.push_back(f);
	Room* g = new Room(4.0f, 5.0f, ROOM_HEIGHT, -8.0f, 11.0f);
	manager.addMesh(g);
	rooms.push_back(g);
	Room* h = new Room(8.0f, 8.0f, ROOM_HEIGHT, 17.0f, -14.0f);
	manager.addMesh(h);
	rooms.push_back(h);
	Room* j = new Room(16.0f, 12.0f, ROOM_HEIGHT, 19.0f, -30.0f);
	manager.addMesh(j);
	rooms.push_back(j);
	Room* k = new Room(12.0f, 6.0f, ROOM_HEIGHT, 34.0f, -30.0f);
	manager.addMesh(k);
	rooms.push_back(k);

	for (int i = 0; i < density; i++) {
		//room dimensions
		int length = minSize + (rand() % (maxSize - minSize + 1));
		int width = minSize + (rand() % (maxSize - minSize + 1));

		//room position (positions by center of the room)
		int x = rand() % fillAreaWidth - fillAreaWidth/2.0f;
		int y = rand() % fillAreaHeight - fillAreaHeight/2.0f;

		//init new room
		Room* newRoom = new Room(length, width, ROOM_HEIGHT, x, y);

		//check if intersects with any other
		bool failed = false;
		for (std::vector<Room*>::iterator it = rooms.begin(); it != rooms.end(); ++it) {
			Room otherRoom = **it;
			if ((*newRoom).intersects(otherRoom)) {
				failed = true;
				break;
			}
		}

		if (!failed) {
			//add the new room
			//manager.addMesh(newRoom);
			//rooms.push_back(newRoom);
		}
	}

	addHallways();
	
	//generate vertices
	manager.computeMergedMesh();
	Mesh computedMesh = manager.finalMesh;
	vertices = computedMesh.getVertices();
	indices = computedMesh.getIndices();
}


RoomGen::~RoomGen()
{
	for (Room* r : rooms)
		delete r;
}

void RoomGen::addHallways() {
	//copy rooms to connect
	std::vector<Room*> roomsToConnect;
	for (Room* room : rooms) {
		roomsToConnect.push_back(room);
	}


	//while all rooms are not connected at least once
	while (roomsToConnect.size() != 0) {
		//pick a random room
		int roomAt = rand() % roomsToConnect.size();
		Room* room = roomsToConnect.at(roomAt);

		//get the closest room to it
		Room* closest = getClosestRoom(*room);

		//connect them
		if (!areConnected(room, closest)) {
			connect(room, closest);
			connections.push_back(std::pair<Room*,Room*>(room, closest));
		}

		removeFromVector(roomsToConnect, room);
	}
}

Room* RoomGen::getClosestRoom(Room room) {
	Room* closest;// (0, 0, 0, INT_MAX, INT_MAX);
	float closestDistance = getDistanceBetween(room, Room(0,0,0, INT_MAX, INT_MAX));
	for (Room* r : rooms) {
		//skip self
		if (*r == room)
			continue;

		float distance = getDistanceBetween(room, *r);
		if (distance < closestDistance) {
			closest = r;
			closestDistance = distance;
		}
	}
	return closest;
}

float RoomGen::getDistanceBetween(Room r1, Room r2) {
	return glm::sqrt(pow(r1.getPosition().x - r2.getPosition().x, 2) + pow(r1.getPosition().y - r2.getPosition().y, 2));
}

void RoomGen::connect(Room* from, Room* to) {
	std::cout << "Connect (" << from->getPosition().x << ", " << from->getPosition().y << ") to (" << to->getPosition().x << ", " << to->getPosition().y << ")" << std::endl;

	if (canConnectHorizontally(*from, *to)) {
		connectHorizontally(from, to);
	}
	else if (canConnectVertically(*from, *to)) {
		connectVertically(from, to);
	}
	else {
		connectIndirectly(from, to);
	}
}

bool RoomGen::canConnectHorizontally(Room from, Room to) {
	//make sure our destination is to the right (positive x)
	bool isToRight = from.getPosition().x < to.getPosition().x;
	if (!isToRight) {
		//swap so destination is on right
		Room temp = from;
		from = to;
		to = temp;
	}

	//find out if we can connect them directly horizontally
	bool toIsHigher = to.getPosition().y > from.getPosition().y;

	//check if they vertically intersect
	bool verticallyIntersect;
	bool fullyIntersect = (from.getTopLeft().y >= to.getTopLeft().y && from.getBottomRight().y <= to.getBottomRight().y)
		|| (to.getTopLeft().y >= from.getTopLeft().y && to.getBottomRight().y <= from.getBottomRight().y);
	if (toIsHigher)
		verticallyIntersect = fullyIntersect || (from.getBottomRight().y <= to.getBottomRight().y && from.getTopLeft().y >= to.getBottomRight().y);
	else
		verticallyIntersect = fullyIntersect || (from.getBottomRight().y <= to.getTopLeft().y && from.getTopLeft().y >= to.getTopLeft().y);

	//check if theres enough space to connect them horizontally
	bool canHorizontal = false;
	if (verticallyIntersect) {
		if (toIsHigher)
			canHorizontal = (from.getTopLeft().y - to.getBottomRight().y) >= MIN_TUNNEL_WIDTH;
		else
			canHorizontal = (to.getTopLeft().y - from.getBottomRight().y) >= MIN_TUNNEL_WIDTH;
	}

	return canHorizontal;
}

void RoomGen::connectHorizontally(Room* from, Room* to) {
	//make sure our destination is to the right (positive x)
	bool isToRight = from->getPosition().x < to->getPosition().x;
	if (!isToRight) {
		//swap so destination is on right
		Room* temp = from;
		from = to;
		to = temp;
	}

	bool toIsHigher = to->getPosition().y > from->getPosition().y;

	float maxTunnelWidth;
	if (toIsHigher)
		maxTunnelWidth = from->getTopLeft().y - to->getBottomRight().y;
	else
		maxTunnelWidth = to->getTopLeft().y - from->getBottomRight().y;

	//limit to half the width of room
	float smallestHalfWidth = glm::min(from->getWidth(), to->getWidth()) / 2.0f;
	maxTunnelWidth = maxTunnelWidth > smallestHalfWidth ? smallestHalfWidth : maxTunnelWidth;

	float tunnelWidth = MIN_TUNNEL_WIDTH + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxTunnelWidth - MIN_TUNNEL_WIDTH)));
	float tunnelLength = to->getPosition().x - from->getPosition().x - to->getLength() / 2.0f - from->getLength() / 2.0f;
	float tunnelHeight = ROOM_HEIGHT;

	float tunnelCenterWorld;	//tunnel y in world
	if (toIsHigher)
		tunnelCenterWorld = from->getTopLeft().y - ((from->getTopLeft().y - to->getBottomRight().y) / 2.0f);
	else
		tunnelCenterWorld = from->getBottomRight().y + ((to->getTopLeft().y - from->getBottomRight().y) / 2.0f);

	float tunnelCenterFrom = tunnelCenterWorld - from->getBottomRight().y;
	float tunnelFromFrom = tunnelCenterFrom - tunnelWidth / 2.0f;
	float tunnelFromTo = tunnelCenterFrom + tunnelWidth / 2.0f;
	from->setFrontOpening(tunnelFromFrom, tunnelFromTo);

	float tunnelCenterTo = tunnelCenterWorld - to->getBottomRight().y;
	float tunnelToFrom = tunnelCenterTo - tunnelWidth / 2.0f;
	float tunnelToTo = tunnelCenterTo + tunnelWidth / 2.0f;
	to->setBackOpening(tunnelToFrom, tunnelToTo);

	Hallway* hallway = new Hallway(tunnelLength, tunnelWidth, tunnelHeight, true, false, false);
	float hallwayX = from->getBottomRight().x + tunnelLength / 2.0f;
	hallway->translateMesh(glm::vec3(tunnelCenterWorld, 0.0f, hallwayX));

	std::cout << "canHorizontal" << std::endl;
	manager.addMesh(hallway);
}

bool RoomGen::canConnectVertically(Room from, Room to) {
	//make sure our destination is to the top (positive y)
	bool isToTop = from.getPosition().y < to.getPosition().y;
	if (!isToTop) {
		//swap so destination is on top
		Room temp = from;
		from = to;
		to = temp;
	}

	//find out if we can connect them directly vertically
	bool toIsRighter = to.getPosition().x > from.getPosition().x;

	//check if they horizontally intersect
	bool horizontallyIntersect;
	bool fullyIntersect = (from.getTopLeft().x <= to.getTopLeft().x && from.getBottomRight().x >= to.getBottomRight().x) 
							|| (to.getTopLeft().x <= from.getTopLeft().x && to.getBottomRight().x >= from.getBottomRight().x);
	if (toIsRighter)
		horizontallyIntersect = fullyIntersect || (from.getBottomRight().x >= to.getTopLeft().x && from.getBottomRight().x <= to.getBottomRight().x);
	else
		horizontallyIntersect = fullyIntersect || (from.getTopLeft().x >= to.getTopLeft().x && from.getTopLeft().x <= to.getBottomRight().x);

	//check if theres enough space to connect them horizontally
	bool canVertical = false;
	if (horizontallyIntersect) {
		if (toIsRighter)
			canVertical = (from.getBottomRight().x - to.getTopLeft().x) >= MIN_TUNNEL_WIDTH;
		else
			canVertical = (to.getBottomRight().x - from.getTopLeft().x) >= MIN_TUNNEL_WIDTH;
	}

	return canVertical;
}

void RoomGen::connectVertically(Room* from, Room* to) {
	//make sure our destination is to the top (positive y)
	bool isToTop = from->getPosition().y < to->getPosition().y;
	if (!isToTop) {
		//swap so destination is on top
		Room* temp = from;
		from = to;
		to = temp;
	}

	bool toIsRighter = to->getPosition().x > from->getPosition().x;

	float maxTunnelWidth;
	if (toIsRighter)
		maxTunnelWidth = from->getBottomRight().x - to->getTopLeft().x;
	else
		maxTunnelWidth = to->getBottomRight().x - from->getTopLeft().x;

	//limit to half the length of room
	float smallestHalfLength = glm::min(from->getLength(), to->getLength()) / 2.0f;
	maxTunnelWidth = maxTunnelWidth > smallestHalfLength ? smallestHalfLength : maxTunnelWidth;

	float tunnelWidth = MIN_TUNNEL_WIDTH + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxTunnelWidth - MIN_TUNNEL_WIDTH)));
	float tunnelLength = to->getPosition().y - from->getPosition().y - to->getWidth() / 2.0f - from->getWidth() / 2.0f;
	float tunnelHeight = ROOM_HEIGHT;

	float tunnelCenterWorld;	//tunnel x in world
	if (toIsRighter)
		tunnelCenterWorld = from->getBottomRight().x - ((from->getBottomRight().x - to->getTopLeft().x) / 2.0f);
	else
		tunnelCenterWorld = from->getTopLeft().x + ((to->getBottomRight().x - from->getTopLeft().x) / 2.0f);

	float tunnelCenterFrom = tunnelCenterWorld - from->getTopLeft().x;
	float tunnelFromFrom = tunnelCenterFrom - tunnelWidth / 2.0f;
	float tunnelFromTo = tunnelCenterFrom + tunnelWidth / 2.0f;
	from->setLeftOpening(tunnelFromFrom, tunnelFromTo);

	float tunnelCenterTo = tunnelCenterWorld - to->getTopLeft().x;
	float tunnelToFrom = tunnelCenterTo - tunnelWidth / 2.0f;
	float tunnelToTo = tunnelCenterTo + tunnelWidth / 2.0f;
	to->setRightOpening(tunnelToFrom, tunnelToTo);

	Hallway* hallway = new Hallway(tunnelLength, tunnelWidth, tunnelHeight, true, false, false);
	float hallwayY = from->getTopLeft().y + tunnelLength / 2.0f;
	hallway->localRotateMesh(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	hallway->translateMesh(glm::vec3(hallwayY, 0.0f, tunnelCenterWorld));

	std::cout << "canVertical" << std::endl;
	manager.addMesh(hallway);
}

void RoomGen::connectIndirectly(Room* from, Room* to) {
	//make sure our destination is to the right (positive x)
	bool isToRight = from->getPosition().x < to->getPosition().x;
	if (!isToRight) {
		//swap so destination is on right
		Room* temp = from;
		from = to;
		to = temp;
	}

	if (!from->hasFrontOpening()) {
		connectIndirectlyHorizontalFirst(from, to);
	}
	else {
		connectIndirectlyVerticalFirst(from, to);
	}
}

void RoomGen::connectIndirectlyHorizontalFirst(Room* from, Room* to) {
	bool toIsHigher = to->getPosition().y > from->getPosition().y;

	//limit to half the length of room
	float maxTunnelWidth = glm::min(from->getWidth(), to->getLength()) / 2.0f;

	float tunnelWidth = MIN_TUNNEL_WIDTH + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxTunnelWidth - MIN_TUNNEL_WIDTH)));
	float hTunnelLength = to->getPosition().x - from->getPosition().x - from->getLength() / 2.0f + tunnelWidth / 2.0f;
	float vTunnelLength = glm::abs(to->getPosition().y - from->getPosition().y) - to->getWidth() / 2.0f - tunnelWidth / 2.0f;
	float tunnelHeight = ROOM_HEIGHT;

	float hTunnelCenterWorld = from->getPosition().y;
	float vTunnelCenterWorld = to->getPosition().x;

	float tunnelCenterFrom = hTunnelCenterWorld - from->getBottomRight().y;
	float tunnelFromFrom = tunnelCenterFrom - tunnelWidth / 2.0f;
	float tunnelFromTo = tunnelCenterFrom + tunnelWidth / 2.0f;
	from->setFrontOpening(tunnelFromFrom, tunnelFromTo);

	float tunnelCenterTo = vTunnelCenterWorld - to->getTopLeft().x;
	float tunnelToFrom = tunnelCenterTo - tunnelWidth / 2.0f;
	float tunnelToTo = tunnelCenterTo + tunnelWidth / 2.0f;
	if (toIsHigher)
		to->setRightOpening(tunnelToFrom, tunnelToTo);
	else
		to->setLeftOpening(tunnelToFrom, tunnelToTo);

	Hallway* hallwayH = new Hallway(hTunnelLength, tunnelWidth, tunnelHeight, false, toIsHigher, !toIsHigher);
	hallwayH->attach(Hallway(vTunnelLength, tunnelWidth, tunnelHeight, true, false, false));
	float hallwayX = from->getBottomRight().x + hTunnelLength / 2.0f;
	hallwayH->translateMesh(glm::vec3(hTunnelCenterWorld, 0.0f, hallwayX));

	std::cout << "indirect Horizontal first" << std::endl;
	manager.addMesh(hallwayH);
}

void RoomGen::connectIndirectlyVerticalFirst(Room* from, Room* to) {
	bool toIsHigher = to->getPosition().y > from->getPosition().y;

	//limit to half the length of room
	float maxTunnelWidth = glm::min(from->getLength(), to->getWidth()) / 2.0f;

	float tunnelWidth = MIN_TUNNEL_WIDTH + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxTunnelWidth - MIN_TUNNEL_WIDTH)));
	float vTunnelLength = glm::abs(to->getPosition().y - from->getPosition().y) - from->getWidth() / 2.0f + tunnelWidth / 2.0f; 
	float hTunnelLength = to->getPosition().x - from->getPosition().x - to->getLength() / 2.0f - tunnelWidth / 2.0f;
	float tunnelHeight = ROOM_HEIGHT;

	float vTunnelCenterWorld = from->getPosition().x;
	float hTunnelCenterWorld = to->getPosition().y;

	float tunnelCenterFrom = vTunnelCenterWorld - from->getTopLeft().x;
	float tunnelFromFrom = tunnelCenterFrom - tunnelWidth / 2.0f;
	float tunnelFromTo = tunnelCenterFrom + tunnelWidth / 2.0f;
	if (toIsHigher)
		from->setLeftOpening(tunnelFromFrom, tunnelFromTo);
	else
		from->setRightOpening(tunnelFromFrom, tunnelFromTo);

	float tunnelCenterTo = hTunnelCenterWorld - to->getBottomRight().y;
	float tunnelToFrom = tunnelCenterTo - tunnelWidth / 2.0f;
	float tunnelToTo = tunnelCenterTo + tunnelWidth / 2.0f;
	to->setBackOpening(tunnelToFrom, tunnelToTo);

	Hallway* hallwayH = new Hallway(vTunnelLength, tunnelWidth, tunnelHeight, false, !toIsHigher, toIsHigher);
	hallwayH->attach(Hallway(hTunnelLength, tunnelWidth, tunnelHeight, true, false, false));
	float hallwayY = toIsHigher ? from->getTopLeft().y + vTunnelLength / 2.0f : from->getBottomRight().y - vTunnelLength / 2.0f;
	hallwayH->localRotateMesh(glm::radians(toIsHigher ? 90.0f : 270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	hallwayH->translateMesh(glm::vec3(hallwayY, 0.0f, vTunnelCenterWorld));


	std::cout << "indirect Vertical first" << std::endl;
	manager.addMesh(hallwayH);
}

void RoomGen::removeFromVector(std::vector<Room*>& rooms, Room* room) {
	std::vector<Room*> newRooms;
	for (Room* r : rooms) {
		if (r != room)
			newRooms.push_back(r);
	}
	rooms = newRooms;
}

bool RoomGen::areConnected(Room* r1, Room* r2) {
	for (std::pair<Room*, Room*> connection : connections) {
		if ((connection.first == r1 && connection.second == r2) || (connection.first == r2 && connection.second == r1)) {
			return true;
		}
	}
	return false;
}