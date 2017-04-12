#ifndef MESH_H
#define MESH_H

// GLEW
//#include <GL/glew.h> // Include glew to get all the required OpenGL headers

// GLFW
#include <GLFW/glfw3.h>

#include <vector>
#include "Vertex.h"
#include <glm/gtx/rotate_vector.hpp> 
#include "Color.h"


// The struct that holds data about a mesh
class Mesh {
	friend class MeshManager;
public:

	Mesh();
	~Mesh();
	void addVertex(Vertex v);
	void addIndices(glm::ivec3 tri);
	void addVertices(const std::vector<Vertex> vertices);
	void addIndices(const std::vector<int> indices);
	void setColor(glm::vec4);
	void reset();

	Mesh copy();
	const inline std::vector<Vertex>& getVertices() {return vertices;}
	const inline std::vector<int>& getIndices() {return indices;}
	int vertexBufferSize();
	int indexBufferSize();

	void scaleMesh(glm::vec3 scale);
	void localScaleMesh(glm::vec3 scale);
	void rotateMesh(float radians, glm::vec3 rotate);
	void localRotateMesh(float radians, glm::vec3 rotate);
	void translateMesh(glm::vec3 translate);

	bool operator==(const Mesh&) const;
	bool operator!=(const Mesh&) const;
protected:
	void updateVerticesFromTransform();
	void recalculateCenter();
	void recalculateNormals();
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	std::vector<glm::ivec3> indiceGroups;
	glm::mat4 transform;

	glm::vec4 center;
	glm::vec4 maxXYZ;
	glm::vec4 minXYZ;
	
};

class GridMesh : public Mesh {
public:
	GridMesh();
	~GridMesh();
};

class XYZAxis : public Mesh {
public:
	XYZAxis();
	~XYZAxis();

};


#endif

