#pragma once
#include "Room.h"
#include "Hallway.h"
class RoomGen : public Mesh {
public:
	RoomGen();
	~RoomGen();
private:
	MeshManager manager;

	std::vector<Room*> rooms; //store generated rooms
	std::vector<std::pair<Room*, Room*>> connections; //store connections between rooms
	
	void addHallways();
	Room* getClosestRoom(Room);
	float getDistanceBetween(Room, Room);

	void connect(Room* r1, Room* r2);
	void connectHorizontally(Room* r1, Room* r2);
	void connectVertically(Room* r1, Room* r2);
	void connectIndirectly(Room* r1, Room* r2);
	void connectIndirectlyHorizontalFirst(Room* r1, Room* r2);
	void connectIndirectlyVerticalFirst(Room* r1, Room* r2);
	bool canConnectHorizontally(Room, Room);
	bool canConnectVertically(Room, Room);

	bool areConnected(Room*, Room*);

	void removeFromVector(std::vector<Room*>&, Room*);

	float ROOM_HEIGHT = 4.0f;
	float MIN_TUNNEL_WIDTH = 1.5f;	//minimum width for a hallway
};

