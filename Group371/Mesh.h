#ifndef MESH_H
#define MESH_H

// GLEW
//#include <GL/glew.h> // Include glew to get all the required OpenGL headers

// GLFW
#include <GLFW/glfw3.h>

#include <vector>
#include "Vertex.h"
#include <glm/gtx/rotate_vector.hpp> 


// The struct that holds data about a mesh
class Mesh {
	friend class MeshManager;
public:

	Mesh();
	~Mesh();
	Mesh copy();
	void addVertex(Vertex& v);
	void addIndices(glm::ivec3 tri);
	const inline std::vector<Vertex>& getVertices() {return vertices;}
	const inline std::vector<int>& getIndices() {return indices;}
	int vertexBufferSize();
	int indexBufferSize();
	void scaleMesh(glm::vec3 scale);
	void localScaleMesh(glm::vec3 scale);
	void rotateMesh(float radians, glm::vec3 rotate);
	void localRotateMesh(float radians, glm::vec3 rotate);
	void tranlateMesh(glm::vec3 translate);
protected:
	void updateVerticesFromTransform();
	void recalculateCenter();
	void transformNormals(glm::mat4& transform);
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

